#pragma once
#include "Context.h"
#include <utilities/math/Ray.h>
#include <utilities/math/Frustum.h>
#include <ui/operation-modes/OperationMode.h>
#include <ui/operation-modes/behaviors/ForwardDeclarations.h>
#include <debug-rendering/GOCVisualDebugDrawRenderer.h>
#include <debug-rendering/visuals/FxColCollisionShape.h>
#include <debug-rendering/visuals/KdTree.h>

namespace ui::operation_modes::modes::fxcol_editor {
	using namespace app::gfx;

	template<> struct MousePicking3DRecursiveRaycastBehaviorTraits<Context> : BehaviorTraitsImpl<Context> {
		using ObjectType = FxColCollisionShape*;
		using LocationType = csl::math::Vector3;

		using BehaviorTraitsImpl::BehaviorTraitsImpl;
		const char* GetObjectName(FxColCollisionShape* object) { return object->name; }
		bool IsSelectable(FxColCollisionShape* object) { return true; }
		bool Intersects(FxColCollisionShape* object, const Ray3f& ray) { return false; }
		void GetRootObjects(csl::ut::MoveArray<FxColCollisionShape*>& rootObjects) {}
		void GetChildren(FxColCollisionShape* object, csl::ut::MoveArray<FxColCollisionShape*>& children) {}
		void GetFrustumResults(const Frustum& frustum, csl::ut::MoveArray<FxColCollisionShape*>& results) {
			if (auto* fxColManager = hh::game::GameManager::GetInstance()->GetService<FxColManager>())
				if (fxColManager->resource != nullptr)
					for (size_t i = 0; i < fxColManager->resource->fxColData->collisionShapeCount; i++) {
						auto& shape = fxColManager->resource->fxColData->collisionShapes[i];
						if (frustum.Test(shape.position))
							results.push_back(&shape);
					}
		}
	};
}

#include <ui/operation-modes/behaviors/prefabs/MousePicking3DRecursiveRaycast.h>

namespace ui::operation_modes::modes::fxcol_editor {
	template<> struct MousePicking3DBehaviorTraits<Context> : MousePicking3DRecursiveRaycastMousePicking3DBehaviorTraitsImpl<Context> {
		using MousePicking3DRecursiveRaycastMousePicking3DBehaviorTraitsImpl::MousePicking3DRecursiveRaycastMousePicking3DBehaviorTraitsImpl;
	};
}

#include <ui/operation-modes/behaviors/prefabs/MousePicking3DCameraManagerCameraProvider.h>

namespace ui::operation_modes::modes::fxcol_editor {
	template<> struct MousePicking3DCameraProvider<Context> : MousePicking3DCameraManagerCameraProvider<Context> {
		using MousePicking3DCameraManagerCameraProvider::MousePicking3DCameraManagerCameraProvider;
	};
}

#include <ui/operation-modes/behaviors/prefabs/MousePicking3D.h>

namespace ui::operation_modes::modes::fxcol_editor {
	template<> struct MousePickingBehaviorTraits<Context> : MousePicking3DMousePickingBehaviorTraitsImpl<Context> {
		using MousePicking3DMousePickingBehaviorTraitsImpl::MousePicking3DMousePickingBehaviorTraitsImpl;
	};
}

namespace ui::operation_modes::modes::fxcol_editor {
	using namespace app::gfx;

	template<> struct SelectionBehaviorTraits<Context> : BehaviorTraitsImpl<Context> {
		using ObjectType = FxColCollisionShape*;
	};

	template<> struct SelectionTransformationBehaviorTraits<Context> : BehaviorTraitsImpl<Context> {
		using BehaviorTraitsImpl::BehaviorTraitsImpl;
		static constexpr bool Projective = false;
		bool HasTransform(FxColCollisionShape* obj) { return true; }
		bool IsRoot(FxColCollisionShape* obj) { return true; }
		FxColCollisionShape* GetParent(FxColCollisionShape* obj) { return nullptr; }
		Eigen::Affine3f GetSelectionSpaceTransform(FxColCollisionShape* obj) const { return Eigen::Affine3f{ Eigen::Translation3f{ obj->position } *obj->rotation }; }
		void SetSelectionSpaceTransform(FxColCollisionShape* obj, const Eigen::Affine3f& transform) {
			obj->position = csl::math::Position{ transform.translation() };
			obj->rotation = csl::math::Rotation{ transform.rotation() };
		}
	};

