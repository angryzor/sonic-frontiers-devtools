#include "ObjectInspector.h"
#include "ObjectInspection.h"
#include <ui/common/viewers/Basic.h>
#include <ui/common/editors/Basic.h>
#include <ui/common/inputs/Basic.h>
#include <utilities/math/EulerTransform.h>
#include <utilities/math/MathUtils.h>
#include <imgui_internal.h>

#ifdef DEVTOOLS_TARGET_SDK_wars
#include "component-inspectors/GOCTransform.h"
#include "component-inspectors/GOCCollider.h"
#include "component-inspectors/wars/GOCPlayerParameter.h"

typedef std::tuple<
	hh::game::GOCTransform,
	hh::physics::GOCSphereCollider,
	hh::physics::GOCBoxCollider,
	hh::physics::GOCCapsuleCollider,
	hh::physics::GOCCylinderCollider,
	app::player::GOCPlayerParameter
> InspectableComponents;
#endif

#ifdef DEVTOOLS_TARGET_SDK_rangers
#include "component-inspectors/GOCTransform.h"
#include "component-inspectors/GOCCollider.h"
#include "component-inspectors/GOCAnimator.h"
#include "component-inspectors/GOCEvent.h"
#include "component-inspectors/GOCPlayerBlackboard.h"
#include "component-inspectors/GOCPlayerKinematicParams.h"
#include "component-inspectors/GOCCamera.h"
#include "component-inspectors/rangers/GOCPlayerParameter.h"

typedef std::tuple<
	hh::game::GOCTransform,
	hh::physics::GOCSphereCollider,
	hh::physics::GOCBoxCollider,
	hh::physics::GOCCapsuleCollider,
	hh::physics::GOCCylinderCollider,
	hh::anim::GOCAnimator,
	app_cmn::camera::GOCCamera,
	app::game::GOCEventCollision,
	app::player::GOCPlayerKinematicParams,
	app::player::GOCPlayerBlackboard,
	app::player::GOCPlayerParameter
> InspectableComponents;
#endif

using namespace hh::fnd;
using namespace hh::game;


template<typename T>
bool RenderComponentInspectorT(hh::game::GOComponent& service) {
	bool result{ service.pStaticClass == T::GetClass() };

	if (result) {
		RenderComponentInspector(static_cast<T&>(service));
	}

	return result;
}

template<typename = InspectableComponents>
class ComponentIterator;
template<typename... T>
class ComponentIterator<std::tuple<T...>> {
public:
	static void Render(hh::game::GOComponent& service) {
		(RenderComponentInspectorT<T>(service) || ...) || (ImGui::Text("Inspector for this component not yet implemented"), true);
	}
};

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
								ComponentIterator<>::Render(*component);

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
