#pragma once
#include <ui/operation-modes/OperationMode.h>
#include <ui/operation-modes/behaviors/SelectionAabb.h>
#include <ui/operation-modes/behaviors/SelectionTransformation.h>
#include <ui/operation-modes/behaviors/MousePicking.h>
#include <ui/operation-modes/behaviors/Delete.h>
#include "ObjectList.h"
#include "ObjectInspector.h"

class ObjectInspection
    : public OperationMode
    , public SelectionAabbBehavior<hh::game::GameObject*>::Operations
    , public SelectionTransformationBehavior<hh::game::GameObject*>::Operations
    , public MousePickingPhysicsBehavior<hh::game::GameObject*>::Operations
    , public DeleteBehavior<hh::game::GameObject*>::Operations
{
    ObjectList objectList{ GetAllocator(), *this };
    ObjectInspector objectInspector{ GetAllocator(), *this };

public:
    ObjectInspection(csl::fnd::IAllocator* allocator);
    virtual void Render() override;

    virtual bool HasTransform(hh::game::GameObject* obj) override;
    virtual bool IsRoot(hh::game::GameObject* obj) override;
    virtual hh::game::GameObject* GetParent(hh::game::GameObject* obj) override;
    virtual Eigen::Affine3f GetSelectionSpaceTransform(hh::game::GameObject* obj) override;
    virtual void SetSelectionSpaceTransform(hh::game::GameObject* obj, const Eigen::Affine3f& transform) override;

    virtual void DeleteObjects(const csl::ut::MoveArray<hh::game::GameObject*>& obj) override;

    virtual hh::game::GameObject* GetObjectForGameObject(hh::game::GameObject* obj) override;
    virtual bool IsSelectable(hh::game::GameObject* obj) override;
    virtual const char* GetObjectName(hh::game::GameObject* obj) override;
    virtual void GetFrustumResults(const Frustum& frustum, csl::ut::MoveArray<hh::game::GameObject*>& results) override;

    virtual bool CalculateAabb(const csl::ut::MoveArray<hh::game::GameObject*>& objects, csl::geom::Aabb& aabb) override;
};
