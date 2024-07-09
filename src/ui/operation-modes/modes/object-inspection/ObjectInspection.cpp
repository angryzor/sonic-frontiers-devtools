#include "ObjectInspection.h"
#include <utilities/BoundingBoxes.h>
#include <ui/operation-modes/behaviors/Selection.h>
#include <ui/operation-modes/behaviors/SelectionAabb.h>
#include <ui/operation-modes/behaviors/SelectionMousePicking.h>
#include <ui/operation-modes/behaviors/SelectionTransformation.h>
#include <ui/operation-modes/behaviors/SelectionVisual.h>
#include <ui/operation-modes/behaviors/Gizmo.h>
#include <ui/operation-modes/behaviors/MousePicking.h>
#include <ui/operation-modes/behaviors/Delete.h>
#include <ui/operation-modes/behaviors/GroundContextMenu.h>
#include <ui/operation-modes/behaviors/DebugCommentsVisual.h>
#include <ui/operation-modes/BehaviorContext.h>
#include "ObjectList.h"
#include "ObjectInspector.h"

namespace ui::operation_modes::modes::object_inspection {

	using namespace hh::fnd;
	using namespace hh::game;

	class SelectionTransformationBehaviorContext : public BehaviorContext<Context, SelectionTransformationBehavior<hh::game::GameObject*>> {
		using BehaviorContext::BehaviorContext;
		virtual bool HasTransform(GameObject* obj) override { return obj->GetComponent<GOCTransform>() != nullptr; }
		virtual bool IsRoot(GameObject* obj) override { return obj->GetComponent<GOCTransform>()->GetParent() == nullptr; }
		virtual GameObject* GetParent(GameObject* obj) override { return obj->GetComponent<GOCTransform>()->GetParent()->GetOwnerGameObject(); }
		virtual Eigen::Affine3f GetSelectionSpaceTransform(GameObject* obj) override { return TransformToAffine3f(obj->GetComponent<GOCTransform>()->GetFrame().fullTransform); }
		virtual void SetSelectionSpaceTransform(GameObject* obj, const Eigen::Affine3f& transform) override {
			auto* gocTransform = obj->GetComponent<GOCTransform>();
			auto absoluteTransform = TransformToAffine3f(gocTransform->GetFrame().fullTransform);
			auto localTransform = TransformToAffine3f(gocTransform->GetTransform());

			gocTransform->SetLocalTransform(Affine3fToTransform(localTransform * absoluteTransform.inverse() * transform));
		}
	};

	class DeleteBehaviorContext : public BehaviorContext<Context, DeleteBehavior<GameObject*>> {
		using BehaviorContext::BehaviorContext;
		void DeleteObjects(const csl::ut::MoveArray<GameObject*>& objects) {
			for (auto* obj : objects)
				obj->Kill();
		}
	};

	class MousePickingPhysicsBehaviorContext : public BehaviorContext<Context, MousePickingPhysicsBehavior<GameObject*>> {
		using BehaviorContext::BehaviorContext;
		virtual const char* GetObjectName(GameObject* obj) override { return obj->name; }
		virtual GameObject* GetObjectForGameObject(GameObject* obj) override { return obj; }
		virtual bool IsSelectable(hh::game::GameObject* obj) override { return true; }
		virtual void GetFrustumResults(const Frustum& frustum, csl::ut::MoveArray<GameObject*>& results) override {
			if (auto* gameManager = hh::game::GameManager::GetInstance())
				for (auto* object : gameManager->objects)
					if (auto* gocTransform = object->GetComponent<GOCTransform>())
						if (frustum.Test(gocTransform->GetFrame().fullTransform.position))
							results.push_back(object);
		}
	};

	class SelectionAabbBehaviorContext : public BehaviorContext<Context, SelectionAabbBehavior<hh::game::GameObject*>> {
		using BehaviorContext::BehaviorContext;
		virtual bool CalculateAabb(const csl::ut::MoveArray<hh::game::GameObject*>& objects, csl::geom::Aabb& aabb) override { return CalcApproxAabb(objects, aabb); }
	};

	ObjectInspection::ObjectInspection(csl::fnd::IAllocator* allocator) : OperationMode{ allocator } {
		AddPanel<ObjectList>();
		AddPanel<ObjectInspector>();
		AddBehavior<SelectionBehavior<GameObject*>>();
		AddBehavior<SelectionAabbBehavior<GameObject*>, SelectionAabbBehaviorContext>();
		AddBehavior<SelectionTransformationBehavior<GameObject*>, SelectionTransformationBehaviorContext>();
		AddBehavior<SelectionVisual3DBehavior>();
		AddBehavior<GizmoBehavior>();
		AddBehavior<MousePickingPhysicsBehavior<GameObject*>, MousePickingPhysicsBehaviorContext>();
		AddBehavior<SelectionMousePickingBehavior<GameObject*>>();
		AddBehavior<DeleteBehavior<GameObject*>, DeleteBehaviorContext>();
		AddBehavior<GroundContextMenuBehavior<ObjectData*>>();
		AddBehavior<DebugCommentsVisualBehavior>();

		hh::game::GameManager::GetInstance()->AddListener(this);
	}

	ObjectInspection::~ObjectInspection()
	{
		hh::game::GameManager::GetInstance()->RemoveListener(this);
	}

	void ObjectInspection::GameObjectRemovedCallback(hh::game::GameManager* gameManager, hh::game::GameObject* gameObject)
	{
		auto* selectionBehavior = GetBehavior<SelectionBehavior<GameObject*>>();

		if (selectionBehavior->IsSelected(gameObject))
			selectionBehavior->Deselect(gameObject);
	}
}
