#pragma once
#include <ui/Desktop.h>
#include <ui/operation-modes/OperationModeBehavior.h>

template<typename T>
class DeleteBehavior : public OperationModeBehavior {
public:
	struct Operations {
		virtual void DeleteObjects(const csl::ut::MoveArray<T>& objects) = 0;
	};

private:
	Operations& operations;

public:
	DeleteBehavior(csl::fnd::IAllocator* allocator, OperationMode& operationMode, Operations& operations) : OperationModeBehavior{ allocator, operationMode }, operations{ operations } {}

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
			auto* selection = operationMode.GetBehavior<SelectionBehavior<T>>();

			csl::ut::MoveArray<T> tmpArr{ hh::fnd::MemoryRouter::GetTempAllocator() };

			for (auto obj : selection->GetSelection())
				tmpArr.push_back(obj);

			selection->DeselectAll();
			operations.DeleteObjects(tmpArr);
		}
		}
	}
};