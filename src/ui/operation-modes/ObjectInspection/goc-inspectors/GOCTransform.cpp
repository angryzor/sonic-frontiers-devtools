#include "GOCTransform.h"

void RenderComponentInspector(GOCTransform& component) {
	auto transform = component.transform;

	ImGui::SeparatorText("Main local transform (editable form)");
	if (Editor("GOCTransformTransformEditable", transform))
		component.SetLocalTransform(transform);

	ImGui::SeparatorText("Main local transform (internal representation)");
	Viewer("GOCTransformTransform", component.transform);

	ImGui::SeparatorText("HFrame local transform");
	Viewer("HFrameTransformLocal", component.frame->localTransform);

	ImGui::SeparatorText("HFrame full transform");
	Viewer("HFrameTransformFull", component.frame->fullTransform);
}
