#pragma once

struct ColliderFilter {
	bool accept{};
	unsigned int layers{ 0xFFFFFFFF };
	unsigned int collisionFilters{ 0xFFFFFFFF };

	inline bool Matches(unsigned int layer, unsigned int collisionFilter) {
		return (layers & (1 << layer)) && (collisionFilters & (1 << collisionFilter));
	}
};

class ColliderFilterList : public csl::ut::MoveArray<ColliderFilter> {
public:
	bool defaultAccept{};

	inline bool Test(unsigned int layer, unsigned int collisionFilter) {
		for (auto& filter : *this)
			if (filter.Matches(layer, collisionFilter))
				return filter.accept;

		return defaultAccept;
	}
};
