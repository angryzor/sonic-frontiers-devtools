#include "GOCCollider.h"
#include <ui/common/editors/Basic.h>
#include <ui/common/viewers/Basic.h>

void RenderComponentInspector(hh::physics::GOCCollider& component)
{
	ImGui::SeparatorText("Collider information");
	ImGui::Text("Shape type: %d", component.shapeType);
	ImGui::Text("Filter category: %d", component.filterCategory);

#ifndef DEVTOOLS_TARGET_SDK_wars
	ImGui::SeparatorText("Local world position (editable form)");
	ImGui::BeginGroup();
	Editor("Local world position", component.localWorldPosition);
	Editor("Scale", component.scale);
	ImGui::EndGroup();

	if (ImGui::IsItemEdited()) {
		component.frame->MarkUpdate();
		component.flags.set(hh::physics::GOCCollider::Flag::FRAME_DIRTY);
	}
#endif

#ifndef DEVTOOLS_TARGET_SDK_wars
	ImGui::SeparatorText("Local world position (internal representation)");
	Viewer("Local world position", component.localWorldPosition);

	ImGui::SeparatorText("Transformed world position");
	Viewer("Transformed world position", component.transformedWorldPosition);
#endif

	if (component.frame) {
		ImGui::SeparatorText("HFrame 1 local transform");
		Viewer("HFrameTransformLocal", component.frame->localTransform);

		ImGui::SeparatorText("HFrame 1 full transform");
		Viewer("HFrameTransformFull", component.frame->fullTransform);
	}

	if (component.frame2) {
		ImGui::SeparatorText("HFrame 2 local transform");
		Viewer("HFrameTransformLocal", component.frame2->localTransform);

		ImGui::SeparatorText("HFrame 2 full transform");
		Viewer("HFrameTransformFull", component.frame2->fullTransform);
	}
}

void RenderComponentInspector(hh::physics::GOCSphereCollider& component)
{
	ImGui::SeparatorText("Parameters");
	ImGui::Text("Radius: %f", component.radius);
	RenderComponentInspector(static_cast<hh::physics::GOCCollider&>(component));
}

void RenderComponentInspector(hh::physics::GOCBoxCollider& component)
{
	ImGui::SeparatorText("Parameters");
	if (BeginVectorViewerTable("Dimensions", false))
	{
		Viewer("Dimensions", component.dimensions);
		EndVectorViewerTable();
	}
	RenderComponentInspector(static_cast<hh::physics::GOCCollider&>(component));
}

void RenderComponentInspector(hh::physics::GOCCapsuleCollider& component)
{
	ImGui::SeparatorText("Parameters");
	ImGui::Text("Radius: %f", component.radius);
	ImGui::Text("Height: %f", component.halfHeight);
	RenderComponentInspector(static_cast<hh::physics::GOCCollider&>(component));
}

void RenderComponentInspector(hh::physics::GOCCylinderCollider& component)
{
	ImGui::SeparatorText("Parameters");
	ImGui::Text("Radius: %f", component.radius);
	ImGui::Text("Height: %f", component.halfHeight);
	RenderComponentInspector(static_cast<hh::physics::GOCCollider&>(component));
}
