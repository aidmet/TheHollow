#pragma once

#include <Geode/Geode.hpp>
#include <array>
#include <string>
#include <vector>

namespace hollow {

enum class Phase : int {
	Fresh = 0,
	JournalOpened,
	SkullHunt,
	JournalClue2,
	BlackMenu,
	ShowdownReady,
	NormalForever,
};

inline constexpr int SKULL_COUNT = 10;
inline constexpr char const* CLUE_SETTINGS = "someS errorsT triggerT terribleE instabilitiesN negatingI gameG stability.S";

class HollowState {
public:
	static HollowState& get();

	bool isARGActive() const;
	Phase phase() const;
	void setPhase(Phase phase);

	bool interstitialShown() const;
	void setInterstitialShown(bool value);

	bool settingsUnlocked() const;
	void setSettingsUnlocked(bool value);

	bool blackMenuDialogueDone() const;
	void setBlackMenuDialogueDone(bool value);

	bool blackMenuSequenceShown() const;
	void setBlackMenuSequenceShown(bool value);

	bool journeyBegunShown() const;
	void setJourneyBegunShown(bool value);

	bool isSkullFound(int index) const;
	void markSkullFound(int index);
	int skullsFound() const;

	std::vector<std::string> const& clues() const;
	void addClue(std::string const& clue);
	bool hasClue(std::string const& clue) const;

	void onSkullCollected();
	void resetARG();

	void save();
	void load();

private:
	HollowState() = default;

	Phase m_phase = Phase::Fresh;
	bool m_interstitialShown = false;
	bool m_settingsUnlocked = false;
	bool m_blackMenuDialogueDone = false;
	bool m_blackMenuSequenceShown = false;
	bool m_journeyBegunShown = false;
	std::array<bool, SKULL_COUNT> m_skulls{};
	std::vector<std::string> m_clues;
};

} // namespace hollow
