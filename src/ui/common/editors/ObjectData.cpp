#include "ObjectData.h"
#include <ui/common/inputs/Basic.h>
#include <ui/common/editors/Basic.h>
#include <ui/common/editors/Reflection.h>
#include <ui/common/viewers/ObjectData.h>
#include <utilities/ObjectDataUtils.h>
#include "ObjectData.h"

using namespace hh::game;

bool Editor(const char* label, hh::game::ObjectTransformData& obj)
{
	bool edited{};
	ImGui::PushID(label);
	ImGui::BeginGroup();
	edited |= Editor("Position", obj.position);
	edited |= DragScalar("Rotation", obj.rotation, 0.005f);
	ImGui::EndGroup();
	ImGui::PopID();
	return edited;
}

bool Editor(const char* label, hh::game::ObjectData& obj)
{
	bool edited{};
	auto* objSystem = GameObjectSystem::GetInstance();

	ImGui::Text("Id: %016zx%016zx", obj.id.groupId, obj.id.objectId);
	edited |= Editor("Name", obj.name);
	ImGui::Text("Class: %s", obj.gameObjectClass);
	ImGui::Text("Parent object id: %016zx%016zx", obj.parentID.groupId, obj.parentID.objectId);

	ImGui::SeparatorText("Transform");
	Viewer("Transform", obj.transform);

	ObjectTransformData localTransformData{ obj.localTransform };

	ImGui::SeparatorText("Local transform");
	bool editedTransform = Editor("Local transform", localTransformData);
	edited |= editedTransform;

	if (editedTransform)
		UpdateLocalTransform(ObjectTransformDataToAffine3f(localTransformData), obj);

	ImGui::SeparatorText("Component configuration");

	for (auto* componentConfig : obj.componentData) {
		auto* gocInfo = objSystem->goComponentRegistry->GetComponentInformationByName(componentConfig->type);
		if (ImGui::CollapsingHeader(gocInfo->name)) {
			ImGui::Text("Component type: %s", gocInfo->componentClass->dynamicName);
			ImGui::Text("Configuration:");
			edited |= ReflectionEditor("Component properties", componentConfig->data, gocInfo->rflClass);
		}
	}

	//if (ImGui::Button("Add component..."))
	//	ImGui::OpenPopup("ComponentConfigSelection");

	//if (ImGui::BeginPopup("ComponentConfigSelection")) {
	//	for (auto* componentConfig : objSystem->goComponentRegistry->GetComponents()) {
	//		if (ImGui::Selectable(componentConfig->name)) {
	//			if (!obj.flags.test(ObjectData::Flag::COMPONENT_DATA_NEEDS_TERMINATION)) {
	//				obj.flags.set(ObjectData::Flag::COMPONENT_DATA_NEEDS_TERMINATION);
	//			obj.componentData.push_back()
	//		}
	//	}
	//	ImGui::EndPopup();
	//}

	if (obj.spawnerData) {
		auto* objClass = objSystem->gameObjectRegistry->GetGameObjectClassByName(obj.gameObjectClass);

		ImGui::SeparatorText("Object properties");
		edited |= ReflectionEditor("Object properties", obj.spawnerData, objClass->spawnerDataRflClass);
	}

	return edited;
}