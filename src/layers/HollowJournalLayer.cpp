#include "HollowJournalLayer.hpp"

#include "../HollowState.hpp"

#include <Geode/utils/cocos.hpp>

using namespace geode::prelude;

namespace hollow {

HollowJournalLayer* HollowJournalLayer::create(bool showJourneyAlert) {
	auto ret = new HollowJournalLayer();
	if (ret && ret->init(showJourneyAlert)) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

CCScene* HollowJournalLayer::scene(bool showJourneyAlert) {
	auto scene = CCScene::create();
	scene->addChild(HollowJournalLayer::create(showJourneyAlert));
	return scene;
}

bool HollowJournalLayer::init(bool showJourneyAlert) {
	if (!CCLayer::init()) return false;

	this->setKeyboardEnabled(true);
	this->setKeypadEnabled(true);

	auto winSize = CCDirector::sharedDirector()->getWinSize();

	auto bg = CCSprite::create("DungeonBg.png"_spr);
	if (bg && !bg->isUsingFallback()) {
		bg->setPosition(winSize / 2);
		bg->setScale(std::max(winSize.width / bg->getContentWidth(), winSize.height / bg->getContentHeight()) * 1.1f);
		this->addChild(bg);
	} else {
		auto fallback = CCLayerColor::create({ 20, 15, 25, 255 });
		fallback->setContentSize(winSize);
		fallback->setAnchorPoint({ 0, 0 });
		fallback->setPosition({ 0, 0 });
		this->addChild(fallback);
	}

	auto veil = CCLayerColor::create({ 0, 0, 0, 120 });
	veil->setContentSize(winSize);
	veil->setAnchorPoint({ 0, 0 });
	veil->setPosition({ 0, 0 });
	this->addChild(veil);

	auto title = CCLabelBMFont::create("THE HOLLOW", "goldFont.fnt");
	title->setScale(0.55f);
	title->setPosition({ winSize.width / 2, winSize.height - 40 });
	this->addChild(title);

	auto subtitle = CCLabelBMFont::create("Journal of Clues", "bigFont.fnt");
	subtitle->setScale(0.4f);
	subtitle->setPosition({ winSize.width / 2, winSize.height - 75 });
	this->addChild(subtitle);

	buildClueList();

	auto hint = CCLabelBMFont::create("Press ESC to leave", "chatFont.fnt");
	hint->setScale(0.45f);
	hint->setOpacity(180);
	hint->setPosition({ winSize.width / 2, 24 });
	this->addChild(hint);

	if (showJourneyAlert) {
		auto& state = HollowState::get();
		if (state.phase() == Phase::Fresh) {
			state.setPhase(Phase::JournalOpened);
		}
		if (!state.journeyBegunShown()) {
			state.setJourneyBegunShown(true);
			this->runAction(CCSequence::create(
				CCDelayTime::create(0.2f),
				CallFuncExt::create([]() {
					FLAlertLayer::create(
						"The Hollow",
						"The journey has begun.",
						"OK"
					)->show();
				}),
				nullptr
			));
		}
	}

	return true;
}

void HollowJournalLayer::buildClueList() {
	auto winSize = CCDirector::sharedDirector()->getWinSize();
	auto& state = HollowState::get();
	auto const& clues = state.clues();

	float y = winSize.height / 2 + 40;
	if (clues.empty()) {
		auto empty = CCLabelBMFont::create("(No clues yet...)", "chatFont.fnt");
		empty->setScale(0.5f);
		empty->setPosition({ winSize.width / 2, y });
		this->addChild(empty);
		return;
	}

	int index = 0;
	for (auto const& clue : clues) {
		auto label = CCLabelBMFont::create(
			fmt::format("{}. {}", index + 1, clue).c_str(),
			"chatFont.fnt"
		);
		label->setScale(0.42f);
		label->setPosition({ winSize.width / 2, y });
		label->setAlignment(kCCTextAlignmentCenter);
		this->addChild(label);
		y -= 36.f;
		++index;
	}
}

void HollowJournalLayer::keyBackClicked() {
	CCDirector::sharedDirector()->popScene();
}

void HollowJournalLayer::keyDown(enumKeyCodes key, double) {
	if (key == KEY_Escape) {
		keyBackClicked();
	}
}

} // namespace hollow
