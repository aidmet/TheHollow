#include "../HollowSkulls.hpp"

#include <Geode/modify/GJGarageLayer.hpp>

using namespace geode::prelude;
using namespace hollow;

class $modify(HollowGarageLayer, GJGarageLayer) {
	bool init() {
		if (!GJGarageLayer::init()) return false;
		addSkullToLayer(this, 3, { 0.88f, 0.75f });
		return true;
	}
};
