#pragma once
#include <ui/common/StandaloneWindow.h>

class NeedleFxSceneDataTesterV2 : public StandaloneWindow {
	hh::fnd::Reference<hh::fnd::ResReflectionT<hh::needle::NeedleFxSceneData>> resource{};
	bool setConfig{ false };
	bool setParam{ false };
	bool setTimeParam{ false };
	bool setWeatherParam{ false };
	int collectionType{ 0 };
	int itemId{ 0 };
	int timeItemId{ 0 };
	int weatherItemId{ 0 };
	bool timeUseEventCameraControl{ false };
	bool weatherUseEventCameraControl{ false };
public:
	NeedleFxSceneDataTesterV2(csl::fnd::IAllocator* allocator);
	~NeedleFxSceneDataTesterV2();
	virtual void PreRender() override;
	virtual void RenderContents() override;
};
