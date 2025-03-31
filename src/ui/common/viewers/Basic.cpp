#include "Basic.h"
#include <ui/Desktop.h>

bool BeginVectorViewerTable (const char* id, bool withWAxis) {
	bool tableResult = ImGui::BeginTable(id, withWAxis ? 5 : 4);

	if (tableResult) {
		ImGui::GetStateStorage()->SetBool(ImGui::GetID("WithW"), withWAxis);

		ImGui::TableSetupColumn("X");
		ImGui::TableSetupColumn("Y");
		ImGui::TableSetupColumn("Z");
		if (withWAxis)
			ImGui::TableSetupColumn("W");
		ImGui::TableSetupColumn("");

		ImGui::TableHeadersRow();
	}

	return tableResult;
}

void EndVectorViewerTable() {
	ImGui::EndTable();
}

void Viewer(const char* label, bool obj)
{
	ImGui::Text("%s: %s", label, obj ? "true" : "false");
}

void Viewer(const char* label, const char* str)
{
	ImGui::Text("%s: %s", label, str);
}

void Viewer(const char* label, const csl::ut::String& str)
{
	// FIXME: Make c_str() const
	ImGui::Text("%s: %s", label, const_cast<csl::ut::String&>(str).c_str());
}

void Viewer(const char* label, const ucsl::objectids::ObjectIdV1& id)
{
	ImGui::Text("%s: %08x", label, id.id);
}

void Viewer(const char* label, const ucsl::objectids::ObjectIdV2& id)
{
	ImGui::Text("%s: %016zx%016zx", label, id.groupId, id.objectId);
}

void Viewer(const char* label, void* const& obj) {
	ImGui::Text("%s: %zx", label, obj);
}

void Viewer(const char* label, const Eigen::Quaternionf& quat)
{
	IM_ASSERT(ImGui::GetStateStorage()->GetBool(ImGui::GetID("WithW")));

	ImGui::TableNextRow();
	ImGui::TableNextColumn();
	ImGui::Text("%f", quat.x());
	ImGui::TableNextColumn();
	ImGui::Text("%f", quat.y());
	ImGui::TableNextColumn();
	ImGui::Text("%f", quat.z());
	ImGui::TableNextColumn();
	ImGui::Text("%f", quat.w());
	ImGui::TableNextColumn();
	ImGui::Text(label);
}

void Viewer(const char* label, const hh::fnd::WorldPosition& worldPos) {
	if (BeginVectorViewerTable(label, true)) {
		Viewer("Position", worldPos.m_Position);
		Viewer("Rotation", worldPos.m_Rotation);
		EndVectorViewerTable();
	}
}

void Viewer(const char* label, const csl::math::Transform& transform) {
	if (BeginVectorViewerTable(label, true)) {
		Viewer("Position", transform.position);
		Viewer("Rotation", transform.rotation);
		Viewer("Scale", transform.scale);
		EndVectorViewerTable();
	}
}

void Viewer(const char* label, hh::game::GameObject* const& gameObject)
{
	ImGui::PushID(label);
	if (gameObject == nullptr) ImGui::BeginDisabled();
	if (ImGui::Button("Select"))
		Desktop::instance->Dispatch(FocusGameObjectAction{ gameObject });
	if (gameObject == nullptr) ImGui::EndDisabled();
	ImGui::PopID();
}

void Viewer(const char* label, const hh::fnd::HFrame& frame)
{
	if (ImGui::TreeNode(label)) {
		Viewer("Transform position", (frame.flags & hh::fnd::HFrame::Flag::TRANSFORM_POSITION) != 0);
		Viewer("Transform rotation", (frame.flags & hh::fnd::HFrame::Flag::TRANSFORM_ROTATION) != 0);
		ImGui::SeparatorText("HFrame local transform");
		Viewer("HFrameTransformLocal", frame.localTransform);
		ImGui::SeparatorText("HFrame full transform");
		Viewer("HFrameTransformFull", frame.fullTransform);
		ImGui::TreePop();
	}
}
