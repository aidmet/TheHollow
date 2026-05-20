#include "../HollowSkulls.hpp"
#include "../HollowSpriteScale.hpp"
#include "../HollowState.hpp"
#include "../layers/HollowBossLayer.hpp"
#include "../layers/HollowInterstitialLayer.hpp"
#include "../layers/HollowJournalLayer.hpp"

#include <Geode/modify/MenuLayer.hpp>
#include <Geode/binding/ButtonSprite.hpp>
#include <Geode/binding/CCMenuItemSpriteExtra.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/utils/cocos.hpp>
#include <Geode/utils/general.hpp>

using namespace geode::prelude;
using namespace hollow;

class $modify(HollowMenuLayer, MenuLayer) {
	struct Fields {
		CCNode* m_blackOverlay = nullptr;
	};

	bool init() {
		if (!MenuLayer::init()) return false;

		auto& state = HollowState::get();

		if (state.phase() == Phase::NormalForever) {
			return true;
		}

		if (state.isARGActive()) {
			addJournalButton();
		}

		if (state.phase() == Phase::BlackMenu && !state.blackMenuDialogueDone() && !state.blackMenuSequenceShown()) {
			state.setBlackMenuSequenceShown(true);
			setupBlackMenuOverlay();
			// Popups during init crash (ButtonSprite/FLAlertLayer not ready). Defer until load finishes.
			this->runAction(CCSequence::create(
				CCDelayTime::create(0.6f),
				CallFuncExt::create([this]() {
					if (HollowState::get().phase() != Phase::BlackMenu) return;
					if (HollowState::get().blackMenuDialogueDone()) return;
					showBlackMenuDialogue();
				}),
				nullptr
			));
		}

		return true;
	}

	void addJournalButton() {
		auto sprite = CCSprite::create("HollowJournalBtn.png"_spr);
		if (!sprite || sprite->isUsingFallback()) {
			sprite = CCSprite::createWithSpriteFrameName("GJ_likeBtn_001.png");
			sprite->setColor({ 80, 40, 120 });
		}
		applyDisplaySize(sprite, JOURNAL_BUTTON_PX);

		auto btn = CCMenuItemSpriteExtra::create(
			sprite,
			this,
			menu_selector(HollowMenuLayer::onJournal)
		);
		btn->setID("hollow-journal-button"_spr);

		if (auto menu = this->getChildByID("bottom-menu")) {
			menu->addChild(btn);
			menu->updateLayout();
		} else if (auto side = this->getChildByID("side-menu")) {
			btn->setPosition({ 30.f, 30.f });
			side->addChild(btn);
		}

		addSkullToLayer(this, 0, { 0.08f, 0.92f });
	}

	void onJournal(CCObject*) {
		CCDirector::sharedDirector()->pushScene(HollowJournalLayer::scene(true));
	}

	void onPlay(cocos2d::CCObject* sender) {
		auto& state = HollowState::get();

		if (state.phase() == Phase::NormalForever) {
			MenuLayer::onPlay(sender);
			return;
		}

		if (state.phase() == Phase::ShowdownReady) {
			CCDirector::sharedDirector()->pushScene(HollowBossLayer::scene());
			return;
		}

		if (state.phase() == Phase::JournalOpened && !state.interstitialShown()) {
			state.setInterstitialShown(true);
			state.setPhase(Phase::SkullHunt);

			CCDirector::sharedDirector()->pushScene(HollowInterstitialLayer::scene([this, sender]() {
				MenuLayer::onPlay(sender);
			}));
			return;
		}

		MenuLayer::onPlay(sender);
	}

	void onOptions(cocos2d::CCObject* sender) {
		auto& state = HollowState::get();

		if (state.phase() == Phase::JournalClue2 && state.settingsUnlocked()) {
			state.setPhase(Phase::BlackMenu);
			geode::utils::game::restart(true);
			return;
		}

		MenuLayer::onOptions(sender);
	}

	void setupBlackMenuOverlay() {
		auto winSize = CCDirector::sharedDirector()->getWinSize();
		m_fields->m_blackOverlay = CCLayerColor::create({ 0, 0, 0, 255 });
		m_fields->m_blackOverlay->setContentSize(winSize);
		m_fields->m_blackOverlay->setAnchorPoint({ 0, 0 });
		m_fields->m_blackOverlay->setPosition({ 0, 0 });
		this->addChild(m_fields->m_blackOverlay, 1000);
	}

	void showBlackMenuDialogue() {
		createQuickPopup(
			"The Hollow",
			"You found it.",
			"Listen",
			nullptr,
			[this](FLAlertLayer*, bool) {
				createQuickPopup(
					"The Hollow",
					"Meet it in the main level list.",
					"Return",
					nullptr,
					[this](FLAlertLayer*, bool) {
						auto& s = HollowState::get();
						s.setBlackMenuDialogueDone(true);
						s.setPhase(Phase::ShowdownReady);
						if (m_fields->m_blackOverlay) {
							m_fields->m_blackOverlay->removeFromParent();
							m_fields->m_blackOverlay = nullptr;
						}
					}
				);
			}
		);
	}
};
