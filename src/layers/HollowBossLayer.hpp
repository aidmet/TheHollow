#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

namespace hollow {

class HollowBossLayer : public CCLayer {
public:
	static HollowBossLayer* create();
	static CCScene* scene();

	bool init() override;
	void update(float dt) override;
	void keyBackClicked() override;

	void onBossTapped(CCObject*);
	void onRetreat(CCObject*);

private:
	void dodgeBoss(bool afterHit);
	void applyHit();
	void flashStatus(char const* text);
	bool isInvulnerable() const;

	int m_hp = 5;
	bool m_fightOver = false;
	bool m_invulnerable = false;
	float m_bossBaseScale = 1.f;
	float m_autoDodgeTimer = 0.f;
	float m_nextAutoDodge = 1.2f;

	CCSize m_winSize{};
	CCLabelBMFont* m_hpLabel = nullptr;
	CCLabelBMFont* m_statusLabel = nullptr;
	CCMenuItemSpriteExtra* m_bossItem = nullptr;
};

} // namespace hollow
