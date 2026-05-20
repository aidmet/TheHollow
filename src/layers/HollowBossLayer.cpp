#include "HollowBossLayer.hpp"

#include "../HollowState.hpp"
#include "../HollowSpriteScale.hpp"

#include <Geode/utils/general.hpp>

#include <cstdlib>

using namespace geode::prelude;

namespace hollow {

namespace {
	float randRange(float min, float max) {
		return min + (static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX)) * (max - min);
	}
}

HollowBossLayer* HollowBossLayer::create() {
	auto ret = new HollowBossLayer();
	if (ret && ret->init()) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

CCScene* HollowBossLayer::scene() {
	auto scene = CCScene::create();
	scene->addChild(HollowBossLayer::create());
	return scene;
}

bool HollowBossLayer::init() {
	if (!CCLayer::init()) return false;

	this->setKeyboardEnabled(true);
	this->scheduleUpdate();

	m_winSize = CCDirector::sharedDirector()->getWinSize();
	auto const center = m_winSize / 2 + CCPoint { 0, 40.f };

	auto bg = CCLayerColor::create({ 55, 55, 60, 255 });
	bg->setContentSize(m_winSize);
	bg->setAnchorPoint({ 0, 0 });
	bg->setPosition({ 0, 0 });
	this->addChild(bg);

	auto bossSprite = CCSprite::create("HollowBoss.png"_spr);
	if (!bossSprite || bossSprite->isUsingFallback()) {
		bossSprite = CCSprite::createWithSpriteFrameName("bossIcon01_001.png");
		if (!bossSprite) {
			bossSprite = CCSprite::createWithSpriteFrameName("GJ_GameIcon_001.png");
		}
		if (bossSprite) {
			bossSprite->setColor({ 40, 0, 60 });
		}
	}

	if (bossSprite) {
		applyDisplaySize(bossSprite, BOSS_SPRITE_PX);
		m_bossBaseScale = bossSprite->getScale();

		m_bossItem = CCMenuItemSpriteExtra::create(
			bossSprite,
			this,
			menu_selector(HollowBossLayer::onBossTapped)
		);
		m_bossItem->setPosition(center);

		auto bossMenu = CCMenu::create();
		bossMenu->setPosition({ 0, 0 });
		bossMenu->addChild(m_bossItem);
		this->addChild(bossMenu, 5);
	}

	m_hpLabel = CCLabelBMFont::create("VOID: 5", "bigFont.fnt");
	m_hpLabel->setScale(0.5f);
	m_hpLabel->setPosition(m_winSize.width / 2, m_winSize.height - 50);
	this->addChild(m_hpLabel);

	auto prompt = CCLabelBMFont::create("Tap the Hollow", "chatFont.fnt");
	prompt->setScale(0.45f);
	prompt->setPosition(m_winSize.width / 2, 60);
	this->addChild(prompt);

	m_statusLabel = CCLabelBMFont::create("", "chatFont.fnt");
	m_statusLabel->setScale(0.42f);
	m_statusLabel->setPosition(m_winSize.width / 2, 95);
	m_statusLabel->setOpacity(0);
	this->addChild(m_statusLabel);

	auto menu = CCMenu::create();
	menu->setPosition(m_winSize.width / 2, 36);

	auto fleeSpr = ButtonSprite::create("Retreat", "goldFont.fnt", "GJ_button_01.png", .7f);
	auto flee = CCMenuItemSpriteExtra::create(fleeSpr, this, menu_selector(HollowBossLayer::onRetreat));
	menu->addChild(flee);
	this->addChild(menu, 20);

	FLAlertLayer::create(
		"The Hollow",
		"It waits in the main level list.",
		"Fight"
	)->show();

	m_nextAutoDodge = randRange(0.8f, 1.6f);
	if (m_bossItem) {
		m_bossItem->setEnabled(false);
		this->runAction(CCSequence::create(
			CCDelayTime::create(0.55f),
			CallFuncExt::create([this]() {
				if (m_bossItem && !m_fightOver) {
					m_bossItem->setEnabled(true);
				}
			}),
			nullptr
		));
	}

	return true;
}

bool HollowBossLayer::isInvulnerable() const {
	return m_fightOver || m_hp <= 0 || m_invulnerable;
}

void HollowBossLayer::flashStatus(char const* text) {
	if (!m_statusLabel) return;
	m_statusLabel->stopAllActions();
	m_statusLabel->setString(text);
	m_statusLabel->setOpacity(255);
	m_statusLabel->runAction(CCSequence::create(
		CCDelayTime::create(0.9f),
		CCFadeOut::create(0.35f),
		nullptr
	));
}

void HollowBossLayer::dodgeBoss(bool afterHit) {
	if (!m_bossItem || m_fightOver) return;

	m_bossItem->stopAllActions();
	m_bossItem->setEnabled(false);
	m_invulnerable = true;

	float const marginX = m_winSize.width * 0.14f;
	float const marginY = m_winSize.height * 0.18f;
	float const minX = marginX;
	float const maxX = m_winSize.width - marginX;
	float const minY = marginY + 70.f;
	float const maxY = m_winSize.height - marginY - 90.f;

	auto const target = CCPoint {
		randRange(minX, maxX),
		randRange(minY, maxY),
	};

	float const speedBoost = static_cast<float>(5 - m_hp) * 0.025f;
	float const dodgeTime = std::max(0.1f, 0.22f - speedBoost);

	if (!afterHit) {
		flashStatus("It slips away.");
	}

	m_bossItem->runAction(CCSequence::create(
		CCEaseOut::create(CCMoveTo::create(dodgeTime, target), 2.5f),
		CallFuncExt::create([this]() {
			m_invulnerable = false;
			if (m_bossItem && !m_fightOver && m_hp > 0) {
				m_bossItem->setEnabled(true);
			}
		}),
		nullptr
	));
}

void HollowBossLayer::update(float dt) {
	if (m_fightOver || m_hp <= 0 || !m_bossItem) return;

	m_autoDodgeTimer += dt;
	if (m_invulnerable) return;

	if (m_autoDodgeTimer >= m_nextAutoDodge) {
		m_autoDodgeTimer = 0.f;
		m_nextAutoDodge = randRange(1.1f, 2.1f);
		dodgeBoss(false);
	}
}

void HollowBossLayer::onBossTapped(CCObject*) {
	if (isInvulnerable()) return;
	applyHit();
	dodgeBoss(true);
}

void HollowBossLayer::applyHit() {
	if (m_hp <= 0 || m_fightOver) return;

	--m_hp;
	m_hpLabel->setString(fmt::format("VOID: {}", m_hp).c_str());

	if (m_bossItem) {
		auto sprite = m_bossItem->getNormalImage();
		if (sprite) {
			sprite->stopAllActions();
			float const hitScale = m_bossBaseScale * 1.12f;
			sprite->runAction(CCSequence::create(
				CCScaleTo::create(0.05f, hitScale * 1.15f),
				CCScaleTo::create(0.12f, hitScale),
				nullptr
			));
		}
	}

	if (m_hp > 0) {
		flashStatus("It shudders.");
		return;
	}

	m_fightOver = true;
	if (m_bossItem) {
		m_bossItem->setEnabled(false);
	}

	HollowState::get().setPhase(Phase::NormalForever);

	createQuickPopup(
		"The Hollow",
		"You found it. The Hollow closes the game.",
		"Quit",
		nullptr,
		[](FLAlertLayer*, bool) {
			geode::utils::game::exit(true);
		}
	);
}

void HollowBossLayer::onRetreat(CCObject*) {
	keyBackClicked();
}

void HollowBossLayer::keyBackClicked() {
	CCDirector::sharedDirector()->popScene();
}

} // namespace hollow
