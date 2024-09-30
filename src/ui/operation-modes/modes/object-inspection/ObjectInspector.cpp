#include "ObjectInspector.h"
#include "Behaviors.h"
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
#include "component-inspectors/GOCActivator.h"
#include "component-inspectors/GOCVisual.h"
#include "component-inspectors/PathComponent.h"
#include "component-inspectors/GOCEvent.h"
#include "component-inspectors/GOCPlayerBlackboard.h"
#include "component-inspectors/GOCPlayerKinematicParams.h"
#include "component-inspectors/GOCCamera.h"
#include "component-inspectors/rangers/GOCPlayerParameter.h"
#include "component-inspectors/rangers/GOCPlayerCollider.h"
#include "component-inspectors/rangers/GOCColliderQuery.h"

typedef std::tuple<
	hh::game::GOCTransform,
	hh::game::GOCActivator,
	hh::physics::GOCSphereCollider,
	hh::physics::GOCBoxCollider,
	hh::physics::GOCCapsuleCollider,
	hh::physics::GOCCylinderCollider,
	hh::gfx::GOCVisual,
	hh::gfx::GOCVisualTransformed,
	hh::gfx::GOCVisualModel,
	hh::anim::GOCAnimator,
	hh::path::PathComponent,
	app_cmn::camera::GOCCamera,
	app::game::GOCEventCollision,
	app::player::GOCPlayerKinematicParams,
	app::player::GOCPlayerBlackboard,
	app::player::GOCPlayerParameter,
	app::player::GOCPlayerCollider,
	app::physics::GOCMoveSphereColliderQuery
> InspectableComponents;
#endif

#ifdef DEVTOOLS_TARGET_SDK_wars
typedef std::tuple<> InspectableGameObjects;
#endif

#ifdef DEVTOOLS_TARGET_SDK_rangers
#include "gameobject-inspectors/CameraFrame.h"
#include "gameobject-inspectors/ObjCameraVolume.h"

typedef std::tuple<
	app_cmn::camera::CameraFrame,
	app::ObjCameraVolume
> InspectableGameObjects;
#endif

namespace ui::operation_modes::modes::object_inspection {

	using namespace hh::fnd;
	using namespace hh::game;


	template<typename T>
	bool RenderComponentInspectorT(hh::game::GOComponent& component) {
		bool result{ component.pStaticClass == T::GetClass() };

		if (result) {
			ImGui::PushID(&component);
			RenderComponentInspector(static_cast<T&>(component));
			ImGui::PopID();
		}

		return result;
	}

	template<typename = InspectableComponents>
	class ComponentIterator;
	template<typename... T>
	class ComponentIterator<std::tuple<T...>> {
	public:
		static void Render(hh::game::GOComponent& component) {
			(RenderComponentInspectorT<T>(component) || ...) || (ImGui::Text("Inspector for this component not yet implemented"), true);
		}
	};


	template<typename T>
	bool RenderGameObjectInspectorT(hh::game::GameObject& gameObject) {
		bool result{ gameObject.objectClass == T::GetClass() };

		if (result) {
			ImGui::PushID(&gameObject);
			RenderGameObjectInspector(static_cast<T&>(gameObject));
			ImGui::PopID();
		}

		return result;
	}

	template<typename = InspectableGameObjects>
	class GameObjectIterator;
	template<typename... T>
	class GameObjectIterator<std::tuple<T...>> {
	public:
		static void Render(hh::game::GameObject& gameObject) {
			(RenderGameObjectInspectorT<T>(gameObject) || ...);
		}
	};

	void ObjectInspector::RenderPanel() {
		auto& selection = GetBehavior<SelectionBehavior<Context>>()->GetSelection();

		if (selection.size() == 0) {
			ImGui::Text("Select an object in the left pane.");
		}
		else if (selection.size() > 1) {
			ImGui::Text("Multiple objects selected");
		}
		else {
			auto focusedObject = selection[0];
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
							snprintf(title, 200, "%s (%s) - %x", component->pStaticClass->pName, component->pStaticClass->category, component->nameHash);

							if (ImGui::CollapsingHeader(title))
								ComponentIterator<>::Render(*component);

							ImGui::PopID();
						}
					}
					ImGui::EndChild();
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Details")) {
					GameObjectIterator<>::Render(*focusedObject);
					ImGui::EndTabItem();
				}
#ifdef DEVTOOLS_TARGET_SDK_rangers
				if (ImGui::BeginTabItem("Update configuration")) {
					if (ImGui::BeginChild("Content")) {
						CheckboxFlags("Asleep", focusedObject->statusFlags, GameObject::StatusFlags::ASLEEP);
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
							snprintf(title, 200, "%s (%s)", component->pStaticClass->pName, component->pStaticClass->category);

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
#endif

				ImGui::EndTabBar();
			}
		}
	}

	PanelTraits ObjectInspector::GetPanelTraits() const
	{
		return { "Object inspector", ImVec2(ImGui::GetMainViewport()->WorkSize.x, 100), ImVec2(600, ImGui::GetMainViewport()->WorkSize.y - 140), ImVec2(1, 0) };
	}
}
