#include "ObjectInspector.h"
#include "ObjectInspection.h"
#include <ui/common/viewers/Basic.h>
#include <ui/common/editors/Basic.h>
#include <ui/common/editors/Camera.h>
#include <ui/common/inputs/Basic.h>
#include <ui/resources/editors/ResReflectionEditor.h>
#include <utilities/math/EulerTransform.h>
#include <utilities/math/MathUtils.h>
#include <imgui_internal.h>

using namespace hh::fnd;
using namespace hh::game;

ObjectInspector::ObjectInspector(csl::fnd::IAllocator* allocator, ObjectInspection& objectInspection) : BaseObject{ allocator }, objectInspection{ objectInspection }
{
}

void RenderCameraBlendTree(app_cmn::camera::CameraFrame::BlendNode* node) {
	if (ImGui::TreeNode("Controller")) {
		ImGui::Text("Name: %s", node->controller->name.c_str());
		if (ImGui::TreeNode("Parameters")) {
			Editor("Parameters", node->controller->parameter);
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
	if (node->interpolator && ImGui::TreeNode("Interpolator")) {
		ImGui::Text("Frame id %d, name %s, address %zx", node->interpolator->cameraFrame->id, node->interpolator->cameraFrame->name.c_str(), node->interpolator->cameraFrame);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Parameters")) {
		Editor("Parameters", node->controller->parameter);
		ImGui::TreePop();
	}
	if (node->child1 && ImGui::TreeNode("Child 1")) {
		RenderCameraBlendTree(node->child1);
		ImGui::TreePop();
	}
	if (node->child2 && ImGui::TreeNode("Child 2")) {
		RenderCameraBlendTree(node->child2);
		ImGui::TreePop();
	}
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
			ImGui::Text("Class: %s", focusedObject->objectClass->name);
			ImGui::Separator();
			if (ImGui::BeginTabBar("Inspector types")) {
				if (ImGui::BeginTabItem("Properties")) {
					if (ImGui::BeginChild("Content")) {
						for (auto* component : focusedObject->components) {
							ImGui::PushID(component);

							char title[200];
							snprintf(title, 200, "%s (%s)", component->pStaticClass->pName, component->pStaticClass->dynamicName);

							if (ImGui::CollapsingHeader(title))
								RenderComponentInspector(*component);

							ImGui::PopID();
						}
					}
					ImGui::EndChild();
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Details")) {
					if (focusedObject->objectClass == app_cmn::camera::CameraFrame::GetClass()) {
						auto* cameraFrame = static_cast<app_cmn::camera::CameraFrame*>(&*focusedObject);

						ImGui::Text("Id: %d", cameraFrame->id);
						ImGui::Text("Name: %s", cameraFrame->name.c_str());
						if (ImGui::TreeNode("Parameters 1")) {
							Editor("Parameters 1", cameraFrame->cameraParameter1);
							ImGui::TreePop();
						}
						if (ImGui::TreeNode("Parameters 2")) {
							Editor("Parameters 2", cameraFrame->cameraParameter2);
							ImGui::TreePop();
						}
						if (ImGui::TreeNode("Frustum 1")) {
							Editor("Frustum 1", cameraFrame->frustumParameter1);
							ImGui::TreePop();
						}
						if (ImGui::TreeNode("Frustum 2")) {
							Editor("Frustum 2", cameraFrame->frustumParameter2);
							ImGui::TreePop();
						}
						if (ImGui::TreeNode("Pose")) {
							Editor("Pose", cameraFrame->pose);
							ImGui::TreePop();
						}

						if (cameraFrame->blendTree && ImGui::TreeNode("Blend tree")) {
							RenderCameraBlendTree(cameraFrame->blendTree);
							ImGui::TreePop();
						}

						ImGui::SeparatorText("Controllers");
						for (auto& controllerUnit : cameraFrame->controllers) {
							ImGui::PushID(&controllerUnit);
							ImGui::Text("Addr: %zx", controllerUnit.controller);
							ImGui::Text("Name: %s", controllerUnit.controller->name.c_str());
							Editor("Camera frame", controllerUnit.controller->cameraFrame);
							ImGui::Text("Priority: %d", controllerUnit.priority);
							if (ImGui::TreeNode("Parameters")) {
								Editor("Parameters", controllerUnit.controller->parameter);
								ImGui::TreePop();
							}
							ImGui::PopID();
						}
					}
					if (focusedObject->objectClass == app::ObjCameraVolume::GetClass()) {
						auto* cameraVolume = static_cast<app::ObjCameraVolume*>(&*focusedObject);

						ImGui::SeparatorText("Players inside volume");
						for (size_t i = 0; i < std::min(8ull, cameraVolume->enterCountPerPlayer.size()); i++) {
							char txt[20];
							snprintf(txt, sizeof(txt), "Player %zd", i);
							CheckboxFlags(txt, cameraVolume->playersInsideFlags, static_cast<uint8_t>(1 << i));
						}

						size_t i{};
						ImGui::SeparatorText("Player entry count");
						for (auto& count : cameraVolume->enterCountPerPlayer) {
							char txt[20];
							snprintf(txt, sizeof(txt), "Player %zd", i++);
							Editor(txt, count);
						}
					}
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Update configuration")) {
					if (ImGui::BeginChild("Content")) {
						ImGui::Text("GameObject requested update phases:");
						ImGui::BeginGroup();
						CheckboxFlags("PreAnim", focusedObject->forcedUpdateFlags, UpdatingPhase::PRE_ANIM);
						if (ImGui::IsItemEdited())
							focusedObject->UNSAFE_SetUpdateFlags(UpdatingPhase::PRE_ANIM);
						ImGui::SameLine();
						CheckboxFlags("PostAnim", focusedObject->forcedUpdateFlags, UpdatingPhase::POST_ANIM);
						if (ImGui::IsItemEdited())
							focusedObject->UNSAFE_SetUpdateFlags(UpdatingPhase::POST_ANIM);
						ImGui::SameLine();
						CheckboxFlags("Final", focusedObject->forcedUpdateFlags, UpdatingPhase::FINAL);
						if (ImGui::IsItemEdited())
							focusedObject->UNSAFE_SetUpdateFlags(UpdatingPhase::FINAL);
						ImGui::EndGroup();
						ImGui::Text("Real update phases due to components:");
						ImGui::BeginDisabled();
						CheckboxFlags("PreAnim", focusedObject->updateFlags, UpdatingPhase::PRE_ANIM);
						ImGui::SameLine();
						CheckboxFlags("PostAnim", focusedObject->updateFlags, UpdatingPhase::POST_ANIM);
						ImGui::SameLine();
						CheckboxFlags("Final", focusedObject->updateFlags, UpdatingPhase::FINAL);
						ImGui::EndDisabled();
						ImGui::SeparatorText("Component update configuration");
						for (auto* component : focusedObject->components) {
							ImGui::PushID(component);

							char title[200];
							snprintf(title, 200, "%s (%s)", component->pStaticClass->pName, component->pStaticClass->dynamicName);

							if (ImGui::CollapsingHeader(title)) {
								ImGui::Text("Update phases");
								//ImGui::BeginDisabled();
								ImGui::BeginGroup();
								CheckboxFlags("PreAnim", component->updateFlags, UpdatingPhase::PRE_ANIM);
								if (ImGui::IsItemDeactivatedAfterEdit()) {
									if (component->updateFlags.test(UpdatingPhase::PRE_ANIM)) {
										auto it = focusedObject->componentsByUpdatingPhase[static_cast<size_t>(UpdatingPhase::PRE_ANIM)].find(component);
										if (it == -1)
											focusedObject->componentsByUpdatingPhase[static_cast<size_t>(UpdatingPhase::PRE_ANIM)].push_back(component);
									}
									else {
										auto it = focusedObject->componentsByUpdatingPhase[static_cast<size_t>(UpdatingPhase::PRE_ANIM)].find(component);
										if (it != -1)
											focusedObject->componentsByUpdatingPhase[static_cast<size_t>(UpdatingPhase::PRE_ANIM)].remove(it);
									}
									focusedObject->UNSAFE_SetComponentLengths(UpdatingPhase::PRE_ANIM);
									focusedObject->UNSAFE_SetUpdateFlags(UpdatingPhase::PRE_ANIM);
								}
								ImGui::SameLine();
								CheckboxFlags("PostAnim", component->updateFlags, UpdatingPhase::POST_ANIM);
								if (ImGui::IsItemDeactivatedAfterEdit()) {
									if (component->updateFlags.test(UpdatingPhase::POST_ANIM)) {
										auto it = focusedObject->componentsByUpdatingPhase[static_cast<size_t>(UpdatingPhase::POST_ANIM)].find(component);
										if (it == -1)
											focusedObject->componentsByUpdatingPhase[static_cast<size_t>(UpdatingPhase::POST_ANIM)].push_back(component);
									}
									else {
										auto it = focusedObject->componentsByUpdatingPhase[static_cast<size_t>(UpdatingPhase::POST_ANIM)].find(component);
										if (it != -1)
											focusedObject->componentsByUpdatingPhase[static_cast<size_t>(UpdatingPhase::POST_ANIM)].remove(it);
									}
									focusedObject->UNSAFE_SetComponentLengths(UpdatingPhase::POST_ANIM);
									focusedObject->UNSAFE_SetUpdateFlags(UpdatingPhase::POST_ANIM);
								}
								ImGui::SameLine();
								CheckboxFlags("Final", component->updateFlags, UpdatingPhase::FINAL);
								if (ImGui::IsItemDeactivatedAfterEdit()) {
									if (component->updateFlags.test(UpdatingPhase::FINAL)) {
										auto it = focusedObject->componentsByUpdatingPhase[static_cast<size_t>(UpdatingPhase::FINAL)].find(component);
										if (it == -1)
											focusedObject->componentsByUpdatingPhase[static_cast<size_t>(UpdatingPhase::FINAL)].push_back(component);
									}
									else {
										auto it = focusedObject->componentsByUpdatingPhase[static_cast<size_t>(UpdatingPhase::FINAL)].find(component);
										if (it != -1)
											focusedObject->componentsByUpdatingPhase[static_cast<size_t>(UpdatingPhase::FINAL)].remove(it);
									}
									focusedObject->UNSAFE_SetComponentLengths(UpdatingPhase::FINAL);
									focusedObject->UNSAFE_SetUpdateFlags(UpdatingPhase::FINAL);
								}
								ImGui::EndGroup();
								//ImGui::EndDisabled();
								ImGui::Text("Update priorities:");
								ImGui::InputScalarN("PreAnim, PostAnim, Final", ImGuiDataType_U8, component->updatePriorityByPhase, 3);
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
	} else if (component.pStaticClass == app::player::GOCPlayerBlackboard::GetClass()) {
		RenderGOCPlayerBlackboardInspector(static_cast<app::player::GOCPlayerBlackboard&>(component));
	} else if (component.pStaticClass == app_cmn::camera::GOCCamera::GetClass()) {
		RenderGOCCameraInspector(static_cast<app_cmn::camera::GOCCamera&>(component));
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

	ComboEnum("Mode", component.mode, playerParameterModes);
	ComboEnum("Super state", component.superState, playerParameterSuperStates);

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
			Editor("Parameters", *component.waterModePackage);
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
	Editor("Position", component.position);
	Editor("Rotation", component.rotation);
	Editor("Scale", component.scale);
	ImGui::PushID("WorldPos");
	ImGui::SeparatorText("World position");
	ImGui::PopID();
	Editor("World position", component.worldPosition);
	ImGui::SeparatorText("Physics");
	Editor("Velocity", component.velocity);
	Editor("Unk1", component.unk1);
	Editor("Unk2", component.unk2);
	Editor("Unk3", component.unk3);
	Editor("Unk4", component.unk4);
	Editor("Unk5", component.unk5);
	Editor("Unk6", component.unk6);
	ImGui::Text("Unk7");
	Viewer("Unk7", component.unk7);
}

const char* GetBlackboardName(unsigned int nameHash) {
	switch (nameHash) {
		case csl::ut::HashString("BlackboardAmy"): return "BlackboardAmy";
		case csl::ut::HashString("BlackboardSpeed"): return "BlackboardSpeed";
		case csl::ut::HashString("BlackboardItem"): return "BlackboardItem";
		case csl::ut::HashString("BlackboardBattle"): return "BlackboardBattle";
		case csl::ut::HashString("BlackboardStatus"): return "BlackboardStatus";
		case csl::ut::HashString("BlackboardTails"): return "BlackboardTails";
		default: return "Unknown";
	}
}
void ObjectInspector::RenderGOCPlayerBlackboardInspector(app::player::GOCPlayerBlackboard& component)
{
	for (auto& content : component.blackboard->contents) {
		if (ImGui::TreeNode(&*content, "%s", GetBlackboardName(content->GetNameHash()))) {
			switch (content->GetNameHash()) {
			case csl::ut::HashString("BlackboardAmy"): RenderBlackboardAmyInspector(static_cast<app::player::BlackboardAmy&>(*content)); break;
			case csl::ut::HashString("BlackboardSpeed"): RenderBlackboardSpeedInspector(static_cast<app::player::BlackboardSpeed&>(*content)); break;
			case csl::ut::HashString("BlackboardItem"): RenderBlackboardItemInspector(static_cast<app::player::BlackboardItem&>(*content)); break;
			case csl::ut::HashString("BlackboardBattle"): RenderBlackboardBattleInspector(static_cast<app::player::BlackboardBattle&>(*content)); break;
			case csl::ut::HashString("BlackboardStatus"): RenderBlackboardStatusInspector(static_cast<app::player::BlackboardStatus&>(*content)); break;
			case csl::ut::HashString("BlackboardTails"): RenderBlackboardTailsInspector(static_cast<app::player::BlackboardTails&>(*content)); break;
			}
			ImGui::TreePop();
		}
	}
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
	Editor("Local world position", component.localWorldPosition);
	Editor("Scale", component.scale);
	ImGui::EndGroup();

	if (ImGui::IsItemEdited()) {
		component.frame->MarkUpdate();
		component.flags.set(hh::physics::GOCCollider::Flag::FRAME_DIRTY);
	}

	ImGui::SeparatorText("Local world position (internal representation)");
	Viewer("Local world position", component.localWorldPosition);

	ImGui::SeparatorText("Transformed world position");
	Viewer("Transformed world position", component.transformedWorldPosition);

	if (component.frame) {
		ImGui::SeparatorText("HFrame 1 local transform");
		Viewer("HFrameTransformLocal", component.frame->localTransform);

		ImGui::SeparatorText("HFrame 1 full transform");
		Viewer("HFrameTransformFull", component.frame->fullTransform);
	}

	if (component.frame2) {
		ImGui::SeparatorText("HFrame 2 local transform");
		Viewer("HFrameTransformLocal", component.frame2->localTransform);

		ImGui::SeparatorText("HFrame 2 full transform");
		Viewer("HFrameTransformFull", component.frame2->fullTransform);
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
		Viewer("Dimensions", component.dimensions);
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
	CheckboxFlags("Triggered", component.flags, app::game::GOCEvent::Flag::ENABLED);
	CheckboxFlags("Unknown 1", component.flags, app::game::GOCEvent::Flag::UNK1);
	CheckboxFlags("Deactivate after event (maybe)", component.flags, app::game::GOCEvent::Flag::MAYBE_DEACTIVATE_AFTER_EVENT);
}

const char* gocEventCollisionConditions[] = { "ON_TRIGGER", "ON_STAY", "UNKNOWN", "PULSE" };

void ObjectInspector::RenderGOCEventCollisionInspector(app::game::GOCEventCollision& component)
{
	ImGui::SliderFloat("Time left", &component.timeLeft, 0, component.lifetime);
	ImGui::DragFloat("Lifetime", &component.lifetime);
	ImGui::Checkbox("Lifetime reached", &component.lifetimeReached);
	ComboEnum("Condition", component.collisionListener.condition, gocEventCollisionConditions);
}

void ObjectInspector::RenderGOCCameraInspector(app_cmn::camera::GOCCamera& component)
{
	for (auto& unit : component.controllerUnits) {
		if (ImGui::TreeNodeEx(&unit, ImGuiTreeNodeFlags_None, "Controller unit %d", unit.id)) {
			if (auto* cameraFrame = static_cast<app_cmn::camera::CameraFrame*>(MessageManager::GetMessengerByHandle(unit.cameraFrame))) {
				ImGui::Text("Camera frame: %s", cameraFrame->name.c_str());
			}

			ImGui::SeparatorText("Controller");
			ImGui::Text("Addr: %zx", unit.controller);
			ImGui::Text("Name: %s", unit.controller->name.c_str());
			if (ImGui::TreeNode("Parameters")) {
				Editor("Parameters", unit.controller->parameter);
				ImGui::TreePop();
			}

			ImGui::TreePop();
		}
	}
}

void ObjectInspector::RenderGOCTransformInspector(GOCTransform& component) {
	auto transform = component.transform;

	ImGui::SeparatorText("Main local transform (editable form)");
	if (Editor("GOCTransformTransformEditable", transform))
		component.SetLocalTransform(transform);

	ImGui::SeparatorText("Main local transform (internal representation)");
	Viewer("GOCTransformTransform", component.transform);

	ImGui::SeparatorText("HFrame local transform");
	Viewer("HFrameTransformLocal", component.frame->localTransform);

	ImGui::SeparatorText("HFrame full transform");
	Viewer("HFrameTransformFull", component.frame->fullTransform);
}

//void ObjectInspector::RenderGOCInputInspector(GOCInput& component) {
//	auto inputComponent = component.GetInputComponent();
//
//	ImGui::Text("Internal player input id: %d", component.GetInternalPlayerInputId());
//	ImGui::SeparatorText("Action monitors (%d):", inputComponent->actionMonitors.size());
//	ImGui::SeparatorText("Axis monitors (%d):", inputComponent->axisMonitors.size());
//
//}

void ObjectInspector::RenderBlackboardAmyInspector(app::player::BlackboardAmy& blackboard) {
	ImGui::Text("Unk1: %zx", blackboard.unk1);
	ImGui::Text("Unk2: %x", blackboard.unk2);
}

void ObjectInspector::RenderBlackboardSpeedInspector(app::player::BlackboardSpeed& blackboard) {
	ImGui::Text("Unk1: %x", blackboard.speed);
	ImGui::Text("Unk2: %x", blackboard.speed2);
}

void ObjectInspector::RenderBlackboardItemInspector(app::player::BlackboardItem& blackboard) {
	static uint32_t zero{};

	ImGui::DragScalar("Ring capacity", ImGuiDataType_U32, &blackboard.ringCapacity);
	ImGui::SliderScalar("Ring count", ImGuiDataType_U32, &blackboard.ringCount, &zero, &blackboard.ringCapacity);
	ImGui::DragScalar("Unk1", ImGuiDataType_U32, &blackboard.unk1);
	ImGui::DragScalar("unk2", ImGuiDataType_U16, &blackboard.unk2);
	ImGui::DragScalar("unk3", ImGuiDataType_U8, &blackboard.unk3);
	ImGui::DragScalar("unk3b", ImGuiDataType_U8, &blackboard.unk3b);
	ImGui::DragScalar("unk4", ImGuiDataType_U8, &blackboard.unk4);
	ImGui::DragScalar("unk5", ImGuiDataType_U8, &blackboard.unk5);
	ImGui::DragScalar("unk5b", ImGuiDataType_U8, &blackboard.unk5b);
	ImGui::DragScalar("unk5c", ImGuiDataType_U8, &blackboard.unk5c);
	ImGui::DragScalar("unk6", ImGuiDataType_U16, &blackboard.unk6);
	ImGui::DragScalar("unk7", ImGuiDataType_U16, &blackboard.unk7);
	ImGui::DragScalar("unk8", ImGuiDataType_U8, &blackboard.unk8);
}

void ObjectInspector::RenderBlackboardBattleInspector(app::player::BlackboardBattle& blackboard) {
	ImGui::DragScalar("powerLevel", ImGuiDataType_U8, &blackboard.powerLevel);
	ImGui::DragScalar("defenseLevel", ImGuiDataType_U8, &blackboard.defenseLevel);
	ImGui::DragScalar("unk1", ImGuiDataType_U8, &blackboard.unk1);
	ImGui::DragScalar("unk2", ImGuiDataType_U8, &blackboard.unk2);
	Editor("unk3", blackboard.unk3);
	csl::ut::MoveArray<void*> debugArray1;
	csl::ut::MoveArray<void*> debugArray2;
	csl::ut::MoveArray<void*> unk4;
	ImGui::DragScalar("Unk5", ImGuiDataType_U32, &blackboard.unk5);
	ImGui::DragScalar("unk6", ImGuiDataType_U16, &blackboard.unk6);
	ImGui::DragScalar("unk7", ImGuiDataType_U8, &blackboard.unk7);
	ImGui::DragScalar("Combo count", ImGuiDataType_U32, &blackboard.comboCount);
	ImGui::DragFloat("Phantom rush amount", &blackboard.phantomRushAmount);
	Editor("Unk9", blackboard.unk9);
	ImGui::DragFloat("Unk10", &blackboard.unk10);
	ImGui::DragFloat("Quick Cyloop amount", &blackboard.quickCyloopAmount);
	ImGui::Text("%zx", blackboard.unkself12);
	ImGui::Text("%zx", blackboard.unk12);
	ImGui::Text("%zx", blackboard.unk13);
	ImGui::Text("%zx", blackboard.unk14);
	ImGui::Text("Unk16: %s", blackboard.unk16.c_str());
	ImGui::DragScalar("unk17", ImGuiDataType_U16, &blackboard.unk17);
}

const char* difficultyNames[]{ "EASY", "NORMAL", "HARD", "EXTREME" };

const char* stateFlagNames[64]{
	"BOOST",
	nullptr,
	nullptr,
	nullptr,
	"AIR_BOOST",
	nullptr,
	"GRIND_JUMP",
	"GRIND",
	"JUMP",
	"DOUBLE_JUMP",
	"BOUNCE_JUMP",
	"FALL",
	"STOMP",
	"DIVING",
	"DIVING_BOOST",
	nullptr,

	nullptr,
	"CYLOOP",
	"CYLOOP_END",
	"DRIFT",
	nullptr,
	nullptr,
	nullptr,
	"HOMING",
	"PARRY",
	"WALL_CLIMB",
	"IDLE",
	nullptr,
	"WALL_CLIMB_IDLE",
	nullptr,
	"BOARDING",
	nullptr,

	nullptr,
	"SPRING_JUMP",
	"SPIN_DASH",
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,

	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
};

const char* combatFlagNames[128]{
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	"SIDE_STEP",
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	"PARRY",

	"PERFECT_PARRY",
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	"AIR_TRICK",
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,

	nullptr,
	nullptr,
	"GRAND_SLAM",
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	"QUICK_CYLOOP",
	nullptr,
	"PHANTOM_RUSH",
	"CYCLONE_KICK",
	"RECOVERY_SMASH",
	nullptr,
	"SONIC_BOOM",
	"WILD_RUSH",

	"LOOP_KICK",
	"SPIN_SLASH",
	"STOMP_ATTACK",
	nullptr,
	"HOMING_SHOT",
	"CROSS_SLASH",
	nullptr,
	nullptr,
	nullptr,
	"AUTO_COMBO",
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	"SPIN_DASH",

	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,

	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,

	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,

	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
};

const char* worldFlagNames[128]{
	nullptr,
	"KILLED",
	"DAMAGED_OR_REPELLED",
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	"OUT_OF_CONTROL",
	nullptr,
	nullptr,
	"AUTO_RUN",
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,

	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	"CYBER_SPACE",
	nullptr,

	nullptr,
	nullptr,
	"WALL_JUMP_LAND",
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	"POWER_BOOST",
	nullptr,
	"AIR_TRICK",
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,

	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	"HEIGHT_MAP_COLLISION",
	"NO_CLIP",
	nullptr,
	"BATTLE",
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,

	nullptr,
	nullptr,
	nullptr,
	"NITRO_BOOST",
	"MAX_SPEED_CHALLENGE",
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,

	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,

	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,

	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
};

void ObjectInspector::RenderBlackboardStatusInspector(app::player::BlackboardStatus& blackboard) {
	ImGui::DragScalar("word20", ImGuiDataType_U16, &blackboard.word20);
	ImGui::DragScalar("dword24", ImGuiDataType_U32, &blackboard.dword24);

	if (ImGui::TreeNode("Combat flags")) {
		for (size_t i = 0; i < 64; i++) {
			char idxName[10];
			snprintf(idxName, sizeof(idxName), "%zd", i);

			CheckboxFlags(combatFlagNames[i] == nullptr ? idxName : combatFlagNames[i], blackboard.combatFlags[0], static_cast<app::player::BlackboardStatus::CombatFlag>(i));
		}
		for (size_t i = 64; i < 128; i++) {
			char idxName[10];
			snprintf(idxName, sizeof(idxName), "%zd", i);

			CheckboxFlags(combatFlagNames[i] == nullptr ? idxName : combatFlagNames[i], blackboard.combatFlags[1], static_cast<app::player::BlackboardStatus::CombatFlag>(i - 64));
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("State flags")) {
		for (size_t i = 0; i < 64; i++) {
			char idxName[10];
			snprintf(idxName, sizeof(idxName), "%zd", i);

			CheckboxFlags(stateFlagNames[i] == nullptr ? idxName : stateFlagNames[i], blackboard.stateFlags, static_cast<app::player::BlackboardStatus::StateFlag>(i));
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("World flags")) {
		for (size_t i = 0; i < 64; i++) {
			char idxName[10];
			snprintf(idxName, sizeof(idxName), "%zd", i);

			CheckboxFlags(worldFlagNames[i] == nullptr ? idxName : worldFlagNames[i], blackboard.worldFlags[0], static_cast<app::player::BlackboardStatus::WorldFlag>(i));
		}
		for (size_t i = 64; i < 128; i++) {
			char idxName[10];
			snprintf(idxName, sizeof(idxName), "%zd", i);

			CheckboxFlags(worldFlagNames[i] == nullptr ? idxName : worldFlagNames[i], blackboard.worldFlags[1], static_cast<app::player::BlackboardStatus::WorldFlag>(i - 64));
		}
		ImGui::TreePop();
	}

	ImGui::Text("Out of control time: %f", blackboard.outOfControlTime);
	ImGui::Text("In control time: %f", blackboard.inControlTime);
	ImGui::Text("%zx", blackboard.qword58);
	ImGui::Text("%zx", blackboard.qword60);
	//ut::PriorityList<bool, hh::fnd::Handle<hh::fnd::Messenger>> qword68;
	//csl::ut::InplaceMoveArray<uint32_t, 4> qword90;
	//csl::ut::InplaceMoveArray<uint32_t, 4> qwordC0;
	//csl::ut::InplaceMoveArray<uint32_t, 4> qwordF0;
	ImGui::Text("%zx", blackboard.qword120);
	ImGui::DragScalar("dword128", ImGuiDataType_U32, &blackboard.dword128);
	//ut::PriorityList<Dimension, hh::fnd::Handle<hh::fnd::Messenger>> qword130;
	Editor("oword160", blackboard.oword160);
	ImGui::DragScalar("dword170", ImGuiDataType_U32, &blackboard.dword170);
	ImGui::DragScalar("qword174", ImGuiDataType_U32, &blackboard.qword174);
	ImGui::DragScalar("qword178", ImGuiDataType_U32, &blackboard.qword178);
	ImGui::DragScalar("dword17C", ImGuiDataType_U32, &blackboard.dword17C);
	ComboEnum("Difficulty", blackboard.byte180, difficultyNames);
	ImGui::DragScalar("qword184", ImGuiDataType_U32, &blackboard.qword184);
	DragScalar("qword188", blackboard.qword188);
}

void ObjectInspector::RenderBlackboardTailsInspector(app::player::BlackboardTails& blackboard) {
	ImGui::DragScalar("Unk1", ImGuiDataType_U32, &blackboard.unk1);
	ImGui::DragScalar("Unk2", ImGuiDataType_U32, &blackboard.unk2);
	ImGui::DragScalar("Unk3", ImGuiDataType_U32, &blackboard.unk3);
	ImGui::DragScalar("Unk4", ImGuiDataType_U32, &blackboard.unk4);
	ImGui::DragScalar("Unk5", ImGuiDataType_U32, &blackboard.unk5);
	ImGui::DragScalar("Unk6", ImGuiDataType_U32, &blackboard.unk6);
}
