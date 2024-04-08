#pragma once
#include <ui/common/StandaloneWindow.h>

class ResObjectWorldEditor : public StandaloneWindow {
	hh::fnd::Reference<hh::game::ResObjectWorld> resource;
public:
	ResObjectWorldEditor(csl::fnd::IAllocator* allocator, hh::game::ResObjectWorld* resource);
	virtual void RenderContents();
	static ResObjectWorldEditor* Create(csl::fnd::IAllocator* allocator, hh::game::ResObjectWorld* resource);
};
