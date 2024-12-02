#include "BoundingBoxes.h"

bool AddToAabb(hh::game::GameObject* object, csl::geom::Aabb& aabb) {
	if (auto* visual = object->GetComponent<hh::gfx::GOCVisualTransformed>()) {
		AddAabb(aabb, visual->transformedAabb);
		return true;
	}

	csl::geom::Aabb tempAabb;
	int colliderCount{ 0 };

	for (auto* component : object->components) {
		if (component->pStaticClass == hh::physics::GOCSphereCollider::GetClass()) {
			tempAabb = CalcAabb(*static_cast<hh::physics::GOCSphereCollider*>(component));
			colliderCount++;
			if (colliderCount > 1)
				break;
		}
		if (component->pStaticClass == hh::physics::GOCBoxCollider::GetClass()) {
			tempAabb = CalcAabb(*static_cast<hh::physics::GOCBoxCollider*>(component));
			colliderCount++;
			if (colliderCount > 1)
				break;
		}
		if (component->pStaticClass == hh::physics::GOCCylinderCollider::GetClass()) {
			tempAabb = CalcAabb(*static_cast<hh::physics::GOCCylinderCollider*>(component));
			colliderCount++;
			if (colliderCount > 1)
				break;
		}
		if (component->pStaticClass == hh::physics::GOCCapsuleCollider::GetClass()) {
			tempAabb = CalcAabb(*static_cast<hh::physics::GOCCapsuleCollider*>(component));
			colliderCount++;
			if (colliderCount > 1)
				break;
		}
	}

	if (colliderCount == 1) {
		AddAabb(aabb, tempAabb);
		return true;
	}
		
	if (auto* gocTransform = object->GetComponent<hh::game::GOCTransform>()) {
		aabb.AddPoint(gocTransform->GetFrame().fullTransform.position);
		return true;
	}

	return false;
}

bool AddToAabb(hh::game::ObjectData* objectData, csl::geom::Aabb& aabb) {
	aabb.AddPoint(objectData->transform.position);
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

csl::geom::Aabb CalcAabb(const hh::physics::GOCSphereCollider& coll) {
	auto tf = coll.GetWorldTransform();

	Eigen::Matrix3f rot;
	Eigen::Matrix3f scale;

	tf.computeRotationScaling(&rot, &scale);

	Eigen::Affine3f transform{};
	transform.fromPositionOrientationScale(tf.translation(), Eigen::Quaternionf::Identity(), scale.diagonal());

	return {
		transform * Eigen::Vector3f{ -coll.radius, -coll.radius, -coll.radius },
		transform * Eigen::Vector3f{ coll.radius, coll.radius, coll.radius },
	};
}

csl::geom::Aabb CalcAabb(const hh::physics::GOCBoxCollider& coll) {
	auto tf = coll.GetWorldTransform();

	csl::math::Vector3 corners[8]{
		tf * csl::math::Vector3{ -coll.dimensions.x(), -coll.dimensions.y(), -coll.dimensions.z() },
		tf * csl::math::Vector3{ -coll.dimensions.x(), -coll.dimensions.y(), coll.dimensions.z() },
		tf * csl::math::Vector3{ -coll.dimensions.x(), coll.dimensions.y(), -coll.dimensions.z() },
		tf * csl::math::Vector3{ coll.dimensions.x(), -coll.dimensions.y(), -coll.dimensions.z() },
		tf * csl::math::Vector3{ coll.dimensions.x(), coll.dimensions.y(), -coll.dimensions.z() },
		tf * csl::math::Vector3{ coll.dimensions.x(), -coll.dimensions.y(), coll.dimensions.z() },
		tf * csl::math::Vector3{ -coll.dimensions.x(), coll.dimensions.y(), coll.dimensions.z() },
		tf * csl::math::Vector3{ coll.dimensions.x(), coll.dimensions.y(), coll.dimensions.z() },
	};

	csl::geom::Aabb res{ { INFINITY, INFINITY, INFINITY }, { -INFINITY, -INFINITY, -INFINITY } };

	for (size_t i = 0; i < 8; i++)
		res.AddPoint(corners[i]);

	return res;
}

csl::geom::Aabb CalcAabb(const hh::physics::GOCCylinderCollider& coll) {
	auto tf = coll.GetWorldTransform();

	csl::math::Vector3 corners[8]{
		tf * csl::math::Vector3{ -coll.radius, -coll.halfHeight, -coll.radius },
		tf * csl::math::Vector3{ -coll.radius, -coll.halfHeight, coll.radius },
		tf * csl::math::Vector3{ -coll.radius, coll.halfHeight, -coll.radius },
		tf * csl::math::Vector3{ coll.radius, -coll.halfHeight, -coll.radius },
		tf * csl::math::Vector3{ coll.radius, coll.halfHeight, -coll.radius },
		tf * csl::math::Vector3{ coll.radius, -coll.halfHeight, coll.radius },
		tf * csl::math::Vector3{ -coll.radius, coll.halfHeight, coll.radius },
		tf * csl::math::Vector3{ coll.radius, coll.halfHeight, coll.radius },
	};

	csl::geom::Aabb res{ { INFINITY, INFINITY, INFINITY }, { -INFINITY, -INFINITY, -INFINITY } };

	for (size_t i = 0; i < 8; i++)
		res.AddPoint(corners[i]);

	return res;
}

csl::geom::Aabb CalcAabb(const hh::physics::GOCCapsuleCollider& coll) {
	auto tf = coll.GetWorldTransform();

	csl::math::Vector3 corners[8]{
		tf * csl::math::Vector3{ -coll.radius, -(coll.halfHeight + coll.radius), -coll.radius },
		tf * csl::math::Vector3{ -coll.radius, -(coll.halfHeight + coll.radius), coll.radius },
		tf * csl::math::Vector3{ -coll.radius, (coll.halfHeight + coll.radius), -coll.radius },
		tf * csl::math::Vector3{ coll.radius, -(coll.halfHeight + coll.radius), -coll.radius },
		tf * csl::math::Vector3{ coll.radius, (coll.halfHeight + coll.radius), -coll.radius },
		tf * csl::math::Vector3{ coll.radius, -(coll.halfHeight + coll.radius), coll.radius },
		tf * csl::math::Vector3{ -coll.radius, (coll.halfHeight + coll.radius), coll.radius },
		tf * csl::math::Vector3{ coll.radius, (coll.halfHeight + coll.radius), coll.radius },
	};

	csl::geom::Aabb res{ { INFINITY, INFINITY, INFINITY }, { -INFINITY, -INFINITY, -INFINITY } };

	for (size_t i = 0; i < 8; i++)
		res.AddPoint(corners[i]);

	return res;
}
