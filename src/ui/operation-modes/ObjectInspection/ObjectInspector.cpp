#include "ObjectInspector.h"
#include "ObjectInspection.h"
#include <ui/resources/editors/ResReflectionEditor.h>
#include <utilities/math/EulerTransform.h>

using namespace hh::fnd;
using namespace hh::game;

ObjectInspector::ObjectInspector(csl::fnd::IAllocator* allocator, ObjectInspection& objectInspection) : BaseObject{ allocator }, objectInspection{ objectInspection }
{
}

void ObjectInspector::Render() {
	const ImGuiWindowFlags windowFlags
		= 0;

	ImGui::SetNextWindowPos(ImVec2(ImGui::GetMainViewport()->WorkSize.x, 100), ImGuiCond_Once, ImVec2(1, 0));
	ImGui::SetNextWindowSize(ImVec2(800, ImGui::GetMainViewport()->WorkSize.y - 140), ImGuiCond_Once);
	if (ImGui::Begin("Object inspector", NULL, windowFlags)) {
		if (objectInspection.focusedObject == nullptr) {
			ImGui::Text("Select an object in the left pane.");
		}
		else {
			if (ImGui::BeginTabBar("Inspector types")) {
				if (ImGui::BeginTabItem("Components")) {
					if (ImGui::BeginChild("Content")) {
						for (auto* component : objectInspection.focusedObject->m_Components) {
							ImGui::PushID(component);

							if (ImGui::CollapsingHeader(component->pStaticClass->dynamicName)) {
								RenderComponentInspector(*component);
							}

							ImGui::PopID();
						}
					}
					ImGui::EndChild();
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

	if (ImGui::Button("Edit character parameters..."))
		switch (component.characterId) {
		case app::player::CharacterId::SONIC:
			ResReflectionEditor::Create(GetAllocator(), &component.characterParameters->sonic);
			break;
		case app::player::CharacterId::AMY:
			ResReflectionEditor::Create(GetAllocator(), &component.characterParameters->amy);
			break;
		case app::player::CharacterId::KNUCKLES:
			ResReflectionEditor::Create(GetAllocator(), &component.characterParameters->knuckles);
			break;
		case app::player::CharacterId::TAILS:
			ResReflectionEditor::Create(GetAllocator(), &component.characterParameters->tails);
			break;
		}

	if (ImGui::Button("Edit Camera Set parameters..."))
		ResReflectionEditor::Create(GetAllocator(), &*component.cameraSetParameters);
}

void ObjectInspector::RenderGOCTransformInspector(GOCTransform& component) {
	EulerTransform localTransform{ component.transform };

	ImGui::SeparatorText("Main local transform (editable form)");

	ImGui::DragFloat3("Position", reinterpret_cast<float*>(&localTransform.position), 0.05f);
	ImGui::DragFloat3("Rotation", reinterpret_cast<float*>(&localTransform.rotation), 0.005f);
	ImGui::DragFloat3("Scale", reinterpret_cast<float*>(&localTransform.scale), 0.05f);

	ImGui::SeparatorText("Main local transform (internal representation)");
	RenderTransformViewer("GOCTransformTransform", component.transform);

	ImGui::SeparatorText("HFrame full transform");
	RenderTransformViewer("HFrameTransformFull", component.frame->fullTransform);

	ImGui::SeparatorText("HFrame local transform");
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
