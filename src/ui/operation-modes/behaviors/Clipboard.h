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

	void HandleCut() {
		HandleCopy();

		auto* selectionBehavior = operationMode.GetBehavior<SelectionBehavior<OpModeContext>>();
		csl::ut::MoveArray<typename SelectionBehaviorTraits<OpModeContext>::ObjectType> oldObjects{ GetAllocator() };

		for (auto object : selectionBehavior->GetSelection())
			oldObjects.push_back(object);

		selectionBehavior->DeselectAll();

		traits.DeleteObjects(oldObjects);
	}

	void HandleCopy() {
		Clear();

		for (auto object : operationMode.GetBehavior<SelectionBehavior<OpModeContext>>()->GetSelection())
			clipboard.push_back(traits.CreateClipboardEntry(object));
	}

	void HandlePaste() {
		if (!traits.CanPlace())
			return;

		csl::ut::MoveArray<typename SelectionBehaviorTraits<OpModeContext>::ObjectType> newObjects{ GetAllocator() };

		for (auto& entry : clipboard)
			newObjects.push_back(traits.CreateObject(entry));

		operationMode.GetBehavior<SelectionBehavior<OpModeContext>>()->Select(newObjects);
	}

public:
	using CutAction = Action<ActionId::CUT>;
	using CopyAction = Action<ActionId::COPY>;
	using PasteAction = Action<ActionId::PASTE>;

	ClipboardBehavior(csl::fnd::IAllocator* allocator, OperationMode<OpModeContext>& operationMode) : OperationModeBehavior{ allocator, operationMode }, traits{ operationMode.GetContext() } {}

	~ClipboardBehavior() {
		Clear();
	}

	static constexpr unsigned int id = 0;
	virtual unsigned int GetId() override { return id; }

	virtual void Init() override {
		Desktop::instance->BindShortcut<CutAction>(ShortcutId::CUT);
		Desktop::instance->BindShortcut<CopyAction>(ShortcutId::COPY);
		Desktop::instance->BindShortcut<PasteAction>(ShortcutId::PASTE);
	}

	virtual void Deinit() override {
		Desktop::instance->UnbindShortcut(ShortcutId::CUT);
		Desktop::instance->UnbindShortcut(ShortcutId::COPY);
		Desktop::instance->UnbindShortcut(ShortcutId::PASTE);
	}

	virtual void ProcessAction(const ActionBase& action) override {
		switch (action.id) {
		case CutAction::id: HandleCut(); break;
		case CopyAction::id: HandleCopy(); break;
		case PasteAction::id: HandlePaste(); break;
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
