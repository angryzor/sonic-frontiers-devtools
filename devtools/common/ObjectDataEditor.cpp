#include "../Pch.h"
#include "ObjectDataEditor.h"
#include "ReflectionEditor.h"
#include "../common/SimpleWidgets.h"

using namespace hh::game;

void ObjectDataEditor::Render(ObjectData* obj)
{
	auto* objSystem = GameObjectSystem::GetInstance();

	ImGui::Text("Id: %016zx%016zx", obj->id.groupId, obj->id.objectId);
	InputText("Name", &obj->name);
	ImGui::Text("Class: %s", obj->gameObjectClass);
	ImGui::SeparatorText("Transform");
	ImGui::PushID("Transform");
	ImGui::DragFloat3("Position", obj->transform.position.data(), 0.05f);
	ImGui::DragFloat3("Rotation", obj->transform.rotation.data(), 0.005f);
	ImGui::PopID();
	ImGui::SeparatorText("Local transform");
	ImGui::PushID("Local Transform");
	ImGui::DragFloat3("Position", obj->localTransform.position.data(), 0.05f);
	ImGui::DragFloat3("Rotation", obj->localTransform.rotation.data(), 0.005f);
	ImGui::PopID();
	ImGui::SeparatorText("Component configuration");

	for (auto* componentConfig : obj->componentData) {
		auto* gocInfo = objSystem->goComponentRegistry->GetComponentInformationByName(componentConfig->type);
		if (ImGui::CollapsingHeader(gocInfo->name)) {
			ImGui::Text("Component type: %s", gocInfo->componentClass->dynamicName);
			ImGui::Text("Configuration:");
			ReflectionEditor::Render(componentConfig->data, gocInfo->rflClass);
		}
	}

	auto* objClass = objSystem->gameObjectRegistry->GetGameObjectClassByName(obj->gameObjectClass);

	ImGui::SeparatorText("Object information");
	ReflectionEditor::Render(obj->objInfo, objClass->rflClass);
}
