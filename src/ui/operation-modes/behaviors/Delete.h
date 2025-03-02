#pragma once
#include <ui/Desktop.h>
#include <ui/operation-modes/OperationModeBehavior.h>
#include "ForwardDeclarations.h"

template<typename OpModeContext>
class DeleteBehavior : public OperationModeBehavior {
public:
	using Traits = DeleteBehaviorTraits<OpModeContext>;

private:
	Traits traits;

public:
	DeleteBehavior(csl::fnd::IAllocator* allocator, OperationMode<OpModeContext>& operationMode) : OperationModeBehavior{ allocator, operationMode }, traits{ operationMode.GetContext() } {}

	using DeleteAction = Action<ActionId::DELETE>;

	static constexpr unsigned int id = 10;
	virtual unsigned int GetId() override { return id; }

	virtual void Init() {
		Desktop::instance->BindShortcut<DeleteAction>(ShortcutId::DELETE);
	}

	virtual void Deinit() {
		Desktop::instance->UnbindShortcut(ShortcutId::DELETE);
	}

	virtual void ProcessAction(const ActionBase& action) override {
		switch (action.id) {
		case DeleteAction::id: {
			auto* selectionBehavior = operationMode.GetBehavior<SelectionBehavior<OpModeContext>>();

			csl::ut::MoveArray<typename SelectionBehavior<OpModeContext>::ObjectType> tmpArr{hh::fnd::MemoryRouter::GetTempAllocator()};

			for (auto obj : selectionBehavior->GetSelection())
				tmpArr.push_back(obj);

			selectionBehavior->DeselectAll();
			traits.DeleteObjects(tmpArr);
			Dispatch(SceneChangedAction{});
		}
		}
	}
};
