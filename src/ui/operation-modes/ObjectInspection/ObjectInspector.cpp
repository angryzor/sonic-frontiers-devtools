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
		if (objectInspection.focusedObjects.size() == 0) {
			ImGui::Text("Select an object in the left pane.");
		}
		else if (objectInspection.focusedObjects.size() > 1) {
			ImGui::Text("Multiple objects selected");
		}
		else {
			auto focusedObject = objectInspection.focusedObjects[0];
			ImGui::Text("Object name: %s", focusedObject->name.c_str());
			ImGui::Text("Layer: %d", focusedObject->layer);
			ImGui::Separator();
			if (ImGui::BeginTabBar("Inspector types")) {
				if (ImGui::BeginTabItem("Components")) {
					if (ImGui::BeginChild("Content")) {
						for (auto* component : focusedObject->components) {
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
	} else if (component.pStaticClass == hh::anim::GOCAnimator::GetClass()) {
		RenderGOCAnimatorInspector(static_cast<hh::anim::GOCAnimator&>(component));
	} else if (component.pStaticClass == app::game::GOCEventCollision::GetClass()) {
		RenderGOCEventCollisionInspector(static_cast<app::game::GOCEventCollision&>(component));
	} else if (component.pStaticClass == app::player::GOCPlayerKinematicParams::GetClass()) {
		RenderGOCPlayerKinematicParamsInspector(static_cast<app::player::GOCPlayerKinematicParams&>(component));
	} else {
		RenderUnknownComponentInspector(component);
	}
}

void ObjectInspector::RenderUnknownComponentInspector(GOComponent& component) {
	ImGui::Text("Inspector for this component not yet implemented");
}

const char* playerParameterModes[] = { "Normal", "Water", "Cyberspace Forward View", "Cyberspace Side View" };
const char* playerParameterSuperStates[] = { "Normal", "Super Sonic", "Super Sonic 2" };

void ObjectInspector::RenderGOCPlayerParameterInspector(app::player::GOCPlayerParameter& component) {
	auto* obj = component.owner;

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

	ComboEnum("Mode", &component.mode, playerParameterModes);
	ComboEnum("Super state", &component.superState, playerParameterSuperStates);

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

	if (ImGui::TreeNode("Currently loaded mode parameters")) {
		ImGui::SeparatorText("Mode packages");
		RenderModeParameterInspector("Mode packages", component.modePackages);
		if (ImGui::TreeNode("Water mode package")) {
			ReflectionEditor::Render(*component.waterModePackage);
			ImGui::TreePop();
		}
		ImGui::SeparatorText("Parameters");
		RenderModeParameterInspector("common", component.commonParameters);
		RenderModeParameterInspector("speed", component.speedParameters);
		RenderModeParameterInspector("jump", component.jumpParameters);
		RenderModeParameterInspector("jumpSpeed", component.jumpSpeedParameters);
		RenderModeParameterInspector("doubleJump", component.doubleJumpParameters);
		RenderModeParameterInspector("boost", component.boostParameters);
		RenderModeParameterInspector("airBoost", component.airBoostParameters);
		ImGui::TreePop();
	}
}

void ObjectInspector::RenderGOCPlayerKinematicParamsInspector(app::player::GOCPlayerKinematicParams& component)
{
	VectorEditor("Position", component.position);
	QuaternionEditor("Rotation", component.rotation);
	VectorEditor("Scale", component.scale);
	ImGui::PushID("WorldPos");
	ImGui::SeparatorText("World position");
	ImGui::PopID();
	WorldPositionEditor(component.worldPosition);
	ImGui::SeparatorText("Physics");
	VectorEditor("Velocity", component.velocity);
	VectorEditor("Unk1", component.unk1);
	VectorEditor("Unk2", component.unk2);
	VectorEditor("Unk3", component.unk3);
	VectorEditor("Unk4", component.unk4);
	VectorEditor("Unk5", component.unk5);
	VectorEditor("Unk6", component.unk6);
	ImGui::Text("Unk7");
	MatrixValues(component.unk7);
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

void RenderBlendNode(hh::anim::BlendNodeBase* node) {
	const char* type;

	switch (node->type) {
	case hh::anim::BlendNodeType::LERP:
		type = "LERP";
		break;
	case hh::anim::BlendNodeType::ADDITIVE:
		type = "ADDITIVE";
		break;
	case hh::anim::BlendNodeType::CLIP:
		type = "CLIP";
		break;
	case hh::anim::BlendNodeType::OVERRIDE:
		type = "OVERRIDE";
		break;
	case hh::anim::BlendNodeType::LAYER:
		type = "LAYER";
		break;
	case hh::anim::BlendNodeType::MULTIPLY:
		type = "MULTIPLY";
		break;
	case hh::anim::BlendNodeType::BLEND_SPACE:
		type = "BLENDSPACE";
		break;
	case hh::anim::BlendNodeType::TWO_POINT_LERP:
		type = "TWO POINT LERP";
		break;
	default:
		type = "UNKNOWN";
		break;
	}

	if (ImGui::TreeNodeEx(node, ImGuiTreeNodeFlags_DefaultOpen, "%x - type %s - flags %x", node, type, node->flags)) {
		for (auto* child : node->children)
			RenderBlendNode(child);
		ImGui::TreePop();
	}
}

void ObjectInspector::RenderGOCAnimatorInspector(hh::anim::GOCAnimator& component)
{
	bool nope{ false };
	const char* select{ "Select one" };
	if (ImGui::BeginCombo("Current state", select)) {
		auto& stateIds = component.asmResourceManager->animatorResource->stateIdsByName;
		for (auto it = stateIds.begin(); it != stateIds.end(); it++) {
			if (ImGui::Selectable(it.key(), &nope)) {
				component.ChangeState(it.key());
			}
		}
		ImGui::EndCombo();
	}
	//if (ImGui::BeginCombo("Variables", select)) {
	ImGui::SeparatorText("Variables");
	auto& variableIds = component.asmResourceManager->animatorResource->variableIdsByName;

	for (auto it = variableIds.begin(); it != variableIds.end(); it++) {
		ImGui::Text("%d: %s", *it, it.key());
	}
	//}

	if (component.animationStateMachine->blendTree) {
		ImGui::SeparatorText("BlendNodes");
		RenderBlendNode(component.animationStateMachine->blendTree);
	}

	ImGui::SeparatorText("Layers");
	for (auto& layer : component.animationStateMachine->layers) {
		if (ImGui::TreeNodeEx(&layer, ImGuiTreeNodeFlags_None, "%d - %s", layer.layerId, component.asmResourceManager->animatorResource->binaryData->layers[layer.layerId].name)) {
			ImGui::Text("%d", layer.layerId);
			ImGui::Text("%x", layer.unk2);
			ImGui::Text("%x", layer.unk3);
			ImGui::Text("%x", layer.unk4);
			ImGui::Text("%x", layer.unk5);
			ImGui::Text("%x", layer.unk6);
			ImGui::Text("%x", layer.unk7);
			ImGui::Text("%x", layer.unk8);
			RenderBlendNode(layer.blendNode);
			ImGui::TreePop();
		}
	}
}

void ObjectInspector::RenderGOCEventInspector(app::game::GOCEvent& component)
{
	CheckboxFlagsLT32("Triggered", &component.flags, app::game::GOCEvent::Flag::ENABLED);
	CheckboxFlagsLT32("Unknown 1", &component.flags, app::game::GOCEvent::Flag::UNK1);
	CheckboxFlagsLT32("Deactivate after event (maybe)", &component.flags, app::game::GOCEvent::Flag::MAYBE_DEACTIVATE_AFTER_EVENT);
}

const char* gocEventCollisionConditions[] = { "ON_TRIGGER", "ON_STAY", "UNKNOWN", "PULSE" };

void ObjectInspector::RenderGOCEventCollisionInspector(app::game::GOCEventCollision& component)
{
	ImGui::SliderFloat("Time left", &component.timeLeft, 0, component.lifetime);
	ImGui::DragFloat("Lifetime", &component.lifetime);
	ImGui::Checkbox("Lifetime reached", &component.lifetimeReached);
	ComboEnum("Condition", &component.collisionListener.condition, gocEventCollisionConditions);
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
