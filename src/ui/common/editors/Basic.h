#pragma once

#include <ui/common/inputs/Basic.h>

// Editors
template<typename T, ImGuiDataType = imgui_datatype<T>::dtype>
static bool Editor(const char* label, T& obj) {
	return DragScalar(label, obj);
}

template<typename T, size_t Len>
static bool Editor(const char* label, T(&arr)[Len]) {
	bool edited{};
	char name[200];
	snprintf(name, sizeof(name), "%s[0..%zd]", label, Len);

	if (ImGui::TreeNode(name)) {
		for (size_t i = 0; i < Len; i++) {
			snprintf(name, sizeof(name), "%s[%zd]", label, i);
			edited |= Editor(name, arr[i]);
		}

		ImGui::TreePop();
	}

	return edited;
}


template<typename T, int Rows, int Cols>
static bool Editor(const char* label, Eigen::Matrix<T, Rows, Cols>& mat) {
	bool edited{};
	auto cols = reinterpret_cast<Eigen::Matrix<T, Rows, 1>(&)[4]>(mat);

	ImGui::PushID(label);
	for (int i = 0; i < Cols; i++) {
		ImGui::PushID(i++);
		edited |= DragScalar(label, cols[i]);
		ImGui::PopID();
	}
	ImGui::PopID();

	return edited;
}
template<size_t Len>
static bool Editor(const char* label, char (&str)[Len]) {
    return InputText(label, str);
}

bool Editor(const char* label, bool& obj);
bool Editor(const char* label, csl::ut::VariableString& str);
bool Editor(const char* label, csl::ut::String& str);
bool Editor(const char* label, Eigen::Quaternionf& quat);
bool Editor(const char* label, csl::ut::Color<float>& color);
bool Editor(const char* label, csl::ut::Color8& color);
bool Editor(const char* label, hh::game::ObjectId& id);
bool Editor(const char* label, hh::game::GameObject*& gameObject);
bool Editor(const char* label, hh::fnd::WorldPosition& worldPos);
bool Editor(const char* label, csl::math::Transform& transform);

template<typename T, std::enable_if_t<std::is_base_of_v<hh::game::GameObject, T>, bool> = true>
static bool Editor(const char* label, T*& gameObject) {
	return Editor(label, reinterpret_cast<hh::game::GameObject*&>(gameObject));
}
