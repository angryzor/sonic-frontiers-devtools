#pragma once
#include <utilities/math/Ray.h>
#include <utilities/math/Frustum.h>
#include <utilities/math/MathUtils.h>
#include <ui/operation-modes/OperationMode.h>
#include <ui/operation-modes/behaviors/ForwardDeclarations.h>
#include "SurfRideElement.h"
#include "Context.h"

namespace ui::operation_modes::modes::surfride_editor {
	template<> struct MousePicking3DRecursiveRaycastBehaviorTraits<Context> : BehaviorTraitsImpl<Context> {
		using BehaviorTraitsImpl::BehaviorTraitsImpl;
		using ObjectType = SurfRideElement;
		using LocationType = Ray3f;
		const char* GetObjectName(SurfRideElement object) {
			switch (object.type) {
			case SurfRideElement::Type::SCENE: return context.FindScene(object.id)->name;
			case SurfRideElement::Type::CAMERA: return context.FindCamera(object.id)->name;
			case SurfRideElement::Type::LAYER: return context.FindLayer(object.id)->name;
			case SurfRideElement::Type::CAST: return context.FindCast(object.id)->name;
			}
		}
		bool IsSelectable(SurfRideElement object) { return object.type != SurfRideElement::Type::CAST ? false : context.FindRuntimeCast(object.id)->transform->display; }
		bool Intersects(SurfRideElement object, const Ray3f& ray) { return Intersects(*context.FindRuntimeCast(object.id), ray); }
		void GetRootObjects(csl::ut::MoveArray<SurfRideElement>& rootObjects) {
			for (auto* layer : context.FindRuntimeScene(context.focusedScene->id)->GetLayers())
				if (!(layer->flags & 0x100))
					for (auto* rootCast : layer->GetCasts())
						rootObjects.push_back(*rootCast->castData);
		}

		void GetChildren(SurfRideElement object, csl::ut::MoveArray<SurfRideElement>& children) {
			for (auto* child : context.FindRuntimeCast(object.id)->GetChildren())
				children.push_back(*child->castData);
		}

		void GetFrustumResults(const Frustum& frustum, csl::ut::MoveArray<SurfRideElement>& results) {
			for (auto* layer : context.FindRuntimeScene(context.focusedScene->id)->GetLayers())
				if (!(layer->flags & 0x100))
					for (auto* cast : layer->GetCasts())
						GetFrustumResultsForCast(cast, frustum, results);
		}

		void GetFrustumResultsForCast(SurfRide::Cast* cast, const Frustum& frustum, csl::ut::MoveArray<SurfRideElement>& results) {
			if (cast->transform->display && frustum.Test(cast->transform->transformationMatrix * Eigen::Vector3f::Zero()))
				results.push_back(*cast->castData);

			for (auto* child : cast->GetChildren())
				GetFrustumResultsForCast(child, frustum, results);
		}

		bool Intersects(const SurfRide::Cast& cast, const Ray3f& ray) {
			switch (static_cast<ucsl::resources::swif::v6::SRS_CASTNODE::Type>(cast.flags & 0xF)) {
			case ucsl::resources::swif::v6::SRS_CASTNODE::Type::IMAGE: return Intersects(*cast.transform, static_cast<const SurfRide::ImageCast&>(cast).size, ray);
			case ucsl::resources::swif::v6::SRS_CASTNODE::Type::SLICE: return Intersects(*cast.transform, static_cast<const SurfRide::SliceCast&>(cast).size, ray);
			default: return false;
			}
		}

#ifdef DEVTOOLS_TARGET_SDK_wars
		bool Intersects(const SurfRide::Transform3D& transform, const ucsl::resources::swif::v6::Vector2& size, const Ray3f& ray) {
#else
		bool Intersects(const SurfRide::Transform& transform, const ucsl::resources::swif::v6::Vector2& size, const Ray3f& ray) {
#endif
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
}

#include <ui/operation-modes/behaviors/prefabs/MousePicking3DRecursiveRaycast.h>

namespace ui::operation_modes::modes::surfride_editor {
	template<> struct MousePicking3DBehaviorTraits<Context> : MousePicking3DRecursiveRaycastMousePicking3DBehaviorTraitsImpl<Context> {
		using MousePicking3DRecursiveRaycastMousePicking3DBehaviorTraitsImpl::MousePicking3DRecursiveRaycastMousePicking3DBehaviorTraitsImpl;
	};
}

namespace ui::operation_modes::modes::surfride_editor {
	template<> struct MousePicking3DCameraProvider<Context> : BehaviorTraitsImpl<Context> {
		using BehaviorTraitsImpl::BehaviorTraitsImpl;
		bool GetCameraMatrix(Eigen::Projective3f& inverseCameraMatrix) {
			if (!context.focusedScene)
				return false;

			inverseCameraMatrix = context.GetFocusedSceneCameraMatrix().inverse();
			return true;
		}
	};
}

#include <ui/operation-modes/behaviors/prefabs/MousePicking3D.h>

namespace ui::operation_modes::modes::surfride_editor {
	template<> struct MousePickingBehaviorTraits<Context> : MousePicking3DMousePickingBehaviorTraitsImpl<Context> {
		using MousePicking3DMousePickingBehaviorTraitsImpl::MousePicking3DMousePickingBehaviorTraitsImpl;
	};
}

namespace ui::operation_modes::modes::surfride_editor
{
	template<> struct SelectionBehaviorTraits<Context> {
		using ObjectType = SurfRideElement;
	};

	template<> struct SelectionAabbBehaviorTraits<Context> : BehaviorTraitsImpl<Context> {
		using BehaviorTraitsImpl::BehaviorTraitsImpl;
		bool CalculateAabb(const csl::ut::MoveArray<SurfRideElement>& elements, csl::geom::Aabb& aabb)
		{
			aabb.min = { INFINITY, INFINITY, INFINITY };
			aabb.max = { -INFINITY, -INFINITY, -INFINITY };

			bool updated{};

			for (auto& el : elements) {
				if (el.type != SurfRideElement::Type::CAST)
					continue;

				updated |= UpdateAabb(context.FindRuntimeCast(el.id), aabb);
			}

			return updated;
		}

		void UpdateAabb(const Eigen::Transform<float, 3, Eigen::Projective>& transform, const ucsl::resources::swif::v6::Vector2& size, csl::geom::Aabb& aabb)
		{
			aabb.AddPoint((transform * Eigen::Vector3f{ -size.x() / 2.0f, size.y() / 2.0f, 0.0f }.homogeneous()).hnormalized());
			aabb.AddPoint((transform * Eigen::Vector3f{ size.x() / 2.0f, size.y() / 2.0f, 0.0f }.homogeneous()).hnormalized());
			aabb.AddPoint((transform * Eigen::Vector3f{ size.x() / 2.0f, -size.y() / 2.0f, 0.0f }.homogeneous()).hnormalized());
			aabb.AddPoint((transform * Eigen::Vector3f{ -size.x() / 2.0f, -size.y() / 2.0f, 0.0f }.homogeneous()).hnormalized());
		}

		bool UpdateAabb(const SurfRide::Cast* cast, csl::geom::Aabb& aabb)
		{
			bool updated{};

			for (auto* child : cast->GetChildren())
				updated |= UpdateAabb(child, aabb);

			switch (static_cast<ucsl::resources::swif::v6::SRS_CASTNODE::Type>(cast->flags & 0xF)) {
			case ucsl::resources::swif::v6::SRS_CASTNODE::Type::IMAGE: UpdateAabb(context.GetFullCastTransform(cast), static_cast<const SurfRide::ImageCast*>(cast)->size, aabb); updated |= true; break;
			case ucsl::resources::swif::v6::SRS_CASTNODE::Type::SLICE: UpdateAabb(context.GetFullCastTransform(cast), static_cast<const SurfRide::SliceCast*>(cast)->size, aabb); updated |= true; break;
			}

			return updated;
		}
	};

	template<> struct SelectionTransformationBehaviorTraits<Context> : BehaviorTraitsImpl<Context> {
		using BehaviorTraitsImpl::BehaviorTraitsImpl;
		static constexpr bool Projective = true;
		bool HasTransform(SurfRideElement element) { return element.type == SurfRideElement::Type::CAST; }
		bool IsRoot(SurfRideElement element) { return context.FindRuntimeCast(element.id)->parentCast == nullptr; }
		SurfRideElement GetParent(SurfRideElement element) { return *context.FindRuntimeCast(element.id)->parentCast->castData; }
		Eigen::Projective3f GetSelectionSpaceTransform(SurfRideElement element) const { return context.GetFullCastTransform(context.FindRuntimeCast(element.id)); }
		void SetSelectionSpaceTransform(SurfRideElement element, const Eigen::Projective3f& transform) {
#ifdef DEVTOOLS_TARGET_SDK_wars
			auto* cast = static_cast<SurfRide::Cast3D*>(context.FindRuntimeCast(element.id));
#else
			auto* cast = context.FindRuntimeCast(element.id);
#endif

			Eigen::Projective3f parentSelectionSpaceTransform = cast->parentCast == nullptr ? Eigen::Projective3f::Identity() : context.GetFullCastTransform(cast->parentCast);
			csl::math::Vector3 newPos = (parentSelectionSpaceTransform.inverse() * transform * Eigen::Vector3f::Zero().homogeneous()).hnormalized();

			size_t castIndex = (ucsl::resources::swif::v6::SRS_CASTNODE*)cast->castData - cast->layer->layerData->casts;

			if (cast->layer->flags.test(SurfRide::Layer::Flag::IS_3D))
				cast->layer->layerData->transforms.transforms3d[castIndex].position = newPos;
			else
				cast->layer->layerData->transforms.transforms2d[castIndex].position = { newPos.x(), newPos.y() };

			context.ApplyTransformChange(*cast->castData);
		}
	};

	template<> struct ScreenSpaceManipulationBehaviorTraits<Context> : BehaviorTraitsImpl<Context> {
		Eigen::Vector3f ImGuiToSelectionSpace(const ImVec2 vec) {
			auto res = ImGuiCoordsToNDC(vec);

			return { res.x(), res.y(), 0.0f };
		}
	};
}

#include <ui/operation-modes/behaviors/Selection.h>
#include <ui/operation-modes/behaviors/SelectionAabb.h>
#include <ui/operation-modes/behaviors/SelectionVisual.h>
#include <ui/operation-modes/behaviors/SelectionMousePicking.h>
#include <ui/operation-modes/behaviors/SelectionTransformation.h>
#include <ui/operation-modes/behaviors/MousePicking.h>
#include <ui/operation-modes/behaviors/ScreenSpaceManipulation.h>
#include <ui/operation-modes/behaviors/Delete.h>
