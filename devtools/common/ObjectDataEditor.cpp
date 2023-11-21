#include "../Pch.h"
#include "ObjectDataEditor.h"
#include "ReflectionEditor.h"

using namespace hh::game;

void ObjectDataEditor::Render(ObjectData* obj)
{
	auto* objSystem = GameObjectSystem::GetInstance();

	ImGui::Text("Id: %zx%zx", obj->id.objectId, obj->id.groupId);
	ImGui::Text("Name: %s", obj->name);
	ImGui::Text("Class: %s", obj->gameObjectClass);
	ImGui::SeparatorText("Transform");
	ImGui::PushID("Transform");
	ImGui::DragFloat3("Position", obj->transform.position.data());
	ImGui::DragFloat3("Rotation", obj->transform.rotation.data());
	ImGui::PopID();
	ImGui::SeparatorText("Local transform");
	ImGui::PushID("Transform");
	ImGui::DragFloat3("Position", obj->localTransform.position.data());
	ImGui::DragFloat3("Rotation", obj->localTransform.rotation.data());
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
