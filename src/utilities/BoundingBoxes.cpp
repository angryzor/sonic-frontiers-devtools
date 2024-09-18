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
		if (auto* gameObject = chunk->GetGameObject(object))
			found |= AddToAabb(gameObject, aabb);
		else
			found |= AddToAabb(object, aabb);
	}

	return found;
}

void AddAabb(csl::geom::Aabb& aabb, const csl::geom::Aabb& other)
{
	aabb.min = aabb.min.cwiseMin(other.min);
	aabb.max = aabb.max.cwiseMax(other.max);
}

csl::geom::Aabb Union(const csl::geom::Aabb& one, const csl::geom::Aabb& other)
{
	csl::geom::Aabb res{ { INFINITY, INFINITY, INFINITY }, { -INFINITY, -INFINITY, -INFINITY } };
	AddAabb(res, one);
	AddAabb(res, other);
	return res;
}

csl::geom::Aabb CalcAabb(const csl::geom::Sphere& sphere)
{
	return {
		sphere.position - sphere.radius * Eigen::Vector3f{ 1.0f, 1.0f, 1.0f },
		sphere.position + sphere.radius * Eigen::Vector3f{ 1.0f, 1.0f, 1.0f },
	};
}

csl::geom::Aabb CalcAabb(const csl::geom::Cylinder& cylinder)
{
	Eigen::Vector3f normal = (cylinder.segment.end - cylinder.segment.start).normalized();
	Eigen::Vector3f dot{ normal.dot(Eigen::Vector3f::UnitX()), normal.dot(Eigen::Vector3f::UnitY()), normal.dot(Eigen::Vector3f::UnitZ()) };
	Eigen::Vector3f delta = cylinder.radius * (Eigen::Vector3f{ 1.0f, 1.0f, 1.0f } - dot.cwiseSquare());

	csl::geom::Aabb startExtent{ cylinder.segment.start - delta, cylinder.segment.start + delta };
	csl::geom::Aabb endExtent{ cylinder.segment.end - delta, cylinder.segment.end + delta };

	return Union(startExtent, endExtent);
}

csl::geom::Aabb CalcAabb(const csl::geom::Obb& obb)
{
	csl::math::Vector3 corners[8]{
		obb.min,
		obb.min + obb.extentX,
		obb.min + obb.extentY,
		obb.min + obb.extentZ,
		obb.min + obb.extentX + obb.extentY,
		obb.min + obb.extentX + obb.extentZ,
		obb.min + obb.extentY + obb.extentZ,
		obb.min + obb.extentX + obb.extentY + obb.extentZ,
	};

	csl::geom::Aabb res{ { INFINITY, INFINITY, INFINITY }, { -INFINITY, -INFINITY, -INFINITY } };

	for (size_t i = 0; i < 8; i++)
		res.AddPoint(corners[i]);

	return res;
}
