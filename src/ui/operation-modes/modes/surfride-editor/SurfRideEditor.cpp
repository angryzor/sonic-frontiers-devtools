#include "SurfRideEditor.h"
#include <utilities/math/MathUtils.h>
#include <ui/common/viewers/Basic.h>
#include <ui/operation-modes/behaviors/Selection.h>
#include <ui/operation-modes/behaviors/SelectionAabb.h>
#include <ui/operation-modes/behaviors/SelectionVisual.h>
#include <ui/operation-modes/behaviors/SelectionMousePicking.h>
#include <ui/operation-modes/behaviors/MousePicking.h>
#include <ui/operation-modes/behaviors/ScreenSpaceManipulation.h>
#include "Selection.h"

using namespace SurfRide;

SurfRideEditor::SurfRideEditor(csl::fnd::IAllocator* allocator) : OperationMode{ allocator }, projectTree{ allocator, *this }, timeline{ allocator, *this }, elementInspector{ allocator, *this }
{
    AddBehavior<SelectionBehavior<SurfRideSelection>>();
    AddBehavior<SelectionAabbBehavior<SurfRideSelection>>(*this);
	AddBehavior<SelectionTransformationBehavior<SurfRideSelection, true>>(*this);
    AddBehavior<SelectionVisual2DBehavior>();
	AddBehavior<ScreenSpaceManipulationBehavior>();
    AddBehavior<MousePicking3DRecursiveRaycastBehavior<SurfRideSelection, Ray3f>>(*this, *this);
    AddBehavior<SelectionMousePickingBehavior<SurfRideSelection>>();
}

void SurfRideEditor::Render()
{
	projectTree.Render();
	timeline.Render();
	elementInspector.Render();
}

bool SurfRideEditor::HasTransform(SurfRideSelection element)
{
	return element.type == SurfRideSelection::Type::CAST;
}

bool SurfRideEditor::IsRoot(SurfRideSelection element)
{
	return element.cast->parentCast == nullptr;
}

SurfRideSelection SurfRideEditor::GetParent(SurfRideSelection element)
{
	return { SurfRideSelection::Type::CAST, element.cast->parentCast };
}

Eigen::Projective3f SurfRideEditor::GetSelectionSpaceTransform(SurfRideSelection element)
{
	return focusedScene->camera.projectionMatrix * focusedScene->camera.viewMatrix * element.cast->transform->transformationMatrix;
}

void SurfRideEditor::SetSelectionSpaceTransform(SurfRideSelection element, const Eigen::Projective3f& transform)
{
	auto* cast = element.cast;

	Eigen::Projective3f parentSelectionSpaceTransform = cast->parentCast == nullptr ? Eigen::Projective3f::Identity() : Eigen::Projective3f{ focusedScene->camera.projectionMatrix * focusedScene->camera.viewMatrix * cast->parentCast->transform->transformationMatrix };

	cast->transform->position = (parentSelectionSpaceTransform.inverse() * transform * Eigen::Vector3f::Zero().homogeneous()).hnormalized();
	cast->transform->dirtyFlag.flags.m_dummy |= cast->transform->dirtyFlag.transformAny.m_dummy;
}

bool SurfRideEditor::GetCameraMatrix(Eigen::Matrix4f& inverseCameraMatrix)
{
	if (!focusedScene)
		return false;

	inverseCameraMatrix = focusedScene->camera.viewMatrix.inverse() * focusedScene->camera.projectionMatrix.inverse();
	return true;
}

void SurfRideEditor::GetRootObjects(csl::ut::MoveArray<SurfRideSelection>& rootObjects)
{
	for (auto* layer : focusedScene->layers)
		if (!(layer->flags & 0x100))
			for (auto* rootCast : layer->topLevelCasts)
				rootObjects.push_back({ SurfRideSelection::Type::CAST, rootCast });
}

void SurfRideEditor::GetChildren(SurfRideSelection object, csl::ut::MoveArray<SurfRideSelection>& children)
{
	for (auto* child : object.cast->children)
		children.push_back({ SurfRideSelection::Type::CAST, child });
}

bool SurfRideEditor::IsSelectable(SurfRideSelection object)
{
	return object.type != SurfRideSelection::Type::CAST ? false : object.cast->transform->display;
}

bool SurfRideEditor::Intersects(SurfRideSelection object, const Ray3f& ray)
{
	return Intersects(object.cast, ray);
}

