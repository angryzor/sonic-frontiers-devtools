#pragma once
#include <ui/common/editors/Reflection.h>

class ObjectInspection;
class ObjectInspector : public hh::fnd::BaseObject {
    ObjectInspection& objectInspection;

    template<typename R>
    void RenderModeParameterInspector(const char* label, R (&parameters)[4]) {
        if (ImGui::TreeNode(label)) {
            if (ImGui::TreeNode("Normal")) {
                Editor("Parameters", *parameters[0]);
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("Water")) {
                Editor("Parameters", *parameters[1]);
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("Cyberspace Forward View")) {
                Editor("Parameters", *parameters[2]);
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("Cyberspace Side View")) {
                Editor("Parameters", *parameters[3]);
                ImGui::TreePop();
            }
            ImGui::TreePop();
        }
    }

    void RenderComponentInspector(hh::game::GOComponent& component);
	void RenderGOCTransformInspector(hh::game::GOCTransform& component);
	void RenderGOCPlayerParameterInspector(app::player::GOCPlayerParameter& component);
    void RenderGOCPlayerKinematicParamsInspector(app::player::GOCPlayerKinematicParams& component);
    void RenderGOCPlayerBlackboardInspector(app::player::GOCPlayerBlackboard& component);
    void RenderGOCColliderInspector(hh::physics::GOCCollider& component);
    void RenderGOCSphereColliderInspector(hh::physics::GOCSphereCollider& component);
    void RenderGOCBoxColliderInspector(hh::physics::GOCBoxCollider& component);
    void RenderGOCCapsuleColliderInspector(hh::physics::GOCCapsuleCollider& component);
    void RenderGOCCylinderColliderInspector(hh::physics::GOCCylinderCollider& component);
    void RenderGOCAnimatorInspector(hh::anim::GOCAnimator& component);
    void RenderGOCEventInspector(app::game::GOCEvent& component);
    void RenderGOCEventCollisionInspector(app::game::GOCEventCollision& component);
    void RenderGOCCameraInspector(app_cmn::camera::GOCCamera& component);
	void RenderUnknownComponentInspector(hh::game::GOComponent& component);

    void RenderBlackboardAmyInspector(app::player::BlackboardAmy& blackboard);
    void RenderBlackboardSpeedInspector(app::player::BlackboardSpeed& blackboard);
    void RenderBlackboardItemInspector(app::player::BlackboardItem& blackboard);
    void RenderBlackboardBattleInspector(app::player::BlackboardBattle& blackboard);
    void RenderBlackboardStatusInspector(app::player::BlackboardStatus& blackboard);
    void RenderBlackboardTailsInspector(app::player::BlackboardTails& blackboard);

public:
    ObjectInspector(csl::fnd::IAllocator* allocator, ObjectInspection& objectInspection);
    void Render();
};
