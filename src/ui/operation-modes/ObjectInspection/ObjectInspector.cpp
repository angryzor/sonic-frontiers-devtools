#include "ObjectInspector.h"
#include "ObjectInspection.h"
#include <ui/common/SimpleWidgets.h>
#include <ui/resources/editors/ResReflectionEditor.h>
#include <utilities/math/EulerTransform.h>
#include <utilities/math/MathUtils.h>
#include <imgui_internal.h>

using namespace hh::fnd;
using namespace hh::game;

ObjectInspector::ObjectInspector(csl::fnd::IAllocator* allocator, ObjectInspection& objectInspection) : BaseObject{ allocator }, objectInspection{ objectInspection }
{
}

void ObjectInspector::Render() {
	const ImGuiWindowFlags windowFlags
		= 0;

	ImGui::SetNextWindowPos(ImVec2(ImGui::GetMainViewport()->WorkSize.x, 100), ImGuiCond_FirstUseEver, ImVec2(1, 0));
	ImGui::SetNextWindowSize(ImVec2(600, ImGui::GetMainViewport()->WorkSize.y - 140), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Object inspector", NULL, windowFlags)) {
		if (objectInspection.focusedObject == nullptr) {
			ImGui::Text("Select an object in the left pane.");
		}
		else {
			ImGui::Text("Object name: %s", objectInspection.focusedObject->pObjectName.c_str());
			ImGui::Text("Layer: %d", objectInspection.focusedObject->layer);
			ImGui::Separator();
			if (ImGui::BeginTabBar("Inspector types")) {
				if (ImGui::BeginTabItem("Components")) {
					if (ImGui::BeginChild("Content")) {
						for (auto* component : objectInspection.focusedObject->m_Components) {
							ImGui::PushID(component);

							char title[200];
							snprintf(title, 200, "%s (%s)", component->pStaticClass->pName, component->pStaticClass->dynamicName);

							if (ImGui::CollapsingHeader(title)) {
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
	} else if (component.pStaticClass == app::player::GOCPlayerParameter::GetClass()) {
		RenderGOCPlayerParameterInspector(static_cast<app::player::GOCPlayerParameter&>(component));
	} else if (component.pStaticClass == hh::physics::GOCSphereCollider::GetClass()) {
		RenderGOCSphereColliderInspector(static_cast<hh::physics::GOCSphereCollider&>(component));
	} else if (component.pStaticClass == hh::physics::GOCBoxCollider::GetClass()) {
		RenderGOCBoxColliderInspector(static_cast<hh::physics::GOCBoxCollider&>(component));
	} else if (component.pStaticClass == hh::physics::GOCCapsuleCollider::GetClass()) {
		RenderGOCCapsuleColliderInspector(static_cast<hh::physics::GOCCapsuleCollider&>(component));
	} else if (component.pStaticClass == hh::physics::GOCCylinderCollider::GetClass()) {
		RenderGOCCylinderColliderInspector(static_cast<hh::physics::GOCCylinderCollider&>(component));
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

void ObjectInspector::RenderGOCColliderInspector(hh::physics::GOCCollider& component)
{
	ImGui::SeparatorText("Collider information");
	ImGui::Text("Shape type: %d", component.shapeType);
	ImGui::Text("Filter category: %d", component.filterCategory);
	//if (ImGui::BeginTable("Scale", 4))
	//{
	//	ImGui::TableSetupColumn("X");
	//	ImGui::TableSetupColumn("Y");
	//	ImGui::TableSetupColumn("Z");
	//	ImGui::TableSetupColumn("");

	//	ImGui::TableHeadersRow();

	//	ImGui::TableNextRow();
	//	ImGui::TableNextColumn();
	//	ImGui::Text("%f", component.scale.x());
	//	ImGui::TableNextColumn();
	//	ImGui::Text("%f", component.scale.y());
	//	ImGui::TableNextColumn();
	//	ImGui::Text("%f", component.scale.z());
	//	ImGui::TableNextColumn();
	//	ImGui::Text("Scale");
	//	ImGui::EndTable();
	//}

	ImGui::SeparatorText("Local world position (editable form)");
	ImGui::BeginGroup();
	WorldPositionEditor(component.localWorldPosition);
	VectorEditor("Scale", component.scale);
	ImGui::EndGroup();

	if (ImGui::IsItemEdited()) {
		component.frame->MarkUpdate();
		component.flags.set(hh::physics::GOCCollider::Flag::FRAME_DIRTY);
	}

	ImGui::SeparatorText("Local world position (internal representation)");
	WorldPositionViewer("Local world position", component.localWorldPosition);

	ImGui::SeparatorText("Transformed world position");
	WorldPositionViewer("Transformed world position", component.transformedWorldPosition);

	if (component.frame) {
		ImGui::SeparatorText("HFrame 1 local transform");
		TransformViewer("HFrameTransformLocal", component.frame->localTransform);

		ImGui::SeparatorText("HFrame 1 full transform");
		TransformViewer("HFrameTransformFull", component.frame->fullTransform);
	}

	if (component.frame2) {
		ImGui::SeparatorText("HFrame 2 local transform");
		TransformViewer("HFrameTransformLocal", component.frame2->localTransform);

		ImGui::SeparatorText("HFrame 2 full transform");
		TransformViewer("HFrameTransformFull", component.frame2->fullTransform);
	}
}

void ObjectInspector::RenderGOCSphereColliderInspector(hh::physics::GOCSphereCollider& component)
{
	ImGui::SeparatorText("Parameters");
	ImGui::Text("Radius: %f", component.radius);
	RenderGOCColliderInspector(component);
}

void ObjectInspector::RenderGOCBoxColliderInspector(hh::physics::GOCBoxCollider& component)
{
	ImGui::SeparatorText("Parameters");
	if (BeginVectorViewerTable("Dimensions", false))
	{
		VectorViewerTableItem("Dimensions", component.dimensions);
		EndVectorViewerTable();
	}
	RenderGOCColliderInspector(component);
}

void ObjectInspector::RenderGOCCapsuleColliderInspector(hh::physics::GOCCapsuleCollider& component)
{
	ImGui::SeparatorText("Parameters");
	ImGui::Text("Radius: %f", component.radius);
	ImGui::Text("Height: %f", component.height);
	RenderGOCColliderInspector(component);
}

void ObjectInspector::RenderGOCCylinderColliderInspector(hh::physics::GOCCylinderCollider& component)
{
	ImGui::SeparatorText("Parameters");
	ImGui::Text("Radius: %f", component.radius);
	ImGui::Text("Height: %f", component.height);
	RenderGOCColliderInspector(component);
}

void ObjectInspector::RenderGOCTransformInspector(GOCTransform& component) {
	auto transform = component.transform;

	ImGui::SeparatorText("Main local transform (editable form)");
	TransformEditor(transform);
	if (ImGui::IsItemEdited())
		component.SetLocalTransform(transform);

	ImGui::SeparatorText("Main local transform (internal representation)");
	TransformViewer("GOCTransformTransform", component.transform);

	ImGui::SeparatorText("HFrame local transform");
	TransformViewer("HFrameTransformLocal", component.frame->localTransform);

	ImGui::SeparatorText("HFrame full transform");
	TransformViewer("HFrameTransformFull", component.frame->fullTransform);
}

//void ObjectInspector::RenderGOCInputInspector(GOCInput& component) {
//	auto inputComponent = component.GetInputComponent();
//
//	ImGui::Text("Internal player input id: %d", component.GetInternalPlayerInputId());
//	ImGui::SeparatorText("Action monitors (%d):", inputComponent->actionMonitors.size());
//	ImGui::SeparatorText("Axis monitors (%d):", inputComponent->axisMonitors.size());
//
//}
