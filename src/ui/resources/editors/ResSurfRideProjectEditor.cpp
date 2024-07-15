#include "ResSurfRideProjectEditor.h"
#include <ui/common/editors/Reflection.h>

ResSurfRideProjectEditor::ResSurfRideProjectEditor(csl::fnd::IAllocator* allocator, hh::ui::ResSurfRideProject* resource) : StandaloneWindow(allocator)
{
	SetTitle("SurfRide Project");
}

ResSurfRideProjectEditor* ResSurfRideProjectEditor::Create(csl::fnd::IAllocator* allocator, hh::ui::ResSurfRideProject* resource)
{
	return new (allocator) ResSurfRideProjectEditor(allocator, resource);
}

void ResSurfRideProjectEditor::RenderContents()
{
}

