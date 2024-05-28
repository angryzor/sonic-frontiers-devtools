#include "GOCTransform.h"
#include <ui/common/editors/Basic.h>
#include <ui/common/viewers/Basic.h>


using namespace hh::game;

void RenderComponentInspector(GOCTransform& component) {
	auto transform = component.GetTransform();

	ImGui::SeparatorText("Main local transform (editable form)");
	if (Editor("GOCTransformTransformEditable", transform))
		component.SetLocalTransform(transform);

	ImGui::SeparatorText("Main local transform (internal representation)");
	Viewer("GOCTransformTransform", component.GetTransform());

	ImGui::SeparatorText("HFrame local transform");
	Viewer("HFrameTransformLocal", component.GetFrame().localTransform);

	ImGui::SeparatorText("HFrame full transform");
	Viewer("HFrameTransformFull", component.GetFrame().fullTransform);
}
