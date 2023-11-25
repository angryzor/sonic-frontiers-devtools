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

	csl::math::Position localPosition{ obj->localTransform.position };
	csl::math::Position localRotation{ obj->localTransform.rotation };

	ImGui::SeparatorText("Local transform");
	ImGui::PushID("Local Transform");
	ImGui::DragFloat3("Position", localPosition.data(), 0.05f);
	ImGui::DragFloat3("Rotation", localRotation.data(), 0.005f);
	ImGui::PopID();
	
	if (localPosition != obj->localTransform.position || localRotation != obj->localTransform.rotation) {
		Eigen::Transform<float, 3, Eigen::Affine> absoluteTransform{};
		Eigen::Transform<float, 3, Eigen::Affine> localTransform{};
		Eigen::Transform<float, 3, Eigen::Affine> updatedLocalTransform{};

		absoluteTransform.fromPositionOrientationScale(
			obj->transform.position,
			Eigen::AngleAxisf(obj->transform.rotation[1], Eigen::Vector3f::UnitY()) * Eigen::AngleAxisf(obj->transform.rotation[0], Eigen::Vector3f::UnitX()) * Eigen::AngleAxisf(obj->transform.rotation[2], Eigen::Vector3f::UnitZ()),
			csl::math::Vector3{ 1.0f, 1.0f, 1.0f }
		);

		localTransform.fromPositionOrientationScale(
			localPosition,
			Eigen::AngleAxisf(localRotation[1], Eigen::Vector3f::UnitY()) * Eigen::AngleAxisf(localRotation[0], Eigen::Vector3f::UnitX()) * Eigen::AngleAxisf(localRotation[2], Eigen::Vector3f::UnitZ()),
			csl::math::Vector3{ 1.0f, 1.0f, 1.0f }
		);

		updatedLocalTransform.fromPositionOrientationScale(
			obj->localTransform.position,
			Eigen::AngleAxisf(obj->localTransform.rotation[1], Eigen::Vector3f::UnitY()) * Eigen::AngleAxisf(obj->localTransform.rotation[0], Eigen::Vector3f::UnitX()) * Eigen::AngleAxisf(obj->localTransform.rotation[2], Eigen::Vector3f::UnitZ()),
			csl::math::Vector3{ 1.0f, 1.0f, 1.0f }
		);

		Eigen::Transform<float, 3, Eigen::Affine> parentTransform = absoluteTransform * localTransform.inverse();
		Eigen::Transform<float, 3, Eigen::Affine> updatedAbsoluteTransform = parentTransform * updatedLocalTransform;

		Eigen::Matrix3f updatedAbsoluteRotation;
		Eigen::Matrix3f updatedAbsoluteScaling;

		updatedAbsoluteTransform.computeRotationScaling(&updatedAbsoluteRotation, &updatedAbsoluteScaling);

		obj->transform.position = { updatedAbsoluteTransform.translation() };
		auto updatedAbsoluteEuler = updatedAbsoluteRotation.eulerAngles(1, 0, 2);
		obj->transform.rotation = { updatedAbsoluteEuler[1], updatedAbsoluteEuler[0], updatedAbsoluteEuler[2] };
		obj->localTransform.position = localPosition;
		obj->localTransform.rotation = localRotation;
	}
	
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
