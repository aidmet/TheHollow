#include "HollowInterstitialLayer.hpp"

#include <Geode/utils/cocos.hpp>

using namespace geode::prelude;

namespace hollow {

HollowInterstitialLayer* HollowInterstitialLayer::create(std::function<void()> onComplete) {
	auto ret = new HollowInterstitialLayer();
	if (ret && ret->init(std::move(onComplete))) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

CCScene* HollowInterstitialLayer::scene(std::function<void()> onComplete) {
	auto scene = CCScene::create();
	scene->addChild(HollowInterstitialLayer::create(std::move(onComplete)));
	return scene;
}

bool HollowInterstitialLayer::init(std::function<void()> onComplete) {
	if (!CCLayer::init()) return false;

	m_onComplete = std::move(onComplete);

	auto winSize = CCDirector::sharedDirector()->getWinSize();

	auto black = CCLayerColor::create({ 0, 0, 0, 255 });
	black->setContentSize(winSize);
	black->setAnchorPoint({ 0, 0 });
	black->setPosition({ 0, 0 });
	this->addChild(black);

	auto label = CCLabelBMFont::create("FIND THE SKULLS", "goldFont.fnt");
	label->setScale(0.7f);
	label->setPosition(winSize / 2);
	this->addChild(label);

	this->runAction(CCSequence::create(
		CCDelayTime::create(3.5f),
		CallFuncExt::create([this]() {
			CCDirector::sharedDirector()->popScene();
			if (m_onComplete) m_onComplete();
		}),
		nullptr
	));

	return true;
}

} // namespace hollow
