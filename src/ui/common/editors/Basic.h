#pragma once

#include <ui/common/inputs/Basic.h>
#include <utilities/math/MathUtils.h>
#include <resources/managed-memory/ManagedCArray.h>

// Editors
template<typename T, ImGuiDataType = imgui_datatype<T>::dtype>
static bool Editor(const char* label, T& obj) {
	return DragScalar(label, obj);
}

template<typename T, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
static bool Editor(const char* label, Eigen::Matrix<T, Rows, Cols, Options, MaxRows, MaxCols>& mat) {
	bool edited{};
	auto cols = reinterpret_cast<Eigen::Matrix<T, Rows, 1, Options, MaxRows>(&)[Cols]>(mat);

	ImGui::PushID(label);
	for (int i = 0; i < Cols; i++) {
		ImGui::PushID(i);
		edited |= DragScalar(label, cols[i]);
		ImGui::PopID();
	}
	ImGui::PopID();

	return edited;
}

template<typename T, int Dim, int Mode>
static bool Editor(const char* label, Eigen::Transform<T, Dim, Mode>& mat) {
	return Editor(label, mat.matrix());
}

template<size_t Len>
static bool Editor(const char* label, char (&str)[Len]) {
    return InputText(label, str);
}

bool Editor(const char* label, bool& obj);
bool Editor(const char* label, ucsl::strings::VariableString& str);
bool Editor(const char* label, csl::ut::String& str);
bool Editor(const char* label, hh::game::ObjectId& id);
bool Editor(const char* label, ucsl::objectids::ObjectIdV1& id);
bool Editor(const char* label, ucsl::objectids::ObjectIdV2& id);
bool Editor(const char* label, hh::game::GameObject*& gameObject);
bool Editor(const char* label, hh::fnd::WorldPosition& worldPos);
bool Editor(const char* label, csl::math::Transform& transform);

template<ucsl::colors::ChannelOrder order>
bool Editor(const char* label, ucsl::colors::Colorf<order>& color) {
	float editableColor[4]{ color.r, color.g, color.b, color.a };

	bool edited = ImGui::ColorEdit4(label, editableColor, ImGuiColorEditFlags_Float);

	if (edited) {
		color.r = editableColor[0];
		color.g = editableColor[1];
		color.b = editableColor[2];
		color.a = editableColor[3];
	}

	return edited;
}

template<ucsl::colors::ChannelOrder order>
bool Editor(const char* label, ucsl::colors::Color8<order>& color) {
	float colorAsFloat[]{
		static_cast<float>(color.r) / 255,
		static_cast<float>(color.g) / 255,
		static_cast<float>(color.b) / 255,
		static_cast<float>(color.a) / 255,
	};
	float editableColor[4]{ colorAsFloat[0], colorAsFloat[1], colorAsFloat[2], colorAsFloat[3] };

	bool edited = ImGui::ColorEdit4(label, editableColor, ImGuiColorEditFlags_Uint8);

	if (edited) {
		color.r = static_cast<uint8_t>(editableColor[0] * 255);
		color.g = static_cast<uint8_t>(editableColor[1] * 255);
		color.b = static_cast<uint8_t>(editableColor[2] * 255);
		color.a = static_cast<uint8_t>(editableColor[3] * 255);
	}

	return edited;
}

template<int Options>
bool Editor(const char* label, Eigen::Quaternion<float, Options>& quat) {
	auto euler = MatrixToEuler(quat.toRotationMatrix());
	bool edited = DragScalar(label, euler, 0.005f);

	if (edited)
		quat = EulerToQuat(euler);

	return edited;
}

template<typename T, std::enable_if_t<std::is_base_of_v<hh::game::GameObject, T>, bool> = true>
static bool Editor(const char* label, T*& gameObject) {
	return Editor(label, reinterpret_cast<hh::game::GameObject*&>(gameObject));
}

