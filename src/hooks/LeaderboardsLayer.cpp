#include "../HollowSkulls.hpp"

#include <Geode/modify/LeaderboardsLayer.hpp>

using namespace geode::prelude;
using namespace hollow;

class $modify(HollowLeaderboardsLayer, LeaderboardsLayer) {
	bool init(LeaderboardType type, LeaderboardStat stat) {
		if (!LeaderboardsLayer::init(type, stat)) return false;
		addSkullToLayer(this, 4, { 0.1f, 0.55f });
		return true;
	}
};
