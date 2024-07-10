#pragma once
#include <ui/common/StandaloneWindow.h>

class ResFxColFile2Editor : public StandaloneWindow {
	hh::fnd::Reference<app::gfx::ResFxColFile2> resource;
public:
	ResFxColFile2Editor(csl::fnd::IAllocator* allocator, app::gfx::ResFxColFile2* resource);
	virtual void RenderContents();
	static ResFxColFile2Editor* Create(csl::fnd::IAllocator* allocator, app::gfx::ResFxColFile2* resource);
};
