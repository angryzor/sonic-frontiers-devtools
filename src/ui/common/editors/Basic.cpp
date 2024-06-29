#include "Basic.h"
#include <utilities/math/MathUtils.h>
#include <ui/common/inputs/Basic.h>
#include <ui/common/viewers/Basic.h>
#include <ui/Desktop.h>
#include <ui/operation-modes/ObjectInspection/ObjectInspection.h>
#include <ui/operation-modes/LevelEditor/LevelEditor.h>

bool Editor(const char* label, Eigen::Quaternionf& quat) {
	auto euler = MatrixToEuler(quat.toRotationMatrix());
	bool edited = DragScalar(label, euler, 0.005f);

	if (edited)
		quat = EulerToQuat(euler);

	return edited;
}

bool Editor(const char* label, hh::fnd::WorldPosition& worldPos) {
    bool edited{};
    ImGui::PushID(label);
	ImGui::BeginGroup();
	edited |= Editor("Position", worldPos.m_Position);
	edited |= Editor("Rotation", worldPos.m_Rotation);
	ImGui::EndGroup();
    ImGui::PopID();
    return edited;
}

bool Editor(const char* label, csl::math::Transform& transform) {
    bool edited{};
    ImGui::PushID(label);
	ImGui::BeginGroup();
	edited |= Editor("Position", transform.position);
	edited |= Editor("Rotation", transform.rotation);
	edited |= Editor("Scale", transform.scale);
	ImGui::EndGroup();
    ImGui::PopID();
    return edited;
}

bool Editor(const char* label, csl::ut::Color<float>& color) {
	float editableColor[4]{ color.r, color.g, color.b , color.a };

	bool edited = ImGui::ColorEdit4(label, reinterpret_cast<float*>(&color), ImGuiColorEditFlags_Float);

	if (edited) {
		color.r = editableColor[0];
		color.g = editableColor[0];
		color.b = editableColor[0];
		color.a = editableColor[0];
	}

	return edited;
}

bool Editor(const char* label, csl::ut::Color8& color) {
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

bool Editor(const char* label, hh::game::ObjectId& id) {
    bool edited{};
	auto* objWorld = hh::game::GameManager::GetInstance()->GetService<hh::game::ObjectWorld>();

	if (objWorld == nullptr) {
		Viewer(label, id);
	}
	else {
		char unkNameBuf[50];
#ifdef DEVTOOLS_TARGET_SDK_wars
		sprintf_s(unkNameBuf, 50, "<%08x>", id.id);
#endif
#ifdef DEVTOOLS_TARGET_SDK_rangers
		sprintf_s(unkNameBuf, 50, "<%016zx%016zx>", id.groupId, id.objectId);
#endif

		const char* name = unkNameBuf;

		for (auto* chunk : objWorld->GetWorldChunks()) {
			int idx = chunk->GetObjectIndexById(id);

			if (idx == -1) ImGui::BeginDisabled();
			if (ImGui::Button("Select")) {
				if (auto* opMode = dynamic_cast<LevelEditor*>(&*Desktop::instance->operationMode))
					opMode->Select(chunk->GetWorldObjectStatusByIndex(idx).objectData);
			}
			if (idx == -1) ImGui::EndDisabled();

			ImGui::SameLine();

			if (idx != -1)
				name = chunk->GetWorldObjectStatusByIndex(idx).objectData->name;
		}

		bool isOpen = ImGui::BeginCombo(label, name);

		if (ImGui::BeginDragDropTarget()) {
			if (auto* payload = ImGui::AcceptDragDropPayload("ObjectData")) {
				auto* objData = *static_cast<hh::game::ObjectData**>(payload->Data);

				id = objData->id;
				edited = true;
			}
			ImGui::EndDragDropTarget();
		}

		if (isOpen) {
			for (auto* chunk : objWorld->GetWorldChunks()) {
				for (auto* layers : chunk->GetLayers()) {
					for (auto* obj : layers->GetResource()->GetObjects()) {
						if (edited |= ImGui::Selectable(obj->name))
							id = obj->id;

						if (id == obj->id)
							ImGui::SetItemDefaultFocus();
					}
				}
			}

			ImGui::EndCombo();
		}
	}
    
    return edited;
}

bool Editor(const char* label, hh::game::GameObject*& gameObject)
{
	bool edited{};

	ImGui::PushID(label);
	if (gameObject == nullptr) ImGui::BeginDisabled();
	if (ImGui::Button("Select")) {
		if (auto* opMode = dynamic_cast<ObjectInspection*>(&*Desktop::instance->operationMode))
			opMode->Select(gameObject);
		else if (auto* opMode = dynamic_cast<LevelEditor*>(&*Desktop::instance->operationMode))
		 	opMode->Select(gameObject);
	}
	if (gameObject == nullptr) ImGui::EndDisabled();

	ImGui::SameLine();

	if (ImGui::BeginCombo(label, gameObject == nullptr ? "<none>" : gameObject->name)) {
		for (auto* obj : hh::game::GameManager::GetInstance()->objects) {
			if (edited |= ImGui::Selectable(obj->name))
				gameObject = obj;

			if (gameObject == obj)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
	ImGui::PopID();

	return edited;
}

bool Editor(const char* label, csl::ut::VariableString& str) {
    return InputText(label, str);
}

bool Editor(const char* label, bool& obj)
{
	return ImGui::Checkbox(label, &obj);
}

bool Editor(const char* label, csl::ut::String& str) {
    return InputText(label, str);
}

