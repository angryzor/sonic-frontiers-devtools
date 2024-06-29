#pragma once
#include <ui/common/StandaloneWindow.h>

class NeedleFxSceneDataTesterV2 : public StandaloneWindow {
	hh::fnd::Reference<hh::fnd::ResReflectionT<hh::NeedleFxSceneData>> resource{};
	bool setParam{ false };
	int collectionType{ 0 };
	int itemId{ 0 };
public:
	NeedleFxSceneDataTesterV2(csl::fnd::IAllocator* allocator);
	virtual void PreRender() override;
	virtual void RenderContents() override;
};
