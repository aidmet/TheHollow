#include "../HollowSkulls.hpp"

#include <Geode/modify/MoreOptionsLayer.hpp>

using namespace geode::prelude;
using namespace hollow;

class $modify(HollowMoreOptionsLayer, MoreOptionsLayer) {
	bool init() {
		if (!MoreOptionsLayer::init()) return false;
		addSkullToLayer(this, 9, { 0.72f, 0.12f });
		return true;
	}
};
