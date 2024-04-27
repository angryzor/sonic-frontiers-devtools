#pragma once
#include <ui/common/ReflectionEditor.h>

class ObjectInspection;
class ObjectInspector : public hh::fnd::BaseObject {
    ObjectInspection& objectInspection;

    template<typename R>
    void RenderModeParameterInspector(const char* label, R (&parameters)[4]) {
        if (ImGui::TreeNode(label)) {
            if (ImGui::TreeNode("Normal")) {
                ReflectionEditor::Render(*parameters[0]);
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("Water")) {
                ReflectionEditor::Render(*parameters[1]);
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("Cyberspace Forward View")) {
                ReflectionEditor::Render(*parameters[2]);
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("Cyberspace Side View")) {
                ReflectionEditor::Render(*parameters[3]);
                ImGui::TreePop();
            }
            ImGui::TreePop();
        }
    }

    void RenderComponentInspector(hh::game::GOComponent& component);
	void RenderGOCTransformInspector(hh::game::GOCTransform& component);
	void RenderGOCPlayerParameterInspector(app::player::GOCPlayerParameter& component);
    void RenderGOCPlayerKinematicParamsInspector(app::player::GOCPlayerKinematicParams& component);
    void RenderGOCColliderInspector(hh::physics::GOCCollider& component);
    void RenderGOCSphereColliderInspector(hh::physics::GOCSphereCollider& component);
    void RenderGOCBoxColliderInspector(hh::physics::GOCBoxCollider& component);
    void RenderGOCCapsuleColliderInspector(hh::physics::GOCCapsuleCollider& component);
    void RenderGOCCylinderColliderInspector(hh::physics::GOCCylinderCollider& component);
    void RenderGOCAnimatorInspector(hh::anim::GOCAnimator& component);
    void RenderGOCEventInspector(app::game::GOCEvent& component);
    void RenderGOCEventCollisionInspector(app::game::GOCEventCollision& component);
	void RenderUnknownComponentInspector(hh::game::GOComponent& component);

public:
    ObjectInspector(csl::fnd::IAllocator* allocator, ObjectInspection& objectInspection);
    void Render();
};
