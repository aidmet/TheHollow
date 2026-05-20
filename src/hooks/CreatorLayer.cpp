#include "../HollowSkulls.hpp"

#include <Geode/modify/CreatorLayer.hpp>

using namespace geode::prelude;
using namespace hollow;

class $modify(HollowCreatorLayer, CreatorLayer) {
	bool init() {
		if (!CreatorLayer::init()) return false;
		addSkullToLayer(this, 2, { 0.12f, 0.2f });
		return true;
	}
};
