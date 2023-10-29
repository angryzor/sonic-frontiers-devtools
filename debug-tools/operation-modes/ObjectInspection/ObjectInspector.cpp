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
		ImGui::End();
	}
}

void ObjectInspector::RenderComponentInspector(GOComponent& component) {
	if (component.pStaticClass == hh::game::GOCTransform::GetClass()) {
		RenderGOCTransformInspector(static_cast<GOCTransform&>(component));
	} else if(component.pStaticClass == app::player::GOCPlayerParameter::GetClass()) {
		RenderGOCPlayerParameterInspector(static_cast<app::player::GOCPlayerParameter&>(component));
	} {
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

void ObjectInspector::RenderGOCTransformInspector(GOCTransform& component) {
	if (ImGui::BeginTable("Transform", 5))
	{
		ImGui::TableSetupColumn("");
		ImGui::TableSetupColumn("X");
		ImGui::TableSetupColumn("Y");
		ImGui::TableSetupColumn("Z");
		ImGui::TableSetupColumn("W");

		ImGui::TableHeadersRow();

		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text("Position");
		ImGui::TableNextColumn();
		ImGui::Text("%f", component.position.x());
		ImGui::TableNextColumn();
		ImGui::Text("%f", component.position.y());
		ImGui::TableNextColumn();
		ImGui::Text("%f", component.position.z());
		ImGui::TableNextColumn();
		ImGui::Text("%f", component.position.w());

		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text("Rotation");
		ImGui::TableNextColumn();
		ImGui::Text("%f", component.rotation.x());
		ImGui::TableNextColumn();
		ImGui::Text("%f", component.rotation.y());
		ImGui::TableNextColumn();
		ImGui::Text("%f", component.rotation.z());
		ImGui::TableNextColumn();
		ImGui::Text("%f", component.rotation.w());

		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text("Scale");
		ImGui::TableNextColumn();
		ImGui::Text("%f", component.scale.x());
		ImGui::TableNextColumn();
		ImGui::Text("%f", component.scale.y());
		ImGui::TableNextColumn();
		ImGui::Text("%f", component.scale.z());
		ImGui::TableNextColumn();
		ImGui::Text("%f", component.scale.w());

		ImGui::EndTable();
	}
}
