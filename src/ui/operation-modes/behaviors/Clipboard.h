#pragma once
#include <ui/operation-modes/OperationModeBehavior.h>
#include "ForwardDeclarations.h"
#include "Selection.h"

template<typename OpModeContext>
class ClipboardBehavior : public OperationModeBehavior {
public:
	using Traits = ClipboardBehaviorTraits<OpModeContext>;

private:
	csl::ut::MoveArray<ClipboardEntry<OpModeContext>> clipboard{ GetAllocator() };
	Traits traits;

public:
	using CopyAction = Action<ActionId::COPY>;
	using PasteAction = Action<ActionId::PASTE>;

	ClipboardBehavior(csl::fnd::IAllocator* allocator, OperationMode<OpModeContext>& operationMode) : OperationModeBehavior{ allocator, operationMode }, traits{ operationMode.GetContext() } {}

	~ClipboardBehavior() {
		Clear();
	}

	static constexpr unsigned int id = 0;
	virtual unsigned int GetId() override { return id; }

	virtual void Init() override {
		Desktop::instance->BindShortcut<CopyAction>(ShortcutId::COPY);
		Desktop::instance->BindShortcut<PasteAction>(ShortcutId::PASTE);
	}

	virtual void Deinit() override {
		Desktop::instance->UnbindShortcut(ShortcutId::COPY);
		Desktop::instance->UnbindShortcut(ShortcutId::PASTE);
	}

	virtual void ProcessAction(const ActionBase& action) override {
		switch (action.id) {
		case CopyAction::id:
			Clear();

			for (auto object : operationMode.GetBehavior<SelectionBehavior<OpModeContext>>()->GetSelection())
				clipboard.push_back(traits.CreateClipboardEntry(object));

			break;
		case PasteAction::id: {
			csl::ut::MoveArray<typename SelectionBehaviorTraits<OpModeContext>::ObjectType> newObjects{ hh::fnd::MemoryRouter::GetTempAllocator() };

			for (auto& entry : clipboard)
				newObjects.push_back(traits.CreateObject(entry));

			operationMode.GetBehavior<SelectionBehavior<OpModeContext>>()->Select(newObjects);
			break;
		}
		}
	}

	void Copy() {
		Desktop::instance->Dispatch(CopyAction{});
	}

	void Paste() {
		Desktop::instance->Dispatch(PasteAction{});
	}

	void Clear() {
		clipboard.clear();
	}
};
