#include "ObjectInspector.h"
#include "ObjectInspection.h"
#include <ui/common/viewers/Basic.h>
#include <ui/common/editors/Basic.h>
#include <ui/common/inputs/Basic.h>
#include <utilities/math/EulerTransform.h>
#include <utilities/math/MathUtils.h>
#include <imgui_internal.h>

#include "component-inspectors/GOCTransform.h"
#include "component-inspectors/wars/GOCPlayerParameter.h"

using namespace hh::fnd;
using namespace hh::game;



void RenderComponentInspector(GOComponent& component) {
	 if (component.pStaticClass == hh::game::GOCTransform::GetClass()) {
	 	RenderComponentInspector(static_cast<GOCTransform&>(component));
	 } else if (component.pStaticClass == app::player::GOCPlayerParameter::GetClass()) {
	 	RenderComponentInspector(static_cast<app::player::GOCPlayerParameter&>(component));
	 // } else if (component.pStaticClass == app::player::GOCPlayerParameter::GetClass()) {
	 // 	RenderGOCPlayerParameterInspector(static_cast<app::player::GOCPlayerParameter&>(component));
	 // } else if (component.pStaticClass == hh::physics::GOCSphereCollider::GetClass()) {
	 // 	RenderGOCSphereColliderInspector(static_cast<hh::physics::GOCSphereCollider&>(component));
	 // } else if (component.pStaticClass == hh::physics::GOCBoxCollider::GetClass()) {
	 // 	RenderGOCBoxColliderInspector(static_cast<hh::physics::GOCBoxCollider&>(component));
	 // } else if (component.pStaticClass == hh::physics::GOCCapsuleCollider::GetClass()) {
	 // 	RenderGOCCapsuleColliderInspector(static_cast<hh::physics::GOCCapsuleCollider&>(component));
	 // } else if (component.pStaticClass == hh::physics::GOCCylinderCollider::GetClass()) {
	 // 	RenderGOCCylinderColliderInspector(static_cast<hh::physics::GOCCylinderCollider&>(component));
	 // } else if (component.pStaticClass == hh::anim::GOCAnimator::GetClass()) {
	 // 	RenderGOCAnimatorInspector(static_cast<hh::anim::GOCAnimator&>(component));
	 // } else if (component.pStaticClass == app::game::GOCEventCollision::GetClass()) {
	 // 	RenderGOCEventCollisionInspector(static_cast<app::game::GOCEventCollision&>(component));
	 // } else if (component.pStaticClass == app::player::GOCPlayerKinematicParams::GetClass()) {
	 // 	RenderGOCPlayerKinematicParamsInspector(static_cast<app::player::GOCPlayerKinematicParams&>(component));
	 // } else if (component.pStaticClass == app::player::GOCPlayerBlackboard::GetClass()) {
	 // 	RenderGOCPlayerBlackboardInspector(static_cast<app::player::GOCPlayerBlackboard&>(component));
	 // } else if (component.pStaticClass == app_cmn::camera::GOCCamera::GetClass()) {
	 // 	RenderGOCCameraInspector(static_cast<app_cmn::camera::GOCCamera&>(component));
	 } else {
		 ImGui::Text("Inspector for this component not yet implemented");
	 }
}

