#include "../HollowSkulls.hpp"

#include <Geode/modify/SecretRewardsLayer.hpp>

using namespace geode::prelude;
using namespace hollow;

class $modify(HollowSecretRewardsLayer, SecretRewardsLayer) {
	bool init(bool fromShop) {
		if (!SecretRewardsLayer::init(fromShop)) return false;
		addSkullToLayer(this, 6, { 0.15f, 0.85f });
		return true;
	}
};
