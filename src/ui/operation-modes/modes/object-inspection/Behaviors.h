#pragma once
#include <utilities/BoundingBoxes.h>
#include <utilities/math/MathUtils.h>
#include <ui/operation-modes/OperationMode.h>
#include <ui/operation-modes/behaviors/ForwardDeclarations.h>
#include "Context.h"

namespace ui::operation_modes::modes::object_inspection {
	using namespace hh::game;

	template<> struct MousePickingPhysicsBehaviorTraits<Context> : BehaviorTraitsImpl<Context> {
		using BehaviorTraitsImpl::BehaviorTraitsImpl;
		using ObjectType = GameObject*;
		const char* GetObjectName(GameObject* obj) { return obj->name; }
		GameObject* GetObjectForGameObject(GameObject* obj) { return obj; }
		bool IsSelectable(GameObject* obj) { return true; }
		void GetFrustumResults(const Frustum& frustum, csl::ut::MoveArray<GameObject*>& results) {
			if (auto* gameManager = GameManager::GetInstance())
				for (auto* object : gameManager->objects)
					if (auto* gocTransform = object->GetComponent<GOCTransform>())
						if (frustum.Test(gocTransform->GetFrame().fullTransform.position))
							results.push_back(object);
		}
	};
}

#include <ui/operation-modes/behaviors/prefabs/MousePickingPhysics.h>

namespace ui::operation_modes::modes::object_inspection {
	template<> struct MousePicking3DBehaviorTraits<Context> : MousePickingPhysicsMousePicking3DBehaviorTraitsImpl<Context> {
		using MousePickingPhysicsMousePicking3DBehaviorTraitsImpl::MousePickingPhysicsMousePicking3DBehaviorTraitsImpl;
	};
}

#include <ui/operation-modes/behaviors/prefabs/MousePicking3DCameraManagerCameraProvider.h>

namespace ui::operation_modes::modes::object_inspection {
	template<> struct MousePicking3DCameraProvider<Context> : MousePicking3DCameraManagerCameraProvider<Context> {
		using MousePicking3DCameraManagerCameraProvider::MousePicking3DCameraManagerCameraProvider;
	};
}

#include <ui/operation-modes/behaviors/prefabs/MousePicking3D.h>

namespace ui::operation_modes::modes::object_inspection {
	template<> struct MousePickingBehaviorTraits<Context> : MousePicking3DMousePickingBehaviorTraitsImpl<Context> {
		using MousePicking3DMousePickingBehaviorTraitsImpl::MousePicking3DMousePickingBehaviorTraitsImpl;
	};
}

namespace ui::operation_modes::modes::object_inspection {
	using namespace hh::game;

	template<> struct SelectionBehaviorTraits<Context> {
		using ObjectType = GameObject*;
	};

	template<> struct SelectionTransformationBehaviorTraits<Context> : BehaviorTraitsImpl<Context> {
		using BehaviorTraitsImpl::BehaviorTraitsImpl;
		static constexpr bool Projective = false;
		bool HasTransform(GameObject* obj) { return obj->GetComponent<GOCTransform>() != nullptr; }
		bool IsRoot(GameObject* obj) { return obj->GetComponent<GOCTransform>()->GetParent() == nullptr; }
		GameObject* GetParent(GameObject* obj) { return obj->GetComponent<GOCTransform>()->GetParent()->GetOwnerGameObject(); }
		Eigen::Affine3f GetSelectionSpaceTransform(GameObject* obj) const { return TransformToAffine3f(obj->GetComponent<GOCTransform>()->GetFrame().fullTransform); }
		void SetSelectionSpaceTransform(GameObject* obj, const Eigen::Affine3f& transform) {
			auto* gocTransform = obj->GetComponent<GOCTransform>();
			auto absoluteTransform = TransformToAffine3f(gocTransform->GetFrame().fullTransform);
			auto localTransform = TransformToAffine3f(gocTransform->GetTransform());

			gocTransform->SetLocalTransform(Affine3fToTransform(localTransform * absoluteTransform.inverse() * transform));
		}
	};

	template<> struct DeleteBehaviorTraits<Context> : BehaviorTraitsImpl<Context> {
		using BehaviorTraitsImpl::BehaviorTraitsImpl;
		void DeleteObjects(const csl::ut::MoveArray<GameObject*>& objects) {
			for (auto* obj : objects)
				obj->Kill();
		}
	};

	template<> struct SelectionAabbBehaviorTraits<Context> : BehaviorTraitsImpl<Context> {
		using BehaviorTraitsImpl::BehaviorTraitsImpl;
		bool CalculateAabb(const csl::ut::MoveArray<GameObject*>& objects, csl::geom::Aabb& aabb) { return CalcApproxAabb(objects, aabb); }
	};

	template<> struct GizmoBehaviorTraits<Context> : BehaviorTraitsImpl<Context> {
		using BehaviorTraitsImpl::BehaviorTraitsImpl;
		static constexpr bool allowTranslate = true;
		static constexpr bool allowRotate = true;
		static constexpr bool allowScale = true;
	};
}

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
