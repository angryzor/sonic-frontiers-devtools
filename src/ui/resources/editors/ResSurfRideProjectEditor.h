#pragma once
#include <ui/common/StandaloneWindow.h>

class ResSurfRideProjectEditor : public StandaloneWindow {
	hh::ui::ResSurfRideProject* resource;
public:
	ResSurfRideProjectEditor(csl::fnd::IAllocator* allocator, hh::ui::ResSurfRideProject* resource);
	static ResSurfRideProjectEditor* Create(csl::fnd::IAllocator* allocator, hh::ui::ResSurfRideProject* resource);
	virtual void RenderContents();
};