template<typename T, std::enable_if_t<std::is_base_of_v<hh::game::GameObject, T>, bool> = true>
static bool Editor(const char* label, hh::fnd::Handle<T>& hGameObject) {
	bool edited{};

	auto* gameObject = hh::game::GameObjectSystem::GetGameObjectByHandle(hGameObject);

	if (edited = Editor(label, gameObject))
		hGameObject = gameObject;

	return edited;
}

template<typename T, typename S>
static bool Editor(const char* label, csl::ut::Array<T, S>& arr) {
	bool edited{};

	if (ImGui::TreeNode(label, "%s[0..]", label)) {
		for (int i = 0; i < arr.size(); i++) {
			ImGui::PushID(i);
			if (ImGui::Button("x")) {
				arr.remove(i);
				edited = true;
			}
			ImGui::SameLine();

			char indexedName[200];
			snprintf(indexedName, sizeof(indexedName), "%s[%d]", label, i);

			edited |= Editor(indexedName, arr[i]);

			ImGui::PopID();
		}

		if (ImGui::Button("Add item")) {
			arr.emplace_back();
			edited = true;
		}

		ImGui::TreePop();
	}

	return edited;
}

template<typename T>
bool Editor(const char* label, std::optional<T>& data) {
	bool edited{};
	ImGui::PushID(label);

	char checkboxLabel[100];
	snprintf(checkboxLabel, sizeof(checkboxLabel), "%s###%s", data.has_value() ? "" : label, "Is Set");

	bool hasValue = data.has_value();
	if (Editor(checkboxLabel, hasValue)) {
		data = hasValue ? std::make_optional(T{}) : std::nullopt;
		edited = true;
	}

	if (data.has_value()) {
		ImGui::SameLine();
		edited |= Editor(label, data.value());
	}

	ImGui::PopID();
	return edited;
}

template<typename T, size_t Len>
static bool Editor(const char* label, T(&arr)[Len]) {
	bool edited{};
	char name[200];
	snprintf(name, sizeof(name), "%s[0..%zd]", label, Len - 1);

	if (ImGui::TreeNode(name)) {
		for (size_t i = 0; i < Len; i++) {
			snprintf(name, sizeof(name), "%s[%zd]", label, i);
			edited |= Editor(name, arr[i]);
		}

		ImGui::TreePop();
	}

	return edited;
}

template<typename T, typename S>
static bool Editor(const char* label, resources::ManagedCArray<T, S>& arr) {
	bool edited{};

	if (ImGui::TreeNode(label, "%s[0..]", label)) {
		for (S i = 0; i < arr.size(); i++) {
			ImGui::PushID(static_cast<int>(i));
			if (ImGui::Button("x")) {
				arr.remove(i);
				edited = true;
			}
			ImGui::SameLine();

			char indexedName[200];
			snprintf(indexedName, sizeof(indexedName), "%s[%d]", label, i);

			edited |= Editor(indexedName, arr[i]);

			ImGui::PopID();
		}

		if (ImGui::Button("Add item")) {
			arr.emplace_back();
			edited = true;
		}

		ImGui::TreePop();
	}

	return edited;
}

template<typename T, typename S, typename I, typename D>
static bool Editor(const char* label, hh::fnd::ManagedResource* resource, resources::ManagedCArray<T, S>& arr, I init, D deinit) {
	bool edited{};

	if (ImGui::TreeNode(label, "%s[0..]", label)) {
		for (S i = 0; i < arr.size(); i++) {
			ImGui::PushID(static_cast<int>(i));
			if (ImGui::Button("x")) {
				deinit(resource, arr[i]);
				arr.remove(i);
				edited = true;
			}
			ImGui::SameLine();

			char indexedName[200];
			snprintf(indexedName, sizeof(indexedName), "%s[%d]", label, i);

			edited |= Editor(indexedName, resource, arr[i]);

			ImGui::PopID();
		}

		if (ImGui::Button("Add item")) {
			arr.emplace_back();
			init(resource, arr[arr.size() - 1]);
			edited = true;
		}

		ImGui::TreePop();
	}

	return edited;
}
