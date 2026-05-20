#include "HollowModLogoFX.hpp"

#include <Geode/cocos/particle_nodes/CCParticleSystem.h>
#include <Geode/utils/cocos.hpp>

#include <algorithm>
#include <cstdlib>

using namespace geode::prelude;

namespace hollow {

namespace {
	constexpr auto LOGO_FX_ID = "gdplatformmaker.the-hollow/logo-glitch-fx";

	float randRange(float min, float max) {
		return min + (static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX)) * (max - min);
	}

	void addBurstParticles(CCNode* parent) {
		auto particles = CCParticleSystemQuad::create("PlayerExplosion_00.plist", false);
		if (!particles) {
			particles = CCParticleSystemQuad::create("speedEffect_normal.plist", false);
		}
		if (!particles) return;

		particles->setScale(0.55f);
		particles->setStartColor({ 0.2f, 0.f, 0.35f, 0.85f });
		particles->setEndColor({ 0.45f, 0.f, 0.55f, 0.2f });
		particles->setPositionType(kCCPositionTypeGrouped);
		particles->setPosVar({ 12.f, 12.f });
		particles->setSpeed(25.f);
		particles->setSpeedVar(10.f);
		particles->setGravity({ 0.f, 0.f });
		particles->setLife(0.5f);
		particles->setLifeVar(0.2f);
		parent->addChildAtPosition(particles, Anchor::Center, { 0.f, 0.f });
	}

	void addGlitchShards(CCNode* parent, float bounds) {
		for (int i = 0; i < 5; ++i) {
			auto shard = CCSprite::create("GlitchOverlay.png"_spr);
			if (!shard || shard->isUsingFallback()) continue;

			shard->setScale(randRange(0.08f, 0.14f));
			shard->setOpacity(static_cast<GLubyte>(randRange(100.f, 200.f)));
			shard->setRotation(randRange(-30.f, 30.f));
			parent->addChildAtPosition(
				shard,
				Anchor::Center,
				CCPoint { randRange(-bounds, bounds), randRange(-bounds, bounds) }
			);

			shard->runAction(CCRepeatForever::create(CCSequence::create(
				CCFadeTo::create(randRange(0.04f, 0.08f), 50),
				CCFadeTo::create(randRange(0.05f, 0.1f), 200),
				CCDelayTime::create(randRange(0.04f, 0.15f)),
				nullptr
			)));
		}
	}
}

void attachModLogoGlitchFX(CCNode* logoNode) {
	if (!logoNode) return;
	if (logoNode->getChildByID(LOGO_FX_ID)) return;

	auto const size = logoNode->getContentSize();
	float const bounds = std::min(size.width, size.height) * 0.3f;

	auto fx = CCNode::create();
	fx->setID(LOGO_FX_ID);
	fx->setContentSize(size);
	logoNode->addChildAtPosition(fx, Anchor::Center, { 0.f, 0.f });

	addBurstParticles(fx);
	addGlitchShards(fx, bounds);
}

} // namespace hollow
