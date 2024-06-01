#pragma once
#include <ui/common/StandaloneWindow.h>
#include <utilities/RflDiff.h>

class RflComparer : public StandaloneWindow {
	hh::fnd::Reference<hh::fnd::ResReflection> resource1{};
	hh::fnd::Reference<hh::fnd::ResReflection> resource2{};
	const hh::fnd::RflClass* rflClass{};
	RflDiffResult diffResult{ GetAllocator() };
public:
	RflComparer(csl::fnd::IAllocator* allocator);
	//virtual void PreRender() override;
	virtual void RenderContents() override;
};
