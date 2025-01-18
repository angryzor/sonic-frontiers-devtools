#include "Colliders.h"

namespace devtools::debug_rendering::renderables {
	void Colliders::RenderIngameDebugVisuals(hh::gfnd::DrawContext& ctx)
	{
		if (!enabled)
			return;

		auto* gameManager = hh::game::GameManager::GetInstance();
		if (!gameManager)
			return;

		for (auto* gameObject : gameManager->objects) {
			//if (auto* gocV = gameObject->GetComponent<GOCVisualTransformed>()) {
			//	renderer->drawContext->DrawOBB(gocV->worldMatrix, { 1, 1, 1 }, { 255, 255, 255, 0 });
			//	renderer->drawContext->DrawAABB(gocV->transformedAabb.m_Min, gocV->transformedAabb.m_Max, { 255, 255, 255, 255 });
			//}
			for (auto* goc : gameObject->components) {
				if (goc->pStaticClass == hh::physics::GOCSphereCollider::GetClass()) {
					auto* cGoc = static_cast<hh::physics::GOCSphereCollider*>(goc);
					if (colliderFilters[gameObject->layer][cGoc->filterCategory])
						ctx.DrawSphere(cGoc->GetWorldTransform(), cGoc->radius, { 0, 255, 255, 255 });
				}
				else if (goc->pStaticClass == hh::physics::GOCBoxCollider::GetClass()) {
					auto* cGoc = static_cast<hh::physics::GOCBoxCollider*>(goc);
					if (colliderFilters[gameObject->layer][cGoc->filterCategory])
						ctx.DrawOBB(cGoc->GetWorldTransform(), cGoc->dimensions, { 0, 255, 255, 255 });
				}
				else if (goc->pStaticClass == hh::physics::GOCCapsuleCollider::GetClass()) {
					auto* cGoc = static_cast<hh::physics::GOCCapsuleCollider*>(goc);
					if (colliderFilters[gameObject->layer][cGoc->filterCategory])
						ctx.DrawCapsule(cGoc->GetWorldTransform(), cGoc->radius, cGoc->halfHeight / 2, { 0, 255, 255, 255 });
				}
				else if (goc->pStaticClass == hh::physics::GOCCylinderCollider::GetClass()) {
					auto* cGoc = static_cast<hh::physics::GOCCylinderCollider*>(goc);
					if (colliderFilters[gameObject->layer][cGoc->filterCategory])
						ctx.DrawCylinder(cGoc->GetWorldTransform(), cGoc->radius, cGoc->halfHeight, { 0, 255, 255, 255 });
				}
			}
		}
	}
}
