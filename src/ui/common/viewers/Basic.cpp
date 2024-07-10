#include "Basic.h"

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

void Viewer(const char* label, const hh::game::ObjectId& id)
{
#ifdef DEVTOOLS_TARGET_SDK_wars
	ImGui::Text("%s: %08x", label, id.id);
#else
	ImGui::Text("%s: %016zx%016zx", label, id.groupId, id.objectId);
#endif
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

void Viewer(const char* label, const csl::ut::Color8& color)
{
	ImGui::Text("%s: R: %d, G: %d, B: %d, A: %d", color.r, color.g, color.b, color.a);
}

void Viewer(const char* label, const csl::ut::Color<float>& color)
{
	ImGui::Text("%s: R: %f, G: %f, B: %f, A: %f", color.r, color.g, color.b, color.a);
}
