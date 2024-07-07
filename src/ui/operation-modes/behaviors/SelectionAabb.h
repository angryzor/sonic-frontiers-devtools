#pragma once
#include <ui/operation-modes/OperationModeBehavior.h>
#include "Selection.h"

class SelectionAabbBehaviorBase : public OperationModeBehavior {
protected:
	csl::geom::Aabb aabb{};
	bool haveAabb{ false };

public:
	static constexpr unsigned int id = 6;
	virtual unsigned int GetId() override { return id; }

	using OperationModeBehavior::OperationModeBehavior;

	bool HaveAabb() const {
		return haveAabb;
	}

	const csl::geom::Aabb& GetAabb() const {
		return aabb;
	}
};

template<typename T>
class SelectionAabbBehavior : public SelectionAabbBehaviorBase {
public:
	struct Operations {
		virtual bool CalculateAabb(const csl::ut::MoveArray<T>& objects, csl::geom::Aabb& aabb) = 0;
	};

private:
	Operations& operations;

public:
	SelectionAabbBehavior(csl::fnd::IAllocator* allocator, OperationMode& operationMode, Operations& operations)
		: SelectionAabbBehaviorBase{ allocator, operationMode }, operations{ operations } {}

	virtual void Render() override {
		haveAabb = operations.CalculateAabb(operationMode.GetBehavior<SelectionBehavior<T>>()->GetSelection(), aabb);
	}
};
