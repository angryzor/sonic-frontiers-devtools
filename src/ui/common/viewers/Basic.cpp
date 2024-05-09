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

void Viewer(const char* label, void* const& obj) {
	ImGui::Text("%s: %zx", obj);
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
