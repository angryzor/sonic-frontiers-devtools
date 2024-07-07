#include "BoundingBoxes.h"

bool AddToAabb(hh::game::GameObject* object, csl::geom::Aabb& aabb) {
	if (auto* visual = object->GetComponent<hh::gfx::GOCVisualTransformed>()) {
		aabb.min.x() = std::fminf(aabb.min.x(), visual->transformedAabb.min.x());
		aabb.min.y() = std::fminf(aabb.min.y(), visual->transformedAabb.min.y());
		aabb.min.z() = std::fminf(aabb.min.z(), visual->transformedAabb.min.z());
		aabb.max.x() = std::fmaxf(aabb.max.x(), visual->transformedAabb.max.x());
		aabb.max.y() = std::fmaxf(aabb.max.y(), visual->transformedAabb.max.y());
		aabb.max.z() = std::fmaxf(aabb.max.z(), visual->transformedAabb.max.z());
		return true;
	}

	csl::geom::Aabb tempAabb;
	int colliderCount{ 0 };

	for (auto* component : object->components) {
		if (component->pStaticClass == hh::physics::GOCSphereCollider::GetClass()) {
			auto* coll = static_cast<hh::physics::GOCSphereCollider*>(component);
			tempAabb.min = coll->GetWorldTransform() * Eigen::Vector3f{ -coll->radius, -coll->radius, -coll->radius };
			tempAabb.max = coll->GetWorldTransform() * Eigen::Vector3f{ coll->radius, coll->radius, coll->radius };
			colliderCount++;
			if (colliderCount > 1)
				break;
		}
	}

	if (colliderCount == 1) {
		aabb.min.x() = std::fminf(aabb.min.x(), tempAabb.min.x());
		aabb.min.y() = std::fminf(aabb.min.y(), tempAabb.min.y());
		aabb.min.z() = std::fminf(aabb.min.z(), tempAabb.min.z());
		aabb.max.x() = std::fmaxf(aabb.max.x(), tempAabb.max.x());
		aabb.max.y() = std::fmaxf(aabb.max.y(), tempAabb.max.y());
		aabb.max.z() = std::fmaxf(aabb.max.z(), tempAabb.max.z());
		return true;
	}
		
	if (auto* gocTransform = object->GetComponent<hh::game::GOCTransform>()) {
		aabb.min.x() = std::fminf(aabb.min.x(), gocTransform->GetFrame().fullTransform.position.x());
		aabb.min.y() = std::fminf(aabb.min.y(), gocTransform->GetFrame().fullTransform.position.y());
		aabb.min.z() = std::fminf(aabb.min.z(), gocTransform->GetFrame().fullTransform.position.z());
		aabb.max.x() = std::fmaxf(aabb.max.x(), gocTransform->GetFrame().fullTransform.position.x());
		aabb.max.y() = std::fmaxf(aabb.max.y(), gocTransform->GetFrame().fullTransform.position.y());
		aabb.max.z() = std::fmaxf(aabb.max.z(), gocTransform->GetFrame().fullTransform.position.z());
		return true;
	}

	return false;
}

bool AddToAabb(hh::game::ObjectData* objectData, csl::geom::Aabb& aabb) {
	aabb.min.x() = std::fminf(aabb.min.x(), objectData->transform.position.x());
	aabb.min.y() = std::fminf(aabb.min.y(), objectData->transform.position.y());
	aabb.min.z() = std::fminf(aabb.min.z(), objectData->transform.position.z());
	aabb.max.x() = std::fmaxf(aabb.max.x(), objectData->transform.position.x());
	aabb.max.y() = std::fmaxf(aabb.max.y(), objectData->transform.position.y());
	aabb.max.z() = std::fmaxf(aabb.max.z(), objectData->transform.position.z());
	return true;
}

bool CalcApproxAabb(const csl::ut::MoveArray<hh::game::GameObject*>& objects, csl::geom::Aabb& aabb)
{
	aabb = csl::geom::Aabb{ { INFINITY, INFINITY, INFINITY }, { -INFINITY, -INFINITY, -INFINITY } };
	bool found{ false };

	for (auto& object : objects) {
		found |= AddToAabb(object, aabb);
	}

	return found;
}

bool CalcApproxAabb(hh::game::ObjectWorldChunk* chunk, const csl::ut::MoveArray<hh::game::ObjectData*>& objects, csl::geom::Aabb& aabb)
{
	aabb = csl::geom::Aabb{ { INFINITY, INFINITY, INFINITY }, { -INFINITY, -INFINITY, -INFINITY } };
	bool found{ false };

	for (auto& object : objects) {
		if (auto* gameObject = chunk->GetGameObjectByObjectId(object->id))
			found |= AddToAabb(gameObject, aabb);
		else
			found |= AddToAabb(object, aabb);
	}

	return found;
}
