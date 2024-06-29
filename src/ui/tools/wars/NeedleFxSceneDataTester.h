#pragma once
#include <ui/common/StandaloneWindow.h>

class NeedleFxSceneDataTester : public StandaloneWindow {
	hh::fnd::Reference<hh::fnd::ResReflectionT<hh::NeedleFxSceneData>> resource{};
	hh::fnd::Reference<app::gfx::FxParamManager> fxParamManager{};
	hh::fnd::Reference<app::gfx::FxColManager> fxColManager{};
	bool setParam{ false };
	int collectionType{ 0 };
	int itemId{ 0 };
public:
	NeedleFxSceneDataTester(csl::fnd::IAllocator* allocator);
	~NeedleFxSceneDataTester();
	virtual void PreRender() override;
	virtual void RenderContents() override;
};
