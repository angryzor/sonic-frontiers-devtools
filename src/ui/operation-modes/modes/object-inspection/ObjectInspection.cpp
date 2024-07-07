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

using namespace hh::fnd;
using namespace hh::game;

ObjectInspection::ObjectInspection(csl::fnd::IAllocator* allocator) : OperationMode{ allocator } {
	AddBehavior<SelectionBehavior<GameObject*>>();
	AddBehavior<SelectionAabbBehavior<GameObject*>>(*this);
	AddBehavior<SelectionTransformationBehavior<GameObject*>>(*this);
	AddBehavior<SelectionVisual3DBehavior>();
	AddBehavior<GizmoBehavior>();
	AddBehavior<MousePickingPhysicsBehavior<GameObject*>>(*this);
	AddBehavior<SelectionMousePickingBehavior<GameObject*>>();
	AddBehavior<DeleteBehavior<GameObject*>>(*this);
}

void ObjectInspection::Render() {
	objectList.Render();
	objectInspector.Render();
}

bool ObjectInspection::HasTransform(GameObject* obj) {
	return obj->GetComponent<GOCTransform>() != nullptr;
}

bool ObjectInspection::IsRoot(GameObject* obj) {
	return obj->GetComponent<GOCTransform>()->GetParent() == nullptr;
}

GameObject* ObjectInspection::GetParent(GameObject* obj) {
	return obj->GetComponent<GOCTransform>()->GetParent()->GetOwnerGameObject();
}

Eigen::Affine3f ObjectInspection::GetSelectionSpaceTransform(GameObject* obj) {
	return TransformToAffine3f(obj->GetComponent<GOCTransform>()->GetFrame().fullTransform);
}

void ObjectInspection::SetSelectionSpaceTransform(GameObject* obj, const Eigen::Affine3f& transform) {
	auto* gocTransform = obj->GetComponent<GOCTransform>();
	auto absoluteTransform = TransformToAffine3f(gocTransform->GetFrame().fullTransform);
	auto localTransform = TransformToAffine3f(gocTransform->GetTransform());

	gocTransform->SetLocalTransform(Affine3fToTransform(localTransform * absoluteTransform.inverse() * transform));
}

void ObjectInspection::DeleteObjects(const csl::ut::MoveArray<GameObject*>& objects) {
	for (auto* obj : objects)
		obj->Kill();
}

GameObject* ObjectInspection::GetObjectForGameObject(GameObject* obj) {
	return obj;
}

bool ObjectInspection::IsSelectable(hh::game::GameObject* obj) {
	return true;
}

const char* ObjectInspection::GetObjectName(GameObject* obj) {
	return obj->name;
}

void ObjectInspection::GetFrustumResults(const Frustum& frustum, csl::ut::MoveArray<GameObject*>& results) {
	if (auto* gameManager = hh::game::GameManager::GetInstance())
		for (auto* object : gameManager->objects)
			if (auto* gocTransform = object->GetComponent<GOCTransform>())
				if (frustum.Test(gocTransform->GetFrame().fullTransform.position))
					results.push_back(object);
}

bool ObjectInspection::CalculateAabb(const csl::ut::MoveArray<hh::game::GameObject*>& objects, csl::geom::Aabb& aabb) {
	return CalcApproxAabb(objects, aabb);
}
