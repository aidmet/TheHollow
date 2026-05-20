#include "../HollowSkulls.hpp"

#include <Geode/modify/OptionsLayer.hpp>

using namespace geode::prelude;
using namespace hollow;

class $modify(HollowOptionsLayer, OptionsLayer) {
	void customSetup() {
		OptionsLayer::customSetup();
		addSkullToLayer(this, 5, { 0.9f, 0.45f });
	}
};
