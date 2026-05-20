#pragma once

#include <Geode/Geode.hpp>
#include <algorithm>

using namespace geode::prelude;

namespace hollow {

// Target on-screen sizes (points) — source PNG resolution does not matter.
inline constexpr float JOURNAL_BUTTON_PX = 44.f;
inline constexpr float SKULL_ICON_PX = 28.f;
inline constexpr float BOSS_SPRITE_PX = 220.f;

inline void applyDisplaySize(CCSprite* sprite, float targetPx) {
	if (!sprite || targetPx <= 0.f) return;
	float const w = std::max(sprite->getContentWidth(), 1.f);
	float const h = std::max(sprite->getContentHeight(), 1.f);
	float const maxDim = std::max(w, h);
	sprite->setScale(targetPx / maxDim);
}

} // namespace hollow
