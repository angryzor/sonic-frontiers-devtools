#include "BoundingBoxes.h"

bool AddToAabb(hh::game::GameObject* object, csl::geom::Aabb& aabb) {
	if (auto* visual = object->GetComponent<hh::gfx::GOCVisualTransformed>()) {
		aabb.m_Min.x() = std::fminf(aabb.m_Min.x(), visual->transformedAabb.m_Min.x());
		aabb.m_Min.y() = std::fminf(aabb.m_Min.y(), visual->transformedAabb.m_Min.y());
		aabb.m_Min.z() = std::fminf(aabb.m_Min.z(), visual->transformedAabb.m_Min.z());
		aabb.m_Max.x() = std::fmaxf(aabb.m_Max.x(), visual->transformedAabb.m_Max.x());
		aabb.m_Max.y() = std::fmaxf(aabb.m_Max.y(), visual->transformedAabb.m_Max.y());
		aabb.m_Max.z() = std::fmaxf(aabb.m_Max.z(), visual->transformedAabb.m_Max.z());
		return true;
	}

	csl::geom::Aabb tempAabb;
	int colliderCount{ 0 };

	for (auto* component : object->components) {
		if (component->pStaticClass == hh::physics::GOCSphereCollider::GetClass()) {
			auto* coll = static_cast<hh::physics::GOCSphereCollider*>(component);
			tempAabb.m_Min = coll->GetWorldTransform() * Eigen::Vector3f{ -coll->radius, -coll->radius, -coll->radius };
			tempAabb.m_Max = coll->GetWorldTransform() * Eigen::Vector3f{ coll->radius, coll->radius, coll->radius };
			colliderCount++;
			if (colliderCount > 1)
				break;
		}
	}

	if (colliderCount == 1) {
		aabb.m_Min.x() = std::fminf(aabb.m_Min.x(), tempAabb.m_Min.x());
		aabb.m_Min.y() = std::fminf(aabb.m_Min.y(), tempAabb.m_Min.y());
		aabb.m_Min.z() = std::fminf(aabb.m_Min.z(), tempAabb.m_Min.z());
		aabb.m_Max.x() = std::fmaxf(aabb.m_Max.x(), tempAabb.m_Max.x());
		aabb.m_Max.y() = std::fmaxf(aabb.m_Max.y(), tempAabb.m_Max.y());
		aabb.m_Max.z() = std::fmaxf(aabb.m_Max.z(), tempAabb.m_Max.z());
		return true;
	}
		
	if (auto* gocTransform = object->GetComponent<hh::game::GOCTransform>()) {
		aabb.m_Min.x() = std::fminf(aabb.m_Min.x(), gocTransform->GetFrame().fullTransform.position.x());
		aabb.m_Min.y() = std::fminf(aabb.m_Min.y(), gocTransform->GetFrame().fullTransform.position.y());
		aabb.m_Min.z() = std::fminf(aabb.m_Min.z(), gocTransform->GetFrame().fullTransform.position.z());
		aabb.m_Max.x() = std::fmaxf(aabb.m_Max.x(), gocTransform->GetFrame().fullTransform.position.x());
		aabb.m_Max.y() = std::fmaxf(aabb.m_Max.y(), gocTransform->GetFrame().fullTransform.position.y());
		aabb.m_Max.z() = std::fmaxf(aabb.m_Max.z(), gocTransform->GetFrame().fullTransform.position.z());
		return true;
	}

	return false;
}

bool AddToAabb(hh::game::ObjectData* objectData, csl::geom::Aabb& aabb) {
	aabb.m_Min.x() = std::fminf(aabb.m_Min.x(), objectData->transform.position.x());
	aabb.m_Min.y() = std::fminf(aabb.m_Min.y(), objectData->transform.position.y());
	aabb.m_Min.z() = std::fminf(aabb.m_Min.z(), objectData->transform.position.z());
	aabb.m_Max.x() = std::fmaxf(aabb.m_Max.x(), objectData->transform.position.x());
	aabb.m_Max.y() = std::fmaxf(aabb.m_Max.y(), objectData->transform.position.y());
	aabb.m_Max.z() = std::fmaxf(aabb.m_Max.z(), objectData->transform.position.z());
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
