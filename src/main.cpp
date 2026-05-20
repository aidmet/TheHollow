#include <Geode/Geode.hpp>
#include <Geode/loader/SettingV3.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <Geode/ui/Notification.hpp>

#include "HollowModLogoFX.hpp"
#include "HollowState.hpp"

using namespace geode::prelude;
using namespace hollow;

$on_mod(Loaded) {
	ModLogoUIEvent().listen([](CCNode* logoNode, std::string_view modId, std::optional<Mod*>) {
		if (!logoNode || modId != Mod::get()->getID()) {
			return ListenerResult::Propagate;
		}
		attachModLogoGlitchFX(logoNode);
		return ListenerResult::Propagate;
	}).leak();

	ButtonSettingPressedEventV3(Mod::get(), "reset-arg").listen([](std::string_view buttonKey) {
		if (buttonKey != "reset") return ListenerResult::Propagate;
		HollowState::get().resetARG();
		Notification::create("The Hollow ARG progress reset", NotificationIcon::Info)->show();
		return ListenerResult::Propagate;
	}).leak();
}
