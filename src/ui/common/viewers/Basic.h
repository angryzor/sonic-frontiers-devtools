#pragma once

#include <ui/common/inputs/Basic.h>

template<typename T, ImGuiDataType = imgui_datatype<T>::dtype>
static void Viewer(const char* label, const T& obj) {
	if constexpr (std::numeric_limits<T>::is_integer)
		ImGui::Text("%s: %zd", label, obj);
	else
		ImGui::Text("%s: %f", label, obj);
}

template<typename T, int Dim>
static void Viewer(const char* label, const Eigen::Matrix<T, Dim, 1>& vec) {
	if constexpr (Dim == 4)
		IM_ASSERT(ImGui::GetStateStorage()->GetBool(ImGui::GetID("WithW")));

	ImGui::TableNextRow();
	ImGui::TableNextColumn();
	ImGui::Text("%f", vec.x());
	ImGui::TableNextColumn();
	ImGui::Text("%f", vec.y());
	ImGui::TableNextColumn();
	ImGui::Text("%f", vec.z());
	if constexpr (Dim == 4) {
		ImGui::TableNextColumn();
		ImGui::Text("%f", vec.w());
	}
	else {
		if (ImGui::GetStateStorage()->GetBool(ImGui::GetID("WithW")))
			ImGui::TableNextColumn();
	}
	ImGui::TableNextColumn();
	ImGui::Text(label);
}

void Viewer(const char* label, void* const& obj);
void Viewer(const char* label, const hh::fnd::WorldPosition& worldPos);
void Viewer(const char* label, const csl::math::Transform& transform);

bool BeginVectorViewerTable(const char* id, bool withWAxis);
void EndVectorViewerTable();

template<typename T, int Rows, int Cols>
static void Viewer(const char* label, Eigen::Matrix<T, Rows, Cols>& mat) {
	auto cols = reinterpret_cast<Eigen::Matrix<T, Rows, 1>(&)[4]>(mat);

	if (BeginVectorViewerTable(label, true)) {
		for (int i = 0; i < Cols; i++) {
			ImGui::PushID(i);
			Viewer(label, cols[i]);
			ImGui::PopID();
		}

		EndVectorViewerTable();
	}
}
