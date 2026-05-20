# Generates compile_commands.json at the project root for clangd.
# Run after CMake configure (requires build/.cmake/api).
$ErrorActionPreference = "Stop"

$root = Split-Path -Parent $PSScriptRoot
$apiDir = Join-Path $root "build\.cmake\api\v1\reply"
if (-not (Test-Path $apiDir)) {
    Write-Error "CMake file API not found. Configure the project first (e.g. geode build)."
}

$targetFile = Get-ChildItem $apiDir -Filter "target-TheHollow-RelWithDebInfo-*.json" |
    Where-Object { $_.Name -notmatch "PACKAGE" } |
    Sort-Object LastWriteTime -Descending |
    Select-Object -First 1
if (-not $targetFile) {
    Write-Error "Could not find TheHollow RelWithDebInfo target in CMake file API."
}

$target = Get-Content $targetFile.FullName -Raw | ConvertFrom-Json
$mainSource = $target.sources | Where-Object { $_.path -eq "src/main.cpp" } | Select-Object -First 1
if (-not $mainSource) {
    Write-Error "src/main.cpp not found in CMake target sources."
}

$group = $target.compileGroups[$mainSource.compileGroupIndex]
$buildDir = (Join-Path $root "build") -replace '\\', '/'
$mainFile = (Join-Path $root "src/main.cpp") -replace '\\', '/'

$clangCmd = Get-Command clang++ -ErrorAction SilentlyContinue
$clang = if ($clangCmd) { ($clangCmd.Source -replace '\\', '/') } else { "clang++" }

$args = @(
    $clang,
    "-std=c++23",
    "--target=x86_64-pc-windows-msvc",
    "-Wno-unknown-warning-option",
    "-Wno-microsoft-enum-value"
)

foreach ($inc in $group.includes) {
    $args += "-I$($inc.path -replace '\\', '/')"
}

foreach ($def in $group.defines) {
    $d = $def.define
    # String macros (GEODE_MOD_ID, GEODE_GD_VERSION_STRING) must keep quotes for clang.
    if ($d -match '^([^=]+)=\\"(.+)\\"$') {
        $args += "-D$($Matches[1])=`"$($Matches[2])`""
    } elseif ($d -match '^([^=]+)="(.+)"$') {
        $args += "-D$($Matches[1])=`"$($Matches[2])`""
    } else {
        $args += "-D$d"
    }
}

foreach ($hdr in $group.precompileHeaders) {
    $args += "-include"
    $args += ($hdr.header -replace '\\', '/')
}

$args += "-c"
$args += $mainFile

$entry = @{
    directory = $buildDir
    file      = $mainFile
    arguments = $args
}

# PowerShell ConvertTo-Json emits a bare object for a single-element array — clangd needs [...].
$entryJson = ($entry | ConvertTo-Json -Depth 30 -Compress)
$json = "[$entryJson]"

$out = Join-Path $root "compile_commands.json"
$utf8NoBom = New-Object System.Text.UTF8Encoding $false
[System.IO.File]::WriteAllText($out, $json, $utf8NoBom)
Write-Host "Wrote $out"
