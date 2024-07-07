#pragma once
#include <utilities/math/Ray.h>
#include <utilities/math/Frustum.h>
#include <ui/operation-modes/OperationModeBehavior.h>
#include <ui/Desktop.h>

template<typename T>
class SelectionBehavior : public OperationModeBehavior {
protected:
	csl::ut::MoveArray<T> selection{ GetAllocator() };

public:
	static constexpr unsigned int id = 5;
	virtual unsigned int GetId() override { return id; }

	using SelectAction = Action<ActionId::SELECT, csl::ut::MoveArray<T>>;
	using DeselectAction = Action<ActionId::DESELECT, csl::ut::MoveArray<T>>;
	using SelectAllAction = Action<ActionId::SELECT_ALL>;
	using DeselectAllAction = Action<ActionId::DESELECT_ALL>;

	using OperationModeBehavior::OperationModeBehavior;

	void Init() {
		Desktop::instance->BindShortcut<DeselectAllAction>(ShortcutId::ESCAPE);
	}

	void Deinit() {
		Desktop::instance->UnbindShortcut(ShortcutId::ESCAPE);
	}

	virtual void ProcessAction(const ActionBase& action) override {
		switch (action.id) {
		case SelectAction::id:
			for (auto& object : static_cast<const SelectAction&>(action).payload)
				selection.push_back(object);
			break;
		case DeselectAction::id:
			for (auto& object : static_cast<const DeselectAction&>(action).payload) {
				auto it = selection.find(object);
				selection.remove(it);
			}
			break;
		case SelectAllAction::id:
			selection.clear();
			break;
		case DeselectAllAction::id:
			selection.clear();
			break;
		}
	}

	void Select(const T& object) {
		DeselectAll();
		AddToSelection(object);
	}

	void Select(const csl::ut::MoveArray<T>& objects) {
		DeselectAll();
		AddToSelection(objects);
	}

	void AddToSelection(const T& object) {
		csl::ut::MoveArray<T> objects{ hh::fnd::MemoryRouter::GetTempAllocator() };
		objects.push_back(object);
		AddToSelection(objects);
	}

	void AddToSelection(const csl::ut::MoveArray<T>& objects) {
		Desktop::instance->Dispatch(SelectAction{ objects });
	}

	void Deselect(const T& object) {
		csl::ut::MoveArray<T> objects{ hh::fnd::MemoryRouter::GetTempAllocator() };
		objects.push_back(object);
		Deselect(objects);
	}

	void Deselect(const csl::ut::MoveArray<T>& objects) {
		Desktop::instance->Dispatch(DeselectAction{ objects });
	}

	void SelectAll() {
		//Desktop::instance->Dispatch(SelectAllAction{});
	}

	void DeselectAll() {
		Desktop::instance->Dispatch(DeselectAllAction{});
	}

	const csl::ut::MoveArray<T>& GetSelection() const {
		return selection;
	}
};