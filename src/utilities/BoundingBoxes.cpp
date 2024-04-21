#include "BoundingBoxes.h"

bool CalcApproxAabb(const csl::ut::MoveArray<hh::game::GameObject*>& objects, csl::geom::Aabb& aabb)
{
	aabb = csl::geom::Aabb{ { INFINITY, INFINITY, INFINITY }, { -INFINITY, -INFINITY, -INFINITY } };
	bool found{ false };

	for (auto& object : objects) {
		if (auto* visual = object->GetComponent<hh::gfx::GOCVisualTransformed>()) {
			aabb.m_Min.x() = std::fminf(aabb.m_Min.x(), visual->transformedAabb.m_Min.x());
			aabb.m_Min.y() = std::fminf(aabb.m_Min.y(), visual->transformedAabb.m_Min.y());
			aabb.m_Min.z() = std::fminf(aabb.m_Min.z(), visual->transformedAabb.m_Min.z());
			aabb.m_Max.x() = std::fmaxf(aabb.m_Max.x(), visual->transformedAabb.m_Max.x());
			aabb.m_Max.y() = std::fmaxf(aabb.m_Max.y(), visual->transformedAabb.m_Max.y());
			aabb.m_Max.z() = std::fmaxf(aabb.m_Max.z(), visual->transformedAabb.m_Max.z());
			found = true;
		}
		else {
			csl::geom::Aabb tempAabb;
			int colliderCount{ 0 };

			for (auto* component : object->components) {
				if (component->pStaticClass == hh::physics::GOCSphereCollider::GetClass()) {
					auto* coll = static_cast<hh::physics::GOCSphereCollider*>(component);
					tempAabb.m_Min = Eigen::Vector3f{ coll->transformedWorldPosition.m_Position - coll->scale * coll->radius };
					tempAabb.m_Max = Eigen::Vector3f{ coll->transformedWorldPosition.m_Position + coll->scale * coll->radius };
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
				found = true;
			}
			else if (auto* gocTransform = object->GetComponent<hh::game::GOCTransform>()) {
				aabb.m_Min.x() = std::fminf(aabb.m_Min.x(), gocTransform->frame->fullTransform.position.x());
				aabb.m_Min.y() = std::fminf(aabb.m_Min.y(), gocTransform->frame->fullTransform.position.y());
				aabb.m_Min.z() = std::fminf(aabb.m_Min.z(), gocTransform->frame->fullTransform.position.z());
				aabb.m_Max.x() = std::fmaxf(aabb.m_Max.x(), gocTransform->frame->fullTransform.position.x());
				aabb.m_Max.y() = std::fmaxf(aabb.m_Max.y(), gocTransform->frame->fullTransform.position.y());
				aabb.m_Max.z() = std::fmaxf(aabb.m_Max.z(), gocTransform->frame->fullTransform.position.z());
				found = true;
			}
		}
	}

	return found;
}
