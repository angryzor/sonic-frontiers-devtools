#include "FxColEditor.h"
#include <ui/operation-modes/behaviors/Selection.h>
#include <ui/operation-modes/behaviors/SelectionAabb.h>
#include <ui/operation-modes/behaviors/SelectionTransformation.h>
#include <ui/operation-modes/behaviors/Gizmo.h>
#include <ui/operation-modes/behaviors/MousePicking.h>
#include <ui/operation-modes/behaviors/SelectionMousePicking.h>
#include <ui/operation-modes/BehaviorContext.h>
#include <debug-rendering/GOCVisualDebugDrawRenderer.h>
#include <debug-rendering/visuals/FxColCollisionShape.h>
#include "ShapeList.h"
#include "ShapeInspector.h"

namespace ui::operation_modes::modes::fxcol_editor {
	using namespace app::gfx;

	class SelectionTransformationBehaviorContext : public BehaviorContext<Context, SelectionTransformationBehavior<FxColCollisionShape*>> {
		using BehaviorContext::BehaviorContext;
		virtual bool HasTransform(FxColCollisionShape* obj) override { return true; }
		virtual bool IsRoot(FxColCollisionShape* obj) override { return true; }
		virtual FxColCollisionShape* GetParent(FxColCollisionShape* obj) override { return nullptr; }
		virtual Eigen::Affine3f GetSelectionSpaceTransform(FxColCollisionShape* obj) override { return Eigen::Affine3f{ Eigen::Translation3f{ obj->position } * obj->rotation }; }
		virtual void SetSelectionSpaceTransform(FxColCollisionShape* obj, const Eigen::Affine3f& transform) override {
			obj->position = csl::math::Position{ transform.translation() };
			obj->rotation = csl::math::Rotation{ transform.rotation() };
		}
	};

	class MousePickingFxColBehaviorContext : public BehaviorContext<Context, MousePicking3DRecursiveRaycastBehavior<FxColCollisionShape*, csl::math::Vector3>> {
		using BehaviorContext::BehaviorContext;
		virtual const char* GetObjectName(FxColCollisionShape* object) override { return object->ownerName; }
		virtual bool IsSelectable(FxColCollisionShape* object) override { return true; }
		virtual bool Intersects(FxColCollisionShape* object, const Ray3f& ray) override { return false; }
		virtual void GetRootObjects(csl::ut::MoveArray<FxColCollisionShape*>& rootObjects) override {}
		virtual void GetChildren(FxColCollisionShape* object, csl::ut::MoveArray<FxColCollisionShape*>& children) override {}
		virtual void GetFrustumResults(const Frustum& frustum, csl::ut::MoveArray<FxColCollisionShape*>& results) override {
			if (auto* fxColManager = hh::game::GameManager::GetInstance()->GetService<FxColManager>())
				if (fxColManager->resource != nullptr)
					for (size_t i = 0; i < fxColManager->resource->fxColData->collisionShapeCount; i++) {
						auto& shape = fxColManager->resource->fxColData->collisionShapes[i];
						if (frustum.Test(shape.position))
							results.push_back(&shape);
					}
		}
	};

	class MousePickingCameraProviderContext : public BehaviorContext<Context, MousePicking3DRecursiveRaycastBehavior<FxColCollisionShape*, csl::math::Vector3>, MousePicking3DCameraManagerCameraProvider> {
		using BehaviorContext::BehaviorContext;
	};

	class SelectionAabbBehaviorContext : public BehaviorContext<Context, SelectionAabbBehavior<FxColCollisionShape*>> {
		using BehaviorContext::BehaviorContext;
		virtual bool CalculateAabb(const csl::ut::MoveArray<FxColCollisionShape*>& objects, csl::geom::Aabb& aabb) override {
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
			Eigen::Affine3f transform{ Eigen::Translation3f{ position } * rotation };

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

	class RenderFxColBehavior : public OperationModeBehavior, public DebugRenderable {
	public:
		using OperationModeBehavior::OperationModeBehavior;

		static constexpr unsigned int id = 13;
		virtual unsigned int GetId() override { return id; }

		virtual void RenderDebugVisuals(hh::gfnd::DrawContext& ctx) override {
			auto* fxColMgr = hh::game::GameManager::GetInstance()->GetService<app::gfx::FxColManager>();

			if (fxColMgr == nullptr || fxColMgr->resource == nullptr)
				return;

			auto& selection = operationMode.GetBehavior<SelectionBehavior<FxColCollisionShape*>>()->GetSelection();

			for (size_t i = 0; i < fxColMgr->resource->fxColData->collisionShapeCount; i++)
				RenderDebugVisual(ctx, fxColMgr->resource->fxColData->collisionShapes[i]);
		}
	};

	FxColEditor::FxColEditor(csl::fnd::IAllocator* allocator) : OperationMode{ allocator } {
		AddPanel<ShapeList>();
		AddPanel<ShapeInspector>();
		AddBehavior<SelectionBehavior<FxColCollisionShape*>>();
		AddBehavior<SelectionAabbBehavior<FxColCollisionShape*>, SelectionAabbBehaviorContext>();
		AddBehavior<SelectionTransformationBehavior<FxColCollisionShape*>, SelectionTransformationBehaviorContext>();
		AddBehavior<GizmoBehavior>();
		AddBehavior<MousePicking3DRecursiveRaycastBehavior<FxColCollisionShape*, csl::math::Vector3>, MousePickingFxColBehaviorContext, MousePickingCameraProviderContext>();
		AddBehavior<SelectionMousePickingBehavior<FxColCollisionShape*>>();
		AddBehavior<RenderFxColBehavior>();
	}
}
