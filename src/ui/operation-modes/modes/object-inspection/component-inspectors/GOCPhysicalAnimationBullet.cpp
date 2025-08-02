#include "GOCPhysicalAnimationBullet.h"
#include <ui/Desktop.h>
#include <ui/resources/editors/ResPhysicalSkeletonEditor.h>
#include <ui/common/editors/Basic.h>

void RenderComponentInspector(hh::pba::GOCPhysicalAnimationBullet& component)
{
	if (ImGui::Button("Edit Physical Skeleton"))
		ResPhysicalSkeletonEditor::Create(Desktop::instance->GetAllocator(), component.physSkelRes, &component);
}
