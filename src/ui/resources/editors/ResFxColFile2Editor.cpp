#include "ResFxColFile2Editor.h"
#include <ui/common/editors/FxCol.h>

using namespace app::gfx;

ResFxColFile2Editor::ResFxColFile2Editor(csl::fnd::IAllocator* allocator, app::gfx::ResFxColFile2* resource) : StandaloneWindow{ allocator }, resource{ resource }
{
	char namebuf[500];
	snprintf(namebuf, sizeof(namebuf), "%s - %s @ 0x%zx (file mapped @ 0x%zx)", resource->GetName(), resource->GetClass().pName, (size_t)&resource, (size_t)resource->fxColData);
	SetTitle(namebuf);
}

ResFxColFile2Editor* ResFxColFile2Editor::Create(csl::fnd::IAllocator* allocator, app::gfx::ResFxColFile2* resource) {
	return new (allocator) ResFxColFile2Editor(allocator, resource);
}

void ResFxColFile2Editor::RenderContents()
{
	auto* fxColData = resource->fxColData;

	for (unsigned int i = 0; i < fxColData->shapeCount; i++) {
		ImGui::PushID(static_cast<int>(i));
		Editor("Collision Shape", fxColData->shapes[i]);
		ImGui::PopID();
	}
}