	template<> struct SelectionAabbBehaviorTraits<Context> : BehaviorTraitsImpl<Context> {
		using BehaviorTraitsImpl::BehaviorTraitsImpl;
		bool CalculateAabb(const csl::ut::MoveArray<FxColCollisionShape*>& objects, csl::geom::Aabb& aabb) {
			aabb = csl::geom::Aabb{ { INFINITY, INFINITY, INFINITY }, { -INFINITY, -INFINITY, -INFINITY } };

			if (objects.size() == 0)
				return false;

			for (auto* shape : objects) {
				switch (shape->shape) {
				case FxColCollisionShape::Shape::SPHERE: {
					auto& extents = shape->extents.sphere;
					aabb.AddPoint(shape->position + csl::math::Vector3{ extents.radius, extents.radius, extents.radius });
					aabb.AddPoint(shape->position - csl::math::Vector3{ extents.radius, extents.radius, extents.radius });
					break;
				}
				case FxColCollisionShape::Shape::CYLINDER: {
					auto& extents = shape->extents.cylinder;
					AddOBB(aabb, shape->position, shape->rotation, { extents.radius, extents.halfHeight, extents.radius });
					break;
				}
				case FxColCollisionShape::Shape::ANISOTROPIC_OBB: {
					auto& extents = shape->extents.anisotropicObb;
					AddOBB(aabb, shape->position, shape->rotation, { extents.width / 2, extents.height / 2, extents.depth / 2 });
					break;
				}
				case FxColCollisionShape::Shape::ISOTROPIC_OBB: {
					auto& extents = shape->extents.isotropicObb;
					AddOBB(aabb, shape->position, shape->rotation, { extents.width / 2, extents.height / 2, extents.depth / 2 });
					break;
				}
				}
			}

			return true;
		}

		void AddOBB(csl::geom::Aabb& aabb, const csl::math::Position& position, const csl::math::Rotation& rotation, const csl::math::Vector3& halfExtents) {
			Eigen::Affine3f transform{ Eigen::Translation3f{ position } *rotation };

			aabb.AddPoint(transform * -halfExtents);
			aabb.AddPoint(transform * halfExtents);
			aabb.AddPoint(transform * -csl::math::Vector3{ halfExtents.x(), halfExtents.y(), -halfExtents.z() });
			aabb.AddPoint(transform * csl::math::Vector3{ halfExtents.x(), halfExtents.y(), -halfExtents.z() });
			aabb.AddPoint(transform * -csl::math::Vector3{ halfExtents.x(), -halfExtents.y(), halfExtents.z() });
			aabb.AddPoint(transform * csl::math::Vector3{ halfExtents.x(), -halfExtents.y(), halfExtents.z() });
			aabb.AddPoint(transform * -csl::math::Vector3{ halfExtents.x(), -halfExtents.y(), -halfExtents.z() });
			aabb.AddPoint(transform * csl::math::Vector3{ halfExtents.x(), -halfExtents.y(), -halfExtents.z() });
		}
	};

	template<> struct GizmoBehaviorTraits<Context> : BehaviorTraitsImpl<Context> {
		using BehaviorTraitsImpl::BehaviorTraitsImpl;
		static constexpr bool allowTranslate = true;
		static constexpr bool allowRotate = true;
		static constexpr bool allowScale = false;
	};
}

#include <ui/operation-modes/behaviors/Selection.h>
#include <ui/operation-modes/behaviors/SelectionAabb.h>
#include <ui/operation-modes/behaviors/SelectionTransformation.h>
#include <ui/operation-modes/behaviors/Gizmo.h>
#include <ui/operation-modes/behaviors/MousePicking.h>
#include <ui/operation-modes/behaviors/SelectionMousePicking.h>

namespace ui::operation_modes::modes::fxcol_editor {
	class RenderFxColBehavior : public OperationModeBehavior, public DebugRenderable {
		bool renderBoundingVolumes{ true };
		bool renderCollisionShapes{ true };
		bool renderKdTree{ false };
	public:
		using OperationModeBehavior::OperationModeBehavior;

		static constexpr unsigned int id = 13;
		unsigned int GetId() { return id; }

		void RenderDebugVisuals(hh::gfnd::DrawContext& ctx) {
			auto* fxColMgr = hh::game::GameManager::GetInstance()->GetService<app::gfx::FxColManager>();

			if (fxColMgr == nullptr || fxColMgr->resource == nullptr)
				return;

			if (renderKdTree)
				RenderDebugVisual(ctx, fxColMgr->collisionTree);

			auto& selection = operationMode.GetBehavior<SelectionBehavior<Context>>()->GetSelection();

			if (renderBoundingVolumes)
				for (size_t i = 0; i < fxColMgr->resource->fxColData->boundingVolumeCount; i++)
					ctx.DrawAABB(fxColMgr->resource->fxColData->boundingVolumes[i].aabbMin, fxColMgr->resource->fxColData->boundingVolumes[i].aabbMax, { 0, 0, 255, 255 });

			if (renderCollisionShapes)
				for (size_t i = 0; i < fxColMgr->resource->fxColData->collisionShapeCount; i++)
					RenderDebugVisual(ctx, fxColMgr->resource->fxColData->collisionShapes[i]);
		}

		void Render() {
			if (ImGui::Begin("Main menu")) {
				ImGui::SameLine();
				ImGui::Checkbox("Render bounding volumes", &renderBoundingVolumes);
				ImGui::SameLine();
				ImGui::Checkbox("Render collision shapes", &renderCollisionShapes);
				ImGui::SameLine();
				ImGui::Checkbox("Render bounding volumes K/D tree", &renderKdTree);
			}
			ImGui::End();
		}
	};
}
