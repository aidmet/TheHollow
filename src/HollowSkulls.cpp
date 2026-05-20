#include "HollowSkulls.hpp"
#include "HollowSpriteScale.hpp"

#include "layers/HollowGlitch.hpp"
#include "layers/HollowJournalLayer.hpp"

using namespace geode::prelude;

namespace hollow {

HollowSkullHandler* HollowSkullHandler::get() {
	static HollowSkullHandler instance;
	return &instance;
}

void HollowSkullHandler::onSkull(CCObject* sender) {
	onSkullClicked(sender);
}

void onSkullClicked(CCObject* sender) {
	auto item = typeinfo_cast<CCMenuItemSpriteExtra*>(sender);
	if (!item) return;

	int const index = item->getTag();
	auto& state = HollowState::get();
	if (state.isSkullFound(index)) return;

	state.markSkullFound(index);
	if (auto parent = item->getParent()) {
		parent->removeFromParent();
	}

	auto const found = state.skullsFound();
	log::info("[The Hollow] Skull {} collected ({}/{})", index + 1, found, SKULL_COUNT);

	if (found >= SKULL_COUNT) {
		if (auto scene = CCDirector::sharedDirector()->getRunningScene()) {
			runGlitchEffect(scene, []() {
				auto& s = HollowState::get();
				s.addClue(CLUE_SETTINGS);
				s.setSettingsUnlocked(true);
				s.setPhase(Phase::JournalClue2);
				CCDirector::sharedDirector()->pushScene(HollowJournalLayer::scene(false));
			});
		}
	}
}

void addSkullToLayer(CCLayer* layer, int skullIndex, CCPoint const& normalizedPos) {
	auto& state = HollowState::get();
	if (!state.isARGActive() || state.phase() != Phase::SkullHunt) return;
	if (state.isSkullFound(skullIndex)) return;

	auto winSize = CCDirector::sharedDirector()->getWinSize();
	auto pos = CCPoint { normalizedPos.x * winSize.width, normalizedPos.y * winSize.height };

	auto sprite = CCSprite::create("SkullIcon.png"_spr);
	if (!sprite || sprite->isUsingFallback()) {
		sprite = CCSprite::createWithSpriteFrameName("GJ_skull_001.png");
	}
	applyDisplaySize(sprite, SKULL_ICON_PX);
	sprite->setOpacity(110);

	auto item = CCMenuItemSpriteExtra::create(
		sprite,
		HollowSkullHandler::get(),
		menu_selector(HollowSkullHandler::onSkull)
	);
	item->setTag(skullIndex);
	item->setPosition(pos);

	auto menu = CCMenu::create();
	menu->setPosition({ 0, 0 });
	menu->addChild(item);
	layer->addChild(menu, 512);
}

} // namespace hollow
