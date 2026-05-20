#include "../HollowSkulls.hpp"

#include <Geode/modify/LevelBrowserLayer.hpp>

using namespace geode::prelude;
using namespace hollow;

class $modify(HollowLevelBrowserLayer, LevelBrowserLayer) {
	bool init(GJSearchObject* search) {
		if (!LevelBrowserLayer::init(search)) return false;
		addSkullToLayer(this, 7, { 0.85f, 0.88f });
		return true;
	}
};
