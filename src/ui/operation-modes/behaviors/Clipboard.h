#pragma once
#include <ui/operation-modes/OperationModeBehavior.h>
#include "ForwardDeclarations.h"
#include "Selection.h"

template<typename OpModeContext>
class Clipboard : CompatibleObject {
public:
	using Traits = ClipboardTraits<OpModeContext>;
	using Entry = typename Traits::Entry;

	struct ClipboardEntry {
		Entry entry;
		int parent;
	};

private:
	csl::ut::MoveArray<ClipboardEntry> entries{ GetAllocator() };
	Traits traits;

	static int IndexOf(const csl::ut::MoveArray<typename SelectionBehaviorTraits<OpModeContext>::ObjectType>& objs, typename SelectionBehaviorTraits<OpModeContext>::ObjectType obj) {
		for (int i = 0; i < objs.size(); i++)
			if (objs[i] == obj)
				return i;

		return -1;
	}

public:
	Clipboard(csl::fnd::IAllocator* allocator, OpModeContext& context) : CompatibleObject{ allocator }, traits{ context } {}

	void Copy(const csl::ut::MoveArray<typename SelectionBehaviorTraits<OpModeContext>::ObjectType>& objects) {
		Clear();
		for (auto object : objects)
			entries.push_back({ .entry = traits.CreateClipboardEntry(object), .parent = traits.IsRoot(object) ? -1 : IndexOf(objects, traits.GetParent(object)) });
	}

	void Paste(csl::ut::MoveArray<typename SelectionBehaviorTraits<OpModeContext>::ObjectType>& results) {
		csl::ut::MoveArray<size_t> depths{ GetAllocator() };
		depths.reserve(entries.size());
		for (auto& entry : entries) {
			size_t depth{};
			int curParent{ entry.parent };

			while (curParent != -1) {
				curParent = entries[curParent].parent;
				depth++;
			}

			depths.push_back(depth);
		}

		csl::ut::MoveArray<size_t> indices{ GetAllocator() };
		indices.reserve(entries.size());
		for (size_t i = 0; i < entries.size(); i++)
			indices.push_back(i);

		std::sort(indices.begin(), indices.end(), [&depths](size_t idx1, size_t idx2) { return depths[idx1] < depths[idx2]; });

		size_t startIdx = results.size();
		for (auto& index : indices)
			results.push_back(traits.CreateObject(entries[index].entry, entries[index].parent == -1 ? std::nullopt : std::make_optional(results[startIdx + indices.find(entries[index].parent)])));
	}

	void Clear() {
		entries.clear();
	}

	size_t Size() const {
		return entries.size();
	}
};

template<typename OpModeContext>
class ClipboardBehavior : public OperationModeBehavior {
public:
	using Traits = ClipboardBehaviorTraits<OpModeContext>;

private:
	Clipboard<OpModeContext> clipboard;
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
		clipboard.Copy(operationMode.GetBehavior<SelectionBehavior<OpModeContext>>()->GetSelection());
	}

	void HandlePaste() {
		if (!traits.CanPlace())
			return;

		csl::ut::MoveArray<typename SelectionBehaviorTraits<OpModeContext>::ObjectType> newObjects{ GetAllocator() };
		clipboard.Paste(newObjects);
		operationMode.GetBehavior<SelectionBehavior<OpModeContext>>()->Select(newObjects);
	}

public:
	using CutAction = Action<ActionId::CUT>;
	using CopyAction = Action<ActionId::COPY>;
	using PasteAction = Action<ActionId::PASTE>;

	ClipboardBehavior(csl::fnd::IAllocator* allocator, OperationMode<OpModeContext>& operationMode) : OperationModeBehavior{ allocator, operationMode }, clipboard{ allocator, operationMode.GetContext() }, traits{ operationMode.GetContext() } {}

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
		clipboard.Clear();
	}
};