ObjectInspector::ObjectInspector(csl::fnd::IAllocator* allocator, ObjectInspection& objectInspection) : CompatibleObject{ allocator }, objectInspection{ objectInspection }
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
			ImGui::Text("Class: %s", focusedObject->objectClass ? focusedObject->objectClass->name : "<none>");
			ImGui::Separator();
			if (ImGui::BeginTabBar("Inspector types")) {
				if (ImGui::BeginTabItem("Properties")) {
					if (ImGui::BeginChild("Content")) {
						for (auto* component : focusedObject->components) {
							ImGui::PushID(component);

							char title[200];
							snprintf(title, 200, "%s (%s)", component->pStaticClass->pName, component->pStaticClass->category);

							if (ImGui::CollapsingHeader(title))
								RenderComponentInspector(*component);

							ImGui::PopID();
						}
					}
					ImGui::EndChild();
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Details")) {
					// if (focusedObject->objectClass == app_cmn::camera::CameraFrame::GetClass()) {
					// 	auto* cameraFrame = static_cast<app_cmn::camera::CameraFrame*>(&*focusedObject);

					// }
					// if (focusedObject->objectClass == app::ObjCameraVolume::GetClass()) {
					// 	auto* cameraVolume = static_cast<app::ObjCameraVolume*>(&*focusedObject);

					// }
					ImGui::EndTabItem();
				}
				//if (ImGui::BeginTabItem("Update configuration")) {
				//	if (ImGui::BeginChild("Content")) {
				//		ImGui::Text("GameObject requested update phases:");
				//		ImGui::BeginGroup();
				//		CheckboxFlags("PreAnim", focusedObject->forcedUpdateFlags, UpdatingPhase::PRE_ANIM);
				//		if (ImGui::IsItemEdited())
				//			focusedObject->UNSAFE_SetUpdateFlags(UpdatingPhase::PRE_ANIM);
				//		ImGui::SameLine();
				//		CheckboxFlags("PostAnim", focusedObject->forcedUpdateFlags, UpdatingPhase::POST_ANIM);
				//		if (ImGui::IsItemEdited())
				//			focusedObject->UNSAFE_SetUpdateFlags(UpdatingPhase::POST_ANIM);
				//		ImGui::SameLine();
				//		CheckboxFlags("Final", focusedObject->forcedUpdateFlags, UpdatingPhase::FINAL);
				//		if (ImGui::IsItemEdited())
				//			focusedObject->UNSAFE_SetUpdateFlags(UpdatingPhase::FINAL);
				//		ImGui::EndGroup();
				//		ImGui::Text("Real update phases due to components:");
				//		ImGui::BeginDisabled();
				//		CheckboxFlags("PreAnim", focusedObject->updateFlags, UpdatingPhase::PRE_ANIM);
				//		ImGui::SameLine();
				//		CheckboxFlags("PostAnim", focusedObject->updateFlags, UpdatingPhase::POST_ANIM);
				//		ImGui::SameLine();
				//		CheckboxFlags("Final", focusedObject->updateFlags, UpdatingPhase::FINAL);
				//		ImGui::EndDisabled();
				//		ImGui::SeparatorText("Component update configuration");
				//		for (auto* component : focusedObject->components) {
				//			ImGui::PushID(component);

				//			char title[200];
				//			snprintf(title, 200, "%s (%s)", component->pStaticClass->pName, component->pStaticClass->category);

				//			if (ImGui::CollapsingHeader(title)) {
				//				ImGui::Text("Update phases");
				//				//ImGui::BeginDisabled();
				//				ImGui::BeginGroup();
				//				CheckboxFlags("PreAnim", component->updateFlags, UpdatingPhase::PRE_ANIM);
				//				if (ImGui::IsItemDeactivatedAfterEdit()) {
				//					if (component->updateFlags.test(UpdatingPhase::PRE_ANIM)) {
				//						auto it = focusedObject->componentsByUpdatingPhase[static_cast<size_t>(UpdatingPhase::PRE_ANIM)].find(component);
				//						if (it == -1)
				//							focusedObject->componentsByUpdatingPhase[static_cast<size_t>(UpdatingPhase::PRE_ANIM)].push_back(component);
				//					}
				//					else {
				//						auto it = focusedObject->componentsByUpdatingPhase[static_cast<size_t>(UpdatingPhase::PRE_ANIM)].find(component);
				//						if (it != -1)
				//							focusedObject->componentsByUpdatingPhase[static_cast<size_t>(UpdatingPhase::PRE_ANIM)].remove(it);
				//					}
				//					focusedObject->UNSAFE_SetComponentLengths(UpdatingPhase::PRE_ANIM);
				//					focusedObject->UNSAFE_SetUpdateFlags(UpdatingPhase::PRE_ANIM);
				//				}
				//				ImGui::SameLine();
				//				CheckboxFlags("PostAnim", component->updateFlags, UpdatingPhase::POST_ANIM);
				//				if (ImGui::IsItemDeactivatedAfterEdit()) {
				//					if (component->updateFlags.test(UpdatingPhase::POST_ANIM)) {
				//						auto it = focusedObject->componentsByUpdatingPhase[static_cast<size_t>(UpdatingPhase::POST_ANIM)].find(component);
				//						if (it == -1)
				//							focusedObject->componentsByUpdatingPhase[static_cast<size_t>(UpdatingPhase::POST_ANIM)].push_back(component);
				//					}
				//					else {
				//						auto it = focusedObject->componentsByUpdatingPhase[static_cast<size_t>(UpdatingPhase::POST_ANIM)].find(component);
				//						if (it != -1)
				//							focusedObject->componentsByUpdatingPhase[static_cast<size_t>(UpdatingPhase::POST_ANIM)].remove(it);
				//					}
				//					focusedObject->UNSAFE_SetComponentLengths(UpdatingPhase::POST_ANIM);
				//					focusedObject->UNSAFE_SetUpdateFlags(UpdatingPhase::POST_ANIM);
				//				}
				//				ImGui::SameLine();
				//				CheckboxFlags("Final", component->updateFlags, UpdatingPhase::FINAL);
				//				if (ImGui::IsItemDeactivatedAfterEdit()) {
				//					if (component->updateFlags.test(UpdatingPhase::FINAL)) {
				//						auto it = focusedObject->componentsByUpdatingPhase[static_cast<size_t>(UpdatingPhase::FINAL)].find(component);
				//						if (it == -1)
				//							focusedObject->componentsByUpdatingPhase[static_cast<size_t>(UpdatingPhase::FINAL)].push_back(component);
				//					}
				//					else {
				//						auto it = focusedObject->componentsByUpdatingPhase[static_cast<size_t>(UpdatingPhase::FINAL)].find(component);
				//						if (it != -1)
				//							focusedObject->componentsByUpdatingPhase[static_cast<size_t>(UpdatingPhase::FINAL)].remove(it);
				//					}
				//					focusedObject->UNSAFE_SetComponentLengths(UpdatingPhase::FINAL);
				//					focusedObject->UNSAFE_SetUpdateFlags(UpdatingPhase::FINAL);
				//				}
				//				ImGui::EndGroup();
				//				//ImGui::EndDisabled();
				//				ImGui::Text("Update priorities:");
				//				ImGui::InputScalarN("PreAnim, PostAnim, Final", ImGuiDataType_U8, component->updatePriorityByPhase, 3);
				//			}

				//			ImGui::PopID();
				//		}
				//	}
				//	ImGui::EndChild();
				//	ImGui::EndTabItem();
				//}
				ImGui::EndTabBar();
			}
		}
	}
	ImGui::End();
}
