#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

namespace hollow {

class HollowJournalLayer : public CCLayer {
public:
	static HollowJournalLayer* create(bool showJourneyAlert);
	static CCScene* scene(bool showJourneyAlert);

	bool init(bool showJourneyAlert);
	void keyBackClicked() override;
	void keyDown(enumKeyCodes key, double p1) override;

private:
	void buildClueList();
};

} // namespace hollow
