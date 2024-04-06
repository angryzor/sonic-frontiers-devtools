#pragma once

#include "StandaloneWindow.h"

class NeedleFxSceneDataTester : public StandaloneWindow {
	hh::fnd::Reference<hh::fnd::ResReflection<app::rfl::NeedleFxSceneData>> resource{};
	hh::fnd::Reference<app::gfx::FxParamManager> fxParamManager{};
	hh::fnd::Reference<app::gfx::FxColManager> fxColManager{};
	bool setParam{ false };
	bool setConfig{ false };
	int collectionType{ 0 };
	int itemId{ 0 };
	int timeItemId{ 0 };
	int weatherItemId{ 0 };
public:
	NeedleFxSceneDataTester(csl::fnd::IAllocator* allocator);
	~NeedleFxSceneDataTester();
	virtual void PreRender() override;
	virtual void RenderContents() override;
};
