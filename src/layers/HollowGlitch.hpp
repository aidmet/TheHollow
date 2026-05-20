#pragma once

#include <Geode/Geode.hpp>
#include <functional>

using namespace geode::prelude;

namespace hollow {

void runGlitchEffect(CCNode* parent, std::function<void()> onComplete);

} // namespace hollow
