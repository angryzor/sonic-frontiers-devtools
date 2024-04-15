#pragma once

class ObjectInspection;
class ObjectInspector : public hh::fnd::BaseObject {
    ObjectInspection& objectInspection;
    void RenderComponentInspector(hh::game::GOComponent& component);
	void RenderGOCTransformInspector(hh::game::GOCTransform& component);
	void RenderGOCPlayerParameterInspector(app::player::GOCPlayerParameter& component);
    void RenderGOCColliderInspector(hh::physics::GOCCollider& component);
    void RenderGOCSphereColliderInspector(hh::physics::GOCSphereCollider& component);
    void RenderGOCBoxColliderInspector(hh::physics::GOCBoxCollider& component);
    void RenderGOCCapsuleColliderInspector(hh::physics::GOCCapsuleCollider& component);
    void RenderGOCCylinderColliderInspector(hh::physics::GOCCylinderCollider& component);
    void RenderGOCAnimatorInspector(hh::anim::GOCAnimator& component);
	void RenderUnknownComponentInspector(hh::game::GOComponent& component);

public:
    ObjectInspector(csl::fnd::IAllocator* allocator, ObjectInspection& objectInspection);
    void Render();
};
