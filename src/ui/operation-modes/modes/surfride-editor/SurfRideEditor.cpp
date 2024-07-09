#include "SurfRideEditor.h"
#include <utilities/math/MathUtils.h>
#include <ui/common/viewers/Basic.h>
#include <ui/operation-modes/behaviors/Selection.h>
#include <ui/operation-modes/behaviors/SelectionAabb.h>
#include <ui/operation-modes/behaviors/SelectionVisual.h>
#include <ui/operation-modes/behaviors/SelectionMousePicking.h>
#include <ui/operation-modes/behaviors/MousePicking.h>
#include <ui/operation-modes/behaviors/ScreenSpaceManipulation.h>
#include <ui/operation-modes/BehaviorContext.h>
#include "ProjectTree.h"
#include "Timeline.h"
#include "ElementInspector.h"

namespace ui::operation_modes::modes::surfride_editor
{
	using namespace SurfRide;

	class SelectionAabbBehaviorContext : public BehaviorContext<Context, SelectionAabbBehavior<SurfRideElement>> {
		using BehaviorContext::BehaviorContext;
		virtual bool CalculateAabb(const csl::ut::MoveArray<SurfRideElement>& elements, csl::geom::Aabb& aabb) override
		{
			aabb.min = { INFINITY, INFINITY, INFINITY };
			aabb.max = { -INFINITY, -INFINITY, -INFINITY };

			bool updated{};

			for (auto& el : elements) {
				if (el.type != SurfRideElement::Type::CAST)
					continue;

				updated |= UpdateAabb(el.cast, aabb);
			}

			return updated;
		}

		void UpdateAabb(const Eigen::Transform<float, 3, Eigen::Projective>& transform, const SurfRide::Vector2& size, csl::geom::Aabb& aabb)
		{
			aabb.AddPoint((transform * Eigen::Vector3f{ -size.x() / 2.0f, size.y() / 2.0f, 0.0f }.homogeneous()).hnormalized());
			aabb.AddPoint((transform * Eigen::Vector3f{ size.x() / 2.0f, size.y() / 2.0f, 0.0f }.homogeneous()).hnormalized());
			aabb.AddPoint((transform * Eigen::Vector3f{ size.x() / 2.0f, -size.y() / 2.0f, 0.0f }.homogeneous()).hnormalized());
			aabb.AddPoint((transform * Eigen::Vector3f{ -size.x() / 2.0f, -size.y() / 2.0f, 0.0f }.homogeneous()).hnormalized());
		}

		bool UpdateAabb(const SurfRide::Cast* cast, csl::geom::Aabb& aabb)
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
	};

	class SelectionTransformationBehaviorContext : public BehaviorContext<Context, SelectionTransformationBehavior<SurfRideElement, true>> {
		using BehaviorContext::BehaviorContext;
		virtual bool HasTransform(SurfRideElement element) override { return element.type == SurfRideElement::Type::CAST; }
		virtual bool IsRoot(SurfRideElement element) override { return element.cast->parentCast == nullptr; }
		virtual SurfRideElement GetParent(SurfRideElement element) override { return { SurfRideElement::Type::CAST, element.cast->parentCast }; }
		virtual Eigen::Projective3f GetSelectionSpaceTransform(SurfRideElement element) override { return context.focusedScene->camera.projectionMatrix * context.focusedScene->camera.viewMatrix * element.cast->transform->transformationMatrix; }
		virtual void SetSelectionSpaceTransform(SurfRideElement element, const Eigen::Projective3f& transform) override {
			auto* cast = element.cast;

			Eigen::Projective3f parentSelectionSpaceTransform = cast->parentCast == nullptr ? Eigen::Projective3f::Identity() : Eigen::Projective3f{ context.focusedScene->camera.projectionMatrix * context.focusedScene->camera.viewMatrix * cast->parentCast->transform->transformationMatrix };

			cast->transform->position = (parentSelectionSpaceTransform.inverse() * transform * Eigen::Vector3f::Zero().homogeneous()).hnormalized();
			cast->transform->dirtyFlag.flags.m_dummy |= cast->transform->dirtyFlag.transformAny.m_dummy;
		}
	};

	class MousePicking3DRecursiveRaycastBehaviorContext : public BehaviorContext<Context, MousePicking3DRecursiveRaycastBehavior<SurfRideElement, Ray3f>> {
		using BehaviorContext::BehaviorContext;
		virtual const char* GetObjectName(SurfRideElement object) override { return object.GetName(); }
		virtual bool IsSelectable(SurfRideElement object) override { return object.type != SurfRideElement::Type::CAST ? false : object.cast->transform->display; }
		virtual bool Intersects(SurfRideElement object, const Ray3f& ray) override { return Intersects(object.cast, ray); }
		virtual void GetRootObjects(csl::ut::MoveArray<SurfRideElement>& rootObjects) override {
			for (auto* layer : context.focusedScene->layers)
				if (!(layer->flags & 0x100))
					for (auto* rootCast : layer->topLevelCasts)
						rootObjects.push_back({ SurfRideElement::Type::CAST, rootCast });
		}

		virtual void GetChildren(SurfRideElement object, csl::ut::MoveArray<SurfRideElement>& children) override {
			for (auto* child : object.cast->children)
				children.push_back({ SurfRideElement::Type::CAST, child });
		}

		virtual void GetFrustumResults(const Frustum& frustum, csl::ut::MoveArray<SurfRideElement>& results) override {
			for (auto* layer : context.focusedScene->layers)
				if (!(layer->flags & 0x100))
					for (auto* cast : layer->casts)
						if (cast->transform->display && frustum.Test(cast->transform->transformationMatrix * Eigen::Vector3f::Zero()))
							results.push_back({ SurfRideElement::Type::CAST, cast });
		}

		bool Intersects(const Cast* cast, const Ray3f& ray) {
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

		bool Intersects(const Transform& transform, const Vector2& size, const Ray3f& ray) {
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
	};

	class MousePickingCameraProvider : public BehaviorContext<Context, MousePicking3DRecursiveRaycastBehavior<SurfRideElement, Ray3f>, MousePicking3DCameraProvider> {
		using BehaviorContext::BehaviorContext;
		virtual bool GetCameraMatrix(Eigen::Matrix4f& inverseCameraMatrix) override {
			if (!context.focusedScene)
				return false;

			inverseCameraMatrix = context.focusedScene->camera.viewMatrix.inverse() * context.focusedScene->camera.projectionMatrix.inverse();
			return true;
		}
	};

	SurfRideEditor::SurfRideEditor(csl::fnd::IAllocator* allocator) : OperationMode{ allocator }
	{
		AddPanel<ProjectTree>();
		AddPanel<Timeline>();
		AddPanel<ElementInspector>();
		AddBehavior<SelectionBehavior<SurfRideElement>>();
		AddBehavior<SelectionAabbBehavior<SurfRideElement>, SelectionAabbBehaviorContext>();
		AddBehavior<SelectionTransformationBehavior<SurfRideElement, true>, SelectionTransformationBehaviorContext>();
		AddBehavior<SelectionVisual2DBehavior>();
		AddBehavior<ScreenSpaceManipulationBehavior>();
		AddBehavior<MousePicking3DRecursiveRaycastBehavior<SurfRideElement, Ray3f>, MousePicking3DRecursiveRaycastBehaviorContext, MousePickingCameraProvider>();
		AddBehavior<SelectionMousePickingBehavior<SurfRideElement>>();
	}
}
