#include "../HollowSkulls.hpp"

#include <Geode/modify/AchievementsLayer.hpp>

using namespace geode::prelude;
using namespace hollow;

class $modify(HollowAchievementsLayer, AchievementsLayer) {
	void customSetup() {
		AchievementsLayer::customSetup();
		addSkullToLayer(this, 8, { 0.5f, 0.08f });
	}
};
