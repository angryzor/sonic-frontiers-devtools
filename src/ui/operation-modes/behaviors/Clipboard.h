#pragma once
#include <ui/operation-modes/OperationModeBehavior.h>
#include "Selection.h"

template<typename T>
struct ClipboardEntry;

template<typename T>
class ClipboardBehavior : public OperationModeBehavior {
public:
	class Operations {
	public:
		virtual ClipboardEntry<T> CreateClipboardEntry(const T object) = 0;
		virtual T CreateObject(const ClipboardEntry<T>& entry) = 0;
	};

private:
	csl::ut::MoveArray<ClipboardEntry<T>> clipboard{ GetAllocator() };
	Operations& operations;

public:
	using CopyAction = Action<ActionId::COPY>;
	using PasteAction = Action<ActionId::PASTE>;

	ClipboardBehavior(csl::fnd::IAllocator* allocator, OperationMode& operationMode, Operations& operations) : OperationModeBehavior{ allocator, operationMode }, operations{ operations } {}

	~ClipboardBehavior() {
		Clear();
	}

	static constexpr unsigned int id = 0;
	virtual unsigned int GetId() override { return id; }

	virtual void ProcessAction(const ActionBase& action) override {
		switch (action.id) {
		case CopyAction::id:
			Clear();

			for (auto object : operationMode.GetBehavior<SelectionBehavior<T>>()->GetSelection())
				clipboard.push_back(operations.CreateClipboardEntry(object));

			break;
		case PasteAction::id: {
			csl::ut::MoveArray<T> newObjects{ hh::fnd::MemoryRouter::GetTempAllocator() };

			for (auto& entry : clipboard)
				newObjects.push_back(operations.CreateObject(entry));

			operationMode.GetBehavior<SelectionBehavior<T>>()->Select(newObjects);
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
