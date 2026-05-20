#pragma once

#include "HollowState.hpp"

#include <Geode/Geode.hpp>
#include <Geode/binding/CCMenuItemSpriteExtra.hpp>

using namespace geode::prelude;

namespace hollow {

class HollowSkullHandler : public CCObject {
public:
	static HollowSkullHandler* get();
	void onSkull(CCObject* sender);
};

void onSkullClicked(CCObject* sender);
void addSkullToLayer(CCLayer* layer, int skullIndex, CCPoint const& normalizedPos);

} // namespace hollow
