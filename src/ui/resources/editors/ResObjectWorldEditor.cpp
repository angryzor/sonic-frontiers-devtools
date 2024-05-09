#include "ResObjectWorldEditor.h"
#include <ui/common/editors/ObjectData.h>

using namespace hh::game;

ResObjectWorldEditor::ResObjectWorldEditor(csl::fnd::IAllocator* allocator, hh::game::ResObjectWorld* resource) : StandaloneWindow{ allocator }, resource{ resource }
{
	char namebuf[500];
	snprintf(namebuf, sizeof(namebuf), "%s - %s @ 0x%zx (file mapped @ 0x%zx)", resource->GetName(), resource->GetClass().pName, (size_t)&resource, (size_t)resource->binaryData);
	SetTitle(namebuf);
}

ResObjectWorldEditor* ResObjectWorldEditor::Create(csl::fnd::IAllocator* allocator, hh::game::ResObjectWorld* resource) {
	return new (allocator) ResObjectWorldEditor(allocator, resource);
}

void ResObjectWorldEditor::RenderContents()
{
	for (auto* obj : resource->GetObjects()) {
		if (ImGui::TreeNode(obj, "%s", obj->name.c_str())) {
			Editor("Object data", *obj);
			ImGui::TreePop();
		}
	}
}
