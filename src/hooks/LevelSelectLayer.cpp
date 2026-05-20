#include "../HollowSkulls.hpp"
#include "../HollowState.hpp"

#include <Geode/modify/LevelSelectLayer.hpp>

using namespace geode::prelude;
using namespace hollow;

class $modify(HollowLevelSelectLayer, LevelSelectLayer) {
	bool init(int page) {
		if (!LevelSelectLayer::init(page)) return false;

		addSkullToLayer(this, 1, { 0.92f, 0.15f });
		return true;
	}
};
