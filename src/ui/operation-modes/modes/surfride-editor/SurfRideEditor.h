#pragma once

#include <ui/operation-modes/OperationMode.h>
#include <ui/operation-modes/behaviors/SelectionAabb.h>
#include <ui/operation-modes/behaviors/SelectionTransformation.h>
#include <ui/operation-modes/behaviors/MousePicking.h>
#include <ui/operation-modes/behaviors/Delete.h>
#include <utilities/math/Ray.h>
#include <utilities/math/Frustum.h>
#include "ProjectTree.h"
#include "Timeline.h"
#include "ElementInspector.h"
#include "Selection.h"

class SurfRideEditor
	: public OperationMode
	, public SelectionTransformationBehavior<SurfRideSelection, true>::Operations
	, public MousePicking3DCameraProvider
	, public MousePicking3DRecursiveRaycastBehavior<SurfRideSelection, Ray3f>::Operations
	, public SelectionAabbBehavior<SurfRideSelection>::Operations
{
public:

private:
    ProjectTree projectTree;
	Timeline timeline;
    ElementInspector elementInspector;

public:
	hh::fnd::Reference<hh::ui::GOCSprite> gocSprite;
	SurfRide::Scene* focusedScene{};

	SurfRideEditor(csl::fnd::IAllocator* allocator);

	virtual void Render() override;

	virtual bool HasTransform(SurfRideSelection elements) override;
	virtual bool IsRoot(SurfRideSelection elements) override;
	virtual SurfRideSelection GetParent(SurfRideSelection elements) override;
	virtual Eigen::Projective3f GetSelectionSpaceTransform(SurfRideSelection elements) override;
	virtual void SetSelectionSpaceTransform(SurfRideSelection elements, const Eigen::Projective3f& transform) override;

	virtual bool GetCameraMatrix(Eigen::Matrix4f& inverseCameraMatrix) override;
	virtual void GetRootObjects(csl::ut::MoveArray<SurfRideSelection>& rootObjects) override;
	virtual void GetChildren(SurfRideSelection object, csl::ut::MoveArray<SurfRideSelection>& children) override;
	virtual bool IsSelectable(SurfRideSelection object) override;
	virtual bool Intersects(SurfRideSelection object, const Ray3f& ray) override;
	virtual void GetFrustumResults(const Frustum& frustum, csl::ut::MoveArray<SurfRideSelection>& results) override;
	virtual const char* GetObjectName(SurfRideSelection object) override;

	virtual bool CalculateAabb(const csl::ut::MoveArray<SurfRideSelection>& elements, csl::geom::Aabb& aabb) override;
	static bool UpdateAabb(const SurfRide::Cast* cast, csl::geom::Aabb& aabb);
	static void UpdateAabb(const Eigen::Transform<float, 3, Eigen::Projective>& transform, const SurfRide::Vector2& size, csl::geom::Aabb& aabb);

	bool Intersects(const SurfRide::Cast* cast, const Ray3f& ray);
	bool Intersects(const SurfRide::Transform& transform, const SurfRide::Vector2& size, const Ray3f& ray);
};
