#pragma once
#include <optional>
#include <ui/Desktop.h>

template<typename OpModeContext>
struct MousePickingPhysicsMousePicking3DBehaviorTraitsImpl {
	using ObjectType = typename MousePickingPhysicsBehaviorTraits<OpModeContext>::ObjectType;
	using LocationType = csl::math::Vector3;

	MousePickingPhysicsBehaviorTraits<OpModeContext> traits;

	MousePickingPhysicsMousePicking3DBehaviorTraitsImpl(OpModeContext& context) : traits{ context } {}

	void GetAllRaycastResults(const Ray3f& ray, csl::ut::MoveArray<ObjectType>& results) {
		if (auto* gameManager = hh::game::GameManager::GetInstance())
			if (auto* physicsWorld = gameManager->GetService<hh::physics::PhysicsWorld>()) {
				csl::ut::MoveArray<hh::physics::PhysicsQueryResult> physicsResults{ hh::fnd::MemoryRouter::GetTempAllocator() };

				if (physicsWorld->RayCastAllHits(ray.start, ray.end, 0xFFFFFFFF, physicsResults)) {
					csl::ut::PointerMap<ObjectType, bool> unique{ hh::fnd::MemoryRouter::GetTempAllocator() };

					for (auto& result : physicsResults) {
						if (auto* collider = hh::game::GameObjectSystem::GetComponentByHandle(result.collider)) {
							auto* gameObject = collider->GetOwnerGameObject();

							if (!traits.IsSelectable(gameObject))
								continue;

							auto* object = traits.GetObjectForGameObject(gameObject);

							if (unique.Find(object) == unique.end())
								results.push_back(object);

							unique.Insert(object, true);
						}
					}
				}
			}
	}

	void GetBestRaycastResult(const Ray3f& ray, csl::ut::MoveArray<ObjectType>& results, csl::math::Vector3& location, bool& pickedLocation) {
		if (auto* gameManager = hh::game::GameManager::GetInstance())
			if (auto* physicsWorld = gameManager->GetService<hh::physics::PhysicsWorld>()) {
				csl::ut::MoveArray<hh::physics::PhysicsQueryResult> physicsResults{ hh::fnd::MemoryRouter::GetTempAllocator() };

				if (physicsWorld->RayCastAllHits(ray.start, ray.end, 0xFFFFFFFF, physicsResults)) {
					std::optional<csl::math::Vector3> fallbackLocation{};

					for (auto& result : physicsResults) {
						if (auto* collider = hh::game::GameObjectSystem::GetComponentByHandle(result.collider)) {
							auto* gameObject = collider->GetOwnerGameObject();

							if (Desktop::selectionColliderFilters[gameObject->layer][collider->filterCategory]) {
								if (!traits.IsSelectable(gameObject)) {
									if (!fallbackLocation.has_value())
										fallbackLocation = result.hitLocation;
									continue;
								}

								results.push_back(traits.GetObjectForGameObject(gameObject));
								location = result.hitLocation;
								pickedLocation = true;
								return;
							}
						}
					}

					if (fallbackLocation.has_value()) {
						location = *fallbackLocation;
						pickedLocation = true;
					}
				}
			}
	}

	void GetFrustumResults(const Frustum& frustum, csl::ut::MoveArray<ObjectType>& results) {
		traits.GetFrustumResults(frustum, results);
	}

	const char* GetObjectName(ObjectType object) {
		return traits.GetObjectName(object);
	}
};
