#pragma once
#include <ui/operation-modes/OperationModeBehavior.h>
#include "ForwardDeclarations.h"
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

template<typename OpModeContext>
class SelectionAabbBehavior : public SelectionAabbBehaviorBase {
public:
	using Traits = SelectionAabbBehaviorTraits<OpModeContext>;

private:
	Traits traits;

public:
	SelectionAabbBehavior(csl::fnd::IAllocator* allocator, OperationMode<OpModeContext>& operationMode)
		: SelectionAabbBehaviorBase{ allocator, operationMode }, traits{ operationMode.GetContext() } {}

	virtual void Render() override {
		haveAabb = traits.CalculateAabb(operationMode.GetBehavior<SelectionBehavior<OpModeContext>>()->GetSelection(), aabb);
	}
};
