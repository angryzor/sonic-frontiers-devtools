#include "GOCVisual.h"
#include <ui/common/editors/Basic.h>
#include <ui/common/viewers/Basic.h>

void RenderComponentInspector(hh::gfx::GOCVisual& component) {
    bool isVisible{ component.IsVisible() };

    if (Editor("Visible", isVisible))
        component.SetVisible(isVisible);
    Editor("unk102", component.unk102);
}

void RenderComponentInspector(hh::gfx::GOCVisualTransformed& component) {
	RenderComponentInspector(static_cast<hh::gfx::GOCVisual&>(component));

	auto transform = component.localTransform;

	ImGui::SeparatorText("Main local transform (editable form)");
	if (Editor("Local Transform Editable", transform))
		component.SetLocalTransform(transform);

	ImGui::SeparatorText("Main local transform (internal representation)");
	Viewer("Local Transform ", transform);

	auto worldMatrix = component.worldMatrix;
	ImGui::SeparatorText("World matrix");
	if (Editor("World matrix", worldMatrix))
		component.SetWorldMatrix(worldMatrix);

	if (component.frame2) {
		ImGui::SeparatorText("Calculated HFrame local transform");
		Viewer("CalculatedHFrameTransformLocal", component.frame2->localTransform);

		ImGui::SeparatorText("Calculated HFrame full transform");
		Viewer("CalculatedHFrameTransformFull", component.frame2->fullTransform);
	}

	if (component.frame1) {
		ImGui::SeparatorText("Offset HFrame local transform");
		Viewer("OffsetHFrameTransformLocal", component.frame1->localTransform);

		ImGui::SeparatorText("Offset HFrame full transform");
		Viewer("OffsetHFrameTransformFull", component.frame1->fullTransform);
	}

	// custom AABB
}

void RenderComponentInspector(hh::gfx::GOCVisualModel& component) {
	RenderComponentInspector(static_cast<hh::gfx::GOCVisualTransformed&>(component));

	if (auto& model = component.model)
		Viewer("Model resource", model->GetName());
	if (auto& skeleton = component.skeleton)
		Viewer("Model resource", skeleton->GetName());
}
