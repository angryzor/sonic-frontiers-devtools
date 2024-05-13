#pragma once

#include <ui/common/inputs/Basic.h>

template<typename T, std::enable_if_t<std::is_fundamental_v<T>, bool> = true>
static void Viewer(const char* label, const T& obj) {
	if constexpr (std::numeric_limits<T>::is_integer) {
		if constexpr (std::is_same_v<uint64_t, T> || std::is_same_v<int64_t, T>)
			ImGui::Text("%s: %zd", label, obj);
		else
			ImGui::Text("%s: %d", label, obj);
	}
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
	if constexpr (Dim >= 3)
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

void Viewer(const char* label, bool obj);
void Viewer(const char* label, const char* str);
void Viewer(const char* label, const hh::game::ObjectId& obj);
void Viewer(const char* label, void* const& obj);
void Viewer(const char* label, const Eigen::Quaternionf& quat);
void Viewer(const char* label, const hh::fnd::WorldPosition& worldPos);
void Viewer(const char* label, const csl::math::Transform& transform);
void Viewer(const char* label, const csl::ut::Color8& color);
void Viewer(const char* label, const csl::ut::Color<float>& color);

bool BeginVectorViewerTable(const char* id, bool withWAxis);
void EndVectorViewerTable();

template<typename T, int Rows, int Cols>
static void Viewer(const char* label, const Eigen::Matrix<T, Rows, Cols>& mat) {
	auto cols = reinterpret_cast<const Eigen::Matrix<T, Rows, 1>(&)[4]>(mat);

	if (BeginVectorViewerTable(label, true)) {
		for (int i = 0; i < Cols; i++) {
			Viewer(label, cols[i]);
		}

		EndVectorViewerTable();
	}
}