void SurfRideEditor::GetFrustumResults(const Frustum& frustum, csl::ut::MoveArray<SurfRideSelection>& results)
{
	for (auto* layer : focusedScene->layers)
		if (!(layer->flags & 0x100))
			for (auto* cast : layer->casts)
				if (cast->transform->display && frustum.Test(cast->transform->transformationMatrix * Eigen::Vector3f::Zero()))
					results.push_back({ SurfRideSelection::Type::CAST, cast });
}

const char* SurfRideEditor::GetObjectName(SurfRideSelection object)
{
	return object.GetName();
}

bool SurfRideEditor::Intersects(const Cast* cast, const Ray3f& ray)
{
	switch (static_cast<SRS_CASTNODE::Type>(cast->flags & 0xF)) {
	case SRS_CASTNODE::Type::IMAGE:
		if (Intersects(*cast->transform, static_cast<const ImageCast*>(cast)->size, ray))
			return cast;
		break;
	case SRS_CASTNODE::Type::SLICE:
		if (Intersects(*cast->transform, static_cast<const SliceCast*>(cast)->size, ray))
			return cast;
		break;
	}
	return false;
}

bool SurfRideEditor::Intersects(const Transform& transform, const Vector2& size, const Ray3f& ray)
{
	auto halfSize = size / 2.0f;

	Eigen::Affine3f invertedTransformMatrix{ transform.transformationMatrix.inverse() };
	Eigen::ParametrizedLine<float, 3> worldLine{ ray };
	Eigen::ParametrizedLine<float, 3> localLine{ invertedTransformMatrix * ray };
	Eigen::Hyperplane<float, 3> castPlane{ { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f } };

	auto intersectionPoint = localLine.intersectionPoint(castPlane);
	auto botLeft = -halfSize;
	auto topRight = halfSize;

	return botLeft.x() <= intersectionPoint.x() && botLeft.y() <= intersectionPoint.y() && topRight.x() >= intersectionPoint.x() && topRight.y() >= intersectionPoint.y();
}

bool SurfRideEditor::CalculateAabb(const csl::ut::MoveArray<SurfRideSelection>& elements, csl::geom::Aabb& aabb)
{
	aabb.min = { INFINITY, INFINITY, INFINITY };
	aabb.max = { -INFINITY, -INFINITY, -INFINITY };

	bool updated{};

	for (auto& el : elements) {
		if (el.type != SurfRideSelection::Type::CAST)
			continue;

		updated |= UpdateAabb(el.cast, aabb);
	}

	return updated;
}

void SurfRideEditor::UpdateAabb(const Eigen::Transform<float, 3, Eigen::Projective>& transform, const SurfRide::Vector2& size, csl::geom::Aabb& aabb)
{
	aabb.AddPoint((transform * Eigen::Vector3f{ -size.x() / 2.0f, size.y() / 2.0f, 0.0f }.homogeneous()).hnormalized());
	aabb.AddPoint((transform * Eigen::Vector3f{ size.x() / 2.0f, size.y() / 2.0f, 0.0f }.homogeneous()).hnormalized());
	aabb.AddPoint((transform * Eigen::Vector3f{ size.x() / 2.0f, -size.y() / 2.0f, 0.0f }.homogeneous()).hnormalized());
	aabb.AddPoint((transform * Eigen::Vector3f{ -size.x() / 2.0f, -size.y() / 2.0f, 0.0f }.homogeneous()).hnormalized());
}

bool SurfRideEditor::UpdateAabb(const SurfRide::Cast* cast, csl::geom::Aabb& aabb)
{
	bool updated{};

	for (auto* child : cast->children)
		updated |= UpdateAabb(child, aabb);

	switch (static_cast<SRS_CASTNODE::Type>(cast->flags & 0xF)) {
	case SRS_CASTNODE::Type::IMAGE: UpdateAabb(cast->layer->scene->camera.projectionMatrix * cast->layer->scene->camera.viewMatrix * cast->transform->transformationMatrix, static_cast<const ImageCast*>(cast)->size, aabb); updated |= true; break;
	case SRS_CASTNODE::Type::SLICE: UpdateAabb(cast->layer->scene->camera.projectionMatrix * cast->layer->scene->camera.viewMatrix * cast->transform->transformationMatrix, static_cast<const SliceCast*>(cast)->size, aabb); updated |= true; break;
	}

	return updated;
}

