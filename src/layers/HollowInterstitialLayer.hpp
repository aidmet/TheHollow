#pragma once

#include <Geode/Geode.hpp>
#include <functional>

using namespace geode::prelude;

namespace hollow {

class HollowInterstitialLayer : public CCLayer {
public:
	static HollowInterstitialLayer* create(std::function<void()> onComplete);
	static CCScene* scene(std::function<void()> onComplete);

	bool init(std::function<void()> onComplete);

private:
	std::function<void()> m_onComplete;
};

} // namespace hollow
