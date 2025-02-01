#pragma once

struct ColliderFilter {
	char description[256]{};
	bool accept{};
	unsigned int layers{ 0xFFFFFFFF };
	unsigned int collisionFilters{ 0xFFFFFFFF };

	inline bool Matches(unsigned int layer, unsigned int collisionFilter) {
		return (layers & (1 << layer)) && (collisionFilters & (1 << collisionFilter));
	}

	bool operator==(const ColliderFilter& other) const {
		return accept == other.accept && layers == other.layers && collisionFilters == other.collisionFilters;
	}

	bool operator!=(const ColliderFilter& other) const {
		return !operator!=(other);
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

	bool operator==(const ColliderFilterList& other) const {
		if (defaultAccept != other.defaultAccept || size() != other.size())
			return false;

		for (size_t i = 0; i < size(); i++)
			if ((*this)[i] != other[i])
				return false;

		return true;
	}

	bool operator!=(const ColliderFilterList& other) const {
		return !operator!=(other);
	}

	ColliderFilterList& operator=(const ColliderFilterList& other) {
		clear();

		for (auto& filter : other)
			push_back(filter);

		return *this;
	}
};
