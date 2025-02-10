#pragma once
#include <optional>

template<typename OpModeContext>
struct MousePicking3DRecursiveRaycastMousePicking3DBehaviorTraitsImpl {
	using ObjectType = typename MousePicking3DRecursiveRaycastBehaviorTraits<OpModeContext>::ObjectType;
	using LocationType = typename MousePicking3DRecursiveRaycastBehaviorTraits<OpModeContext>::LocationType;

	MousePicking3DRecursiveRaycastBehaviorTraits<OpModeContext> traits;

	MousePicking3DRecursiveRaycastMousePicking3DBehaviorTraitsImpl(OpModeContext& context) : traits{ context } {}

	virtual void GetAllRaycastResults(const Ray3f& ray, csl::ut::MoveArray<ObjectType>& results) {
		csl::ut::MoveArray<ObjectType> rootObjects{ hh::fnd::MemoryRouter::GetTempAllocator() };
		traits.GetRootObjects(rootObjects);

		for (auto object : rootObjects)
			RaycastAll(object, ray, results);
	}

	virtual void GetBestRaycastResult(const Ray3f& ray, csl::ut::MoveArray<ObjectType>& results, LocationType& location, bool& pickedLocation) {
		csl::ut::MoveArray<ObjectType> rootObjects{ hh::fnd::MemoryRouter::GetTempAllocator() };
		traits.GetRootObjects(rootObjects);

		for (auto object : rootObjects)
			if (auto pick = RaycastBest(object, ray)) {
				results.push_back(*pick);
				return;
			}
	}

	virtual void GetFrustumResults(const Frustum& frustum, csl::ut::MoveArray<ObjectType>& results) {
		traits.GetFrustumResults(frustum, results);
	}

	virtual const char* GetObjectName(ObjectType object) {
		return traits.GetObjectName(object);
	}

	void RaycastAll(ObjectType object, const Ray3f& ray, csl::ut::MoveArray<ObjectType>& results)
	{
		if (!traits.IsSelectable(object))
			return;

		csl::ut::MoveArray<ObjectType> children{ hh::fnd::MemoryRouter::GetTempAllocator() };
		traits.GetChildren(object, children);

		for (size_t i = children.size(); i > 0; i--)
			RaycastAll(children[i - 1], ray, results);

		if (traits.Intersects(object, ray))
			results.push_back(object);
	}

	std::optional<ObjectType> RaycastBest(ObjectType object, const Ray3f& ray)
	{
		if (!traits.IsSelectable(object))
			return std::nullopt;

		csl::ut::MoveArray<ObjectType> children{ hh::fnd::MemoryRouter::GetTempAllocator() };
		traits.GetChildren(object, children);

		for (size_t i = children.size(); i > 0; i--)
			if (auto childPick = RaycastBest(children[i - 1], ray))
				return childPick;

		if (traits.Intersects(object, ray))
			return std::make_optional(object);

		return std::nullopt;
	}
};
