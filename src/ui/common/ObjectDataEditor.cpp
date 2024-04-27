#include "ObjectDataEditor.h"
#include "ReflectionEditor.h"
#include <ui/common/SimpleWidgets.h>
#include <utilities/ObjectDataUtils.h>

using namespace hh::game;

void ObjectDataEditor::Render(ObjectData* obj)
{
	auto* objSystem = GameObjectSystem::GetInstance();

	ImGui::Text("Id: %016zx%016zx", obj->id.groupId, obj->id.objectId);
	InputText("Name", &obj->name);
	ImGui::Text("Class: %s", obj->gameObjectClass);
	ImGui::Text("Parent object id: %016zx%016zx", obj->parentID.groupId, obj->parentID.objectId);

	ImGui::SeparatorText("Transform");
	if (ImGui::BeginTable("Transform", 4))
	{
		ImGui::TableSetupColumn("X");
		ImGui::TableSetupColumn("Y");
		ImGui::TableSetupColumn("Z");
		ImGui::TableSetupColumn("");

		ImGui::TableHeadersRow();

		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text("%f", obj->transform.position.x());
		ImGui::TableNextColumn();
		ImGui::Text("%f", obj->transform.position.y());
		ImGui::TableNextColumn();
		ImGui::Text("%f", obj->transform.position.z());
		ImGui::TableNextColumn();
		ImGui::Text("Position");

		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text("%f", obj->transform.rotation.x());
		ImGui::TableNextColumn();
		ImGui::Text("%f", obj->transform.rotation.y());
		ImGui::TableNextColumn();
		ImGui::Text("%f", obj->transform.rotation.z());
		ImGui::TableNextColumn();
		ImGui::Text("Rotation");

		ImGui::EndTable();
	}

	hh::game::ObjectTransformData localTransformData{ obj->localTransform };

	ImGui::SeparatorText("Local transform");
	ImGui::PushID("Local Transform");
	ImGui::DragFloat3("Position", localTransformData.position.data(), 0.05f);
	ImGui::DragFloat3("Rotation", localTransformData.rotation.data(), 0.005f);
	ImGui::PopID();

	if (localTransformData != obj->localTransform)
		UpdateLocalTransform(ObjectTransformDataToAffine3f(localTransformData), *obj);

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

	if (obj->spawnerData) {
		ImGui::SeparatorText("Object information");
		ReflectionEditor::Render(obj->spawnerData, objClass->spawnerDataRflClass);
	}
}
