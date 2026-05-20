#include "HollowGlitch.hpp"

#include <Geode/utils/cocos.hpp>

using namespace geode::prelude;

namespace hollow {

void runGlitchEffect(CCNode* parent, std::function<void()> onComplete) {
	if (!parent) {
		if (onComplete) onComplete();
		return;
	}

	auto winSize = CCDirector::sharedDirector()->getWinSize();

	auto flash = CCLayerColor::create({ 255, 255, 255, 0 });
	flash->setContentSize(winSize);
	flash->setAnchorPoint({ 0, 0 });
	flash->setPosition({ 0, 0 });
	parent->addChild(flash, 9998);

	auto overlay = CCSprite::create("GlitchOverlay.png"_spr);
	if (overlay && !overlay->isUsingFallback()) {
		overlay->setPosition(winSize / 2);
		overlay->setScale(std::max(
			winSize.width / overlay->getContentWidth(),
			winSize.height / overlay->getContentHeight()
		));
		overlay->setOpacity(140);
		parent->addChild(overlay, 9999);

		overlay->runAction(CCSequence::create(
			CCRotateBy::create(0.4f, 180.f),
			CCFadeOut::create(0.2f),
			CCRemoveSelf::create(),
			nullptr
		));
	}

	auto actions = CCArray::create();
	for (int i = 0; i < 6; ++i) {
		actions->addObject(CCSequence::create(
			CCFadeTo::create(0.06f, 180),
			CCFadeTo::create(0.06f, 0),
			nullptr
		));
	}
	actions->addObject(CallFuncExt::create([flash, onComplete]() {
		flash->removeFromParent();
		if (onComplete) onComplete();
	}));

	flash->runAction(CCSequence::create(actions));
}

} // namespace hollow
