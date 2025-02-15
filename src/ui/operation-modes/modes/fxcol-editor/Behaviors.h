#pragma once
#include "Context.h"
#include <utilities/math/Ray.h>
#include <utilities/math/Frustum.h>
#include <utilities/BoundingBoxes.h>
#include <ui/Desktop.h>
#include <ui/operation-modes/OperationMode.h>
#include <ui/operation-modes/behaviors/ForwardDeclarations.h>
#include <debug-rendering/DebugRenderable.h>
#include <debug-rendering/visuals/FxColCollisionShapeData.h>
#include <debug-rendering/visuals/KdTree.h>

namespace ui::operation_modes::modes::fxcol_editor {
	using namespace ucsl::resources::fxcol::v1;

	template<> struct MousePicking3DRecursiveRaycastBehaviorTraits<Context> : BehaviorTraitsImpl<Context> {
		using ObjectType = ShapeData*;
		using LocationType = csl::math::Vector3;

		using BehaviorTraitsImpl::BehaviorTraitsImpl;
		const char* GetObjectName(ShapeData* object) { return object->name; }
		bool IsSelectable(ShapeData* object) { return true; }
		bool Intersects(ShapeData* object, const Ray3f& ray) { return false; }
		void GetRootObjects(csl::ut::MoveArray<ShapeData*>& rootObjects) {}
		void GetChildren(ShapeData* object, csl::ut::MoveArray<ShapeData*>& children) {}
		void GetFrustumResults(const Frustum& frustum, csl::ut::MoveArray<ShapeData*>& results) {
			if (auto* fxColManager = hh::game::GameManager::GetInstance()->GetService<app::gfx::FxColManager>())
				if (fxColManager->resource != nullptr)
					for (size_t i = 0; i < fxColManager->resource->fxColData->shapeCount; i++) {
						auto& shape = fxColManager->resource->fxColData->shapes[i];
						if (frustum.Test(shape.position))
							results.push_back(&shape);
					}
		}
	};
}

#include <ui/operation-modes/behaviors/prefabs/MousePicking3DRecursiveRaycastWithPhysicsLocation.h>

namespace ui::operation_modes::modes::fxcol_editor {
	template<> struct MousePicking3DBehaviorTraits<Context> : MousePicking3DRecursiveRaycastWithPhysicsLocationMousePicking3DBehaviorTraitsImpl<Context> {
		using MousePicking3DRecursiveRaycastWithPhysicsLocationMousePicking3DBehaviorTraitsImpl::MousePicking3DRecursiveRaycastWithPhysicsLocationMousePicking3DBehaviorTraitsImpl;
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
		using ObjectType = ShapeData*;
	};

	template<> struct SelectionTransformationBehaviorTraits<Context> : BehaviorTraitsImpl<Context> {
		using BehaviorTraitsImpl::BehaviorTraitsImpl;
		static constexpr bool Projective = false;
		bool HasTransform(ShapeData* obj) { return true; }
		bool IsRoot(ShapeData* obj) { return true; }
		ShapeData* GetParent(ShapeData* obj) { return nullptr; }
		Eigen::Affine3f GetSelectionSpaceTransform(ShapeData* obj) const { return Eigen::Affine3f{ Eigen::Translation3f{ obj->position } *obj->rotation }; }
		void SetSelectionSpaceTransform(ShapeData* obj, const Eigen::Affine3f& transform) {
			obj->position = csl::math::Position{ transform.translation() };
			obj->rotation = csl::math::Rotation{ transform.rotation() };

			context.RecalculateBoundingVolume(context.GetBoundingVolumeOfShape(obj));
		}
	};

	template<> struct SelectionAabbBehaviorTraits<Context> : BehaviorTraitsImpl<Context> {
		using BehaviorTraitsImpl::BehaviorTraitsImpl;
		bool CalculateAabb(const csl::ut::MoveArray<ShapeData*>& objects, csl::geom::Aabb& aabb) {
			aabb = csl::geom::Aabb{ { INFINITY, INFINITY, INFINITY }, { -INFINITY, -INFINITY, -INFINITY } };

			if (objects.size() == 0)
				return false;

			for (auto* shape : objects)
				AddAabb(aabb, Context::CalculateAabb(shape));

			return true;
		}
	};

	template<> struct GizmoBehaviorTraits<Context> : BehaviorTraitsImpl<Context> {
		using BehaviorTraitsImpl::BehaviorTraitsImpl;
		static constexpr bool allowTranslate = true;
		static constexpr bool allowRotate = true;
		static constexpr bool allowScale = false;
	};

	template<> struct PlacementBehaviorTraits<Context> : BehaviorTraitsImpl<Context> {
		using BehaviorTraitsImpl::BehaviorTraitsImpl;
		constexpr static bool is3D = true;
		bool CanPlace() const { return context.placementVolume; }
		ShapeData* PlaceObject(const csl::math::Vector3& location) {
			return context.AddCollisionShape(context.placementVolume, location);
		}
	};

	template<> struct DeleteBehaviorTraits<Context> : BehaviorTraitsImpl<Context> {
		using BehaviorTraitsImpl::BehaviorTraitsImpl;
		void DeleteObjects(const csl::ut::MoveArray<ShapeData*>& objects) {
			for (auto* obj : objects)
				context.RemoveCollisionShape(obj);
		}
	};
}

#include <ui/operation-modes/behaviors/Selection.h>
#include <ui/operation-modes/behaviors/SelectionAabb.h>
#include <ui/operation-modes/behaviors/SelectionTransformation.h>
#include <ui/operation-modes/behaviors/SelectionVisual.h>
#include <ui/operation-modes/behaviors/Gizmo.h>
#include <ui/operation-modes/behaviors/MousePicking.h>
#include <ui/operation-modes/behaviors/SelectionMousePicking.h>
#include <ui/operation-modes/behaviors/GroundContextMenu.h>
#include <ui/operation-modes/behaviors/Placement.h>
#include <ui/operation-modes/behaviors/Delete.h>

namespace ui::operation_modes::modes::fxcol_editor {
	class RenderFxColBehavior : public OperationModeBehavior, public devtools::debug_rendering::DebugRenderable {
		bool renderBoundingVolumes{ true };
		bool renderCollisionShapes{ true };
		bool renderKdTree{ false };
	public:
		using OperationModeBehavior::OperationModeBehavior;

		static constexpr unsigned int id = 13;
		unsigned int GetId() { return id; }

		void RenderIngameDebugVisuals(hh::gfnd::DrawContext& ctx) {
			auto* fxColMgr = hh::game::GameManager::GetInstance()->GetService<app::gfx::FxColManager>();

			if (fxColMgr == nullptr || fxColMgr->resource == nullptr)
				return;

			if (renderKdTree)
				RenderDebugVisual(ctx, fxColMgr->collisionTree);

			if (renderBoundingVolumes)
				for (size_t i = 0; i < fxColMgr->resource->fxColData->kdTreeLeafCount; i++)
					ctx.DrawAABB(fxColMgr->resource->fxColData->kdTreeLeaves[i].aabbMin, fxColMgr->resource->fxColData->kdTreeLeaves[i].aabbMax, { 0, 0, 255, 255 });

			if (renderCollisionShapes)
				for (size_t i = 0; i < fxColMgr->resource->fxColData->shapeCount; i++)
					RenderDebugVisual(ctx, fxColMgr->resource->fxColData->shapes[i]);
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
