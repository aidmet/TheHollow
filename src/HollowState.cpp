#include "HollowState.hpp"

#include <Geode/utils/cocos.hpp>

using namespace geode::prelude;

namespace hollow {

HollowState& HollowState::get() {
	static HollowState instance;
	static bool loaded = false;
	if (!loaded) {
		instance.load();
		loaded = true;
	}
	return instance;
}

bool HollowState::isARGActive() const {
	return m_phase != Phase::NormalForever;
}

Phase HollowState::phase() const {
	return m_phase;
}

void HollowState::setPhase(Phase phase) {
	m_phase = phase;
	save();
}

bool HollowState::interstitialShown() const {
	return m_interstitialShown;
}

void HollowState::setInterstitialShown(bool value) {
	m_interstitialShown = value;
	save();
}

bool HollowState::settingsUnlocked() const {
	return m_settingsUnlocked;
}

void HollowState::setSettingsUnlocked(bool value) {
	m_settingsUnlocked = value;
	save();
}

bool HollowState::blackMenuDialogueDone() const {
	return m_blackMenuDialogueDone;
}

void HollowState::setBlackMenuDialogueDone(bool value) {
	m_blackMenuDialogueDone = value;
	save();
}

bool HollowState::blackMenuSequenceShown() const {
	return m_blackMenuSequenceShown;
}

void HollowState::setBlackMenuSequenceShown(bool value) {
	m_blackMenuSequenceShown = value;
}

bool HollowState::journeyBegunShown() const {
	return m_journeyBegunShown;
}

void HollowState::setJourneyBegunShown(bool value) {
	m_journeyBegunShown = value;
	save();
}

bool HollowState::isSkullFound(int index) const {
	if (index < 0 || index >= SKULL_COUNT) return true;
	return m_skulls[static_cast<size_t>(index)];
}

void HollowState::markSkullFound(int index) {
	if (index < 0 || index >= SKULL_COUNT) return;
	m_skulls[static_cast<size_t>(index)] = true;
	save();
}

int HollowState::skullsFound() const {
	int count = 0;
	for (bool found : m_skulls) {
		if (found) ++count;
	}
	return count;
}

std::vector<std::string> const& HollowState::clues() const {
	return m_clues;
}

void HollowState::addClue(std::string const& clue) {
	if (hasClue(clue)) return;
	m_clues.push_back(clue);
	save();
}

bool HollowState::hasClue(std::string const& clue) const {
	for (auto const& existing : m_clues) {
		if (existing == clue) return true;
	}
	return false;
}

void HollowState::onSkullCollected() {
	if (skullsFound() >= SKULL_COUNT && m_phase == Phase::SkullHunt) {
		setPhase(Phase::JournalClue2);
		addClue(CLUE_SETTINGS);
	}
}

void HollowState::resetARG() {
	m_phase = Phase::Fresh;
	m_interstitialShown = false;
	m_settingsUnlocked = false;
	m_blackMenuDialogueDone = false;
	m_blackMenuSequenceShown = false;
	m_journeyBegunShown = false;
	m_skulls.fill(false);
	m_clues.clear();
	save();
}

void HollowState::save() {
	auto mod = Mod::get();
	mod->setSavedValue("hollow-phase", static_cast<int>(m_phase));
	mod->setSavedValue("hollow-interstitial", m_interstitialShown);
	mod->setSavedValue("hollow-settings-unlocked", m_settingsUnlocked);
	mod->setSavedValue("hollow-black-dialogue", m_blackMenuDialogueDone);
	mod->setSavedValue("hollow-journey-begun", m_journeyBegunShown);
	mod->setSavedValue("hollow-clues", m_clues);

	int mask = 0;
	for (int i = 0; i < SKULL_COUNT; ++i) {
		if (m_skulls[static_cast<size_t>(i)]) {
			mask |= 1 << i;
		}
	}
	mod->setSavedValue("hollow-skulls", mask);
}

void HollowState::load() {
	auto mod = Mod::get();
	m_phase = static_cast<Phase>(mod->getSavedValue<int>("hollow-phase", 0));
	m_interstitialShown = mod->getSavedValue<bool>("hollow-interstitial", false);
	m_settingsUnlocked = mod->getSavedValue<bool>("hollow-settings-unlocked", false);
	m_blackMenuDialogueDone = mod->getSavedValue<bool>("hollow-black-dialogue", false);
	m_journeyBegunShown = mod->getSavedValue<bool>("hollow-journey-begun", false);
	m_clues = mod->getSavedValue<std::vector<std::string>>("hollow-clues", {});

	if (!m_journeyBegunShown && m_phase != Phase::Fresh) {
		m_journeyBegunShown = true;
	}

	int mask = mod->getSavedValue<int>("hollow-skulls", 0);
	m_skulls.fill(false);
	for (int i = 0; i < SKULL_COUNT; ++i) {
		m_skulls[static_cast<size_t>(i)] = (mask & (1 << i)) != 0;
	}
}

} // namespace hollow
