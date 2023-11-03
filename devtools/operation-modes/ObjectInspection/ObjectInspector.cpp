#include "../../Pch.h"
#include "ObjectInspector.h"
#include "ObjectInspection.h"
#include "../../resource-editors/ResReflectionEditor.h"

using namespace hh::fnd;
using namespace hh::game;

ObjectInspector::ObjectInspector(csl::fnd::IAllocator* allocator, ObjectInspection& objectInspection) : BaseObject{ allocator }, objectInspection{ objectInspection }
{
}

void ObjectInspector::Render() {
	const ImGuiWindowFlags windowFlags
		= ImGuiWindowFlags_MenuBar;

	ImGui::SetNextWindowPos(ImVec2(ImGui::GetMainViewport()->WorkSize.x, 60), ImGuiCond_Once, ImVec2(1, 0));
	ImGui::SetNextWindowSize(ImVec2(800, ImGui::GetMainViewport()->WorkSize.y - 100), ImGuiCond_Once);
	if (ImGui::Begin("Object inspector", NULL, windowFlags)) {
		if (objectInspection.focusedObject == nullptr) {
			ImGui::Text("Select an object in the left pane.");
		}
		else {
			if (ImGui::BeginTabBar("Inspector types")) {
				if (ImGui::BeginTabItem("Components")) {
					csl::ut::PointerMap<void*, int> amountSeen{ hh::fnd::GetTempAllocator(hh::fnd::GetAllocatorSystem()) };
					for (auto* component : objectInspection.focusedObject->m_Components) {
						int idx = amountSeen.GetValueOrFallback(component->GetClassId(), 0);
						amountSeen.Insert(component->GetClassId(), idx + 1);

						char buf[200];
						snprintf(buf, sizeof(buf), "%s (#%d)", component->pStaticClass->dynamicName, idx);

						if (ImGui::CollapsingHeader(buf)) {
							RenderComponentInspector(*component);
						}
					}
					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}
		}
	}
	ImGui::End();
}

void ObjectInspector::RenderComponentInspector(GOComponent& component) {
	if (component.pStaticClass == hh::game::GOCTransform::GetClass()) {
		RenderGOCTransformInspector(static_cast<GOCTransform&>(component));
	} else if (component.pStaticClass == app::player::GOCPlayerParameter::GetClass()) {
		RenderGOCPlayerParameterInspector(static_cast<app::player::GOCPlayerParameter&>(component));
	} else {
		RenderUnknownComponentInspector(component);
	}
}

void ObjectInspector::RenderUnknownComponentInspector(GOComponent& component) {
	ImGui::Text("Inspector for this component not yet implemented");
}

void ObjectInspector::RenderGOCPlayerParameterInspector(app::player::GOCPlayerParameter& component) {
	auto* obj = component.pOwnerGameObject;

	ImGui::Text("Character:");
	ImGui::SameLine();
	switch (component.characterId) {
	case app::player::CharacterId::SONIC:
		ImGui::Text("Sonic");
		break;
	case app::player::CharacterId::AMY:
		ImGui::Text("Amy");
		break;
	case app::player::CharacterId::KNUCKLES:
		ImGui::Text("Knuckles");
		break;
	case app::player::CharacterId::TAILS:
		ImGui::Text("Tails");
		break;
	default:
		ImGui::Text("Unknown");
		break;
	}

	ImGui::Text("Character parameters:");
	switch (component.characterId) {
	case app::player::CharacterId::SONIC:
		ResReflectionEditor::Render(component.characterParameters->sonic);
		break;
	case app::player::CharacterId::AMY:
		ResReflectionEditor::Render(component.characterParameters->amy);
		break;
	case app::player::CharacterId::KNUCKLES:
		ResReflectionEditor::Render(component.characterParameters->knuckles);
		break;
	case app::player::CharacterId::TAILS:
		ResReflectionEditor::Render(component.characterParameters->tails);
		break;
	}

	ImGui::Text("Camera Set Parameters:");
	ResReflectionEditor::Render(*component.cameraSetParameters);
}

struct EuclidianTransform {
	csl::math::Vector3 position;
	csl::math::Vector3 rotation;
	csl::math::Vector3 scale;

	EuclidianTransform(const csl::math::Transform& transform) : position{ transform.position }, rotation{ transform.rotation.toRotationMatrix().eulerAngles(0, 1, 2) }, scale{transform.scale} {}

	operator csl::math::Transform() {
		return {
			position,
			csl::math::Quaternion(
				Eigen::AngleAxisf(rotation[0], Eigen::Vector3f::UnitX()) *
				Eigen::AngleAxisf(rotation[1], Eigen::Vector3f::UnitY()) *
				Eigen::AngleAxisf(rotation[2], Eigen::Vector3f::UnitZ())
			),
			scale,
		};
	}

	bool operator==(const EuclidianTransform& other) const {
		return position == other.position && rotation == other.rotation && scale == other.scale;
	}

	bool operator!=(const EuclidianTransform& other) const {
		return !operator==(other);
	}
};

void ObjectInspector::RenderGOCTransformInspector(GOCTransform& component) {
	EuclidianTransform localTransform{ component.transform };

	ImGui::SeparatorText("Main local transform (editable form)");

	ImGui::DragFloat3("Position", reinterpret_cast<float*>(&localTransform.position), 0.05f);
	ImGui::DragFloat3("Rotation", reinterpret_cast<float*>(&localTransform.rotation), 0.005f);
	ImGui::DragFloat3("Scale", reinterpret_cast<float*>(&localTransform.scale), 0.05f);

	ImGui::SeparatorText("Main local transform (internal representation)");
	RenderTransformViewer("GOCTransformTransform", component.transform);

	ImGui::SeparatorText("HFrame full transform");
	RenderTransformViewer("HFrameTransformFull", component.frame->fullTransform);

	ImGui::SeparatorText("HFrame full transform");
	RenderTransformViewer("HFrameTransformLocal", component.frame->localTransform);

	if (localTransform != component.transform) {
		component.SetLocalTransform(localTransform);
	}
}

void ObjectInspector::RenderTransformViewer(const char* name, const csl::math::Transform& transform) {
	if (ImGui::BeginTable(name, 5))
	{
		ImGui::TableSetupColumn("X");
		ImGui::TableSetupColumn("Y");
		ImGui::TableSetupColumn("Z");
		ImGui::TableSetupColumn("W");
		ImGui::TableSetupColumn("");

		ImGui::TableHeadersRow();

		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text("%f", transform.position.x());
		ImGui::TableNextColumn();
		ImGui::Text("%f", transform.position.y());
		ImGui::TableNextColumn();
		ImGui::Text("%f", transform.position.z());
		ImGui::TableNextColumn();
		ImGui::TableNextColumn();
		ImGui::Text("Position");

		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text("%f", transform.rotation.x());
		ImGui::TableNextColumn();
		ImGui::Text("%f", transform.rotation.y());
		ImGui::TableNextColumn();
		ImGui::Text("%f", transform.rotation.z());
		ImGui::TableNextColumn();
		ImGui::Text("%f", transform.rotation.w());
		ImGui::TableNextColumn();
		ImGui::Text("Rotation");

		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text("%f", transform.scale.x());
		ImGui::TableNextColumn();
		ImGui::Text("%f", transform.scale.y());
		ImGui::TableNextColumn();
		ImGui::Text("%f", transform.scale.z());
		ImGui::TableNextColumn();
		ImGui::TableNextColumn();
		ImGui::Text("Scale");

		ImGui::EndTable();
	}
}

//void ObjectInspector::RenderGOCInputInspector(GOCInput& component) {
//	auto inputComponent = component.GetInputComponent();
//
//	ImGui::Text("Internal player input id: %d", component.GetInternalPlayerInputId());
//	ImGui::SeparatorText("Action monitors (%d):", inputComponent->actionMonitors.size());
//	ImGui::SeparatorText("Axis monitors (%d):", inputComponent->axisMonitors.size());
//
//}
