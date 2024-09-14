#pragma once
#include "MousePicking3DRecursiveRaycast.h"

template<typename OpModeContext>
struct MousePicking3DRecursiveRaycastWithPhysicsLocationMousePicking3DBehaviorTraitsImpl : MousePicking3DRecursiveRaycastMousePicking3DBehaviorTraitsImpl<OpModeContext> {
	using MousePicking3DRecursiveRaycastMousePicking3DBehaviorTraitsImpl<OpModeContext>::MousePicking3DRecursiveRaycastMousePicking3DBehaviorTraitsImpl;

	virtual void GetBestRaycastResult(const Ray3f& ray, csl::ut::MoveArray<typename MousePicking3DRecursiveRaycastMousePicking3DBehaviorTraitsImpl<OpModeContext>::ObjectType>& results, typename MousePicking3DRecursiveRaycastMousePicking3DBehaviorTraitsImpl<OpModeContext>::LocationType& location, bool& pickedLocation) {
		MousePicking3DRecursiveRaycastMousePicking3DBehaviorTraitsImpl<OpModeContext>::GetBestRaycastResult(ray, results, location, pickedLocation);

		if (!pickedLocation)
		if (auto* gameManager = hh::game::GameManager::GetInstance())
		if (auto* physicsWorld = gameManager->GetService<hh::physics::PhysicsWorld>()) {
			csl::ut::MoveArray<hh::physics::PhysicsQueryResult> physicsResults{ hh::fnd::MemoryRouter::GetTempAllocator() };

			if (physicsWorld->RayCastAllHits(ray.start, ray.end, 0xFFFFFFFF, physicsResults)) {
				for (auto& result : physicsResults) {
					if (auto* collider = hh::game::GameObjectSystem::GetComponentByHandle(result.collider)) {
						auto* gameObject = collider->GetOwnerGameObject();

						if (Desktop::selectionColliderFilters[gameObject->layer][collider->filterCategory]) {
							location = result.hitLocation;
							pickedLocation = true;
							return;
						}
					}
				}
			}
		}
	}
};
