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

	struct SelectionChangedPayload {
		csl::ut::MoveArray<T> previousSelection;
		csl::ut::MoveArray<T> currentSelection;
		csl::ut::MoveArray<T> selected;
		csl::ut::MoveArray<T> deselected;
	};
	using SelectionChangedAction = Action<ActionId::SELECTION_CHANGED, SelectionChangedPayload>;

	using OperationModeBehavior::OperationModeBehavior;

	void Init() {
		Desktop::instance->BindShortcut<DeselectAllAction>(ShortcutId::ESCAPE);
	}

	void Deinit() {
		Desktop::instance->UnbindShortcut(ShortcutId::ESCAPE);
	}

	virtual void ProcessAction(const ActionBase& action) override {
		switch (action.id) {
		case SelectAction::id: {
			auto& selected = static_cast<const SelectAction&>(action).payload;

			csl::ut::MoveArray<T> previousSelection{ hh::fnd::MemoryRouter::GetTempAllocator() };
			for (auto& object : selection)
				previousSelection.push_back(object);

			for (auto& object : static_cast<const SelectAction&>(action).payload)
				selection.push_back(object);

			Dispatch(SelectionChangedAction{ { previousSelection, selection, selected, {} } });
			break;
		}
		case DeselectAction::id: {
			auto& deselected = static_cast<const DeselectAction&>(action).payload;

			csl::ut::MoveArray<T> previousSelection{ hh::fnd::MemoryRouter::GetTempAllocator() };
			for (auto& object : selection)
				previousSelection.push_back(object);

			for (auto& object : deselected) {
				auto it = selection.find(object);
				selection.remove(it);
			}

			Dispatch(SelectionChangedAction{ { previousSelection, selection, {}, deselected } });
			break;
		}
		case SelectAllAction::id:
			assert(false);
			break;
		case DeselectAllAction::id: {
			csl::ut::MoveArray<T> previousSelection{ hh::fnd::MemoryRouter::GetTempAllocator() };
			for (auto& object : selection)
				previousSelection.push_back(object);

			selection.clear();

			Dispatch(SelectionChangedAction{ { previousSelection, selection, {}, previousSelection } });
			break;
		}
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
		Dispatch(SelectAction{ objects });
	}

	void Deselect(const T& object) {
		csl::ut::MoveArray<T> objects{ hh::fnd::MemoryRouter::GetTempAllocator() };
		objects.push_back(object);
		Deselect(objects);
	}

	void Deselect(const csl::ut::MoveArray<T>& objects) {
		Dispatch(DeselectAction{ objects });
	}

	void SelectAll() {
		//Dispatch(SelectAllAction{});
	}

	void DeselectAll() {
		Dispatch(DeselectAllAction{});
	}

	const csl::ut::MoveArray<T>& GetSelection() const {
		return selection;
	}
	
	bool IsSelected(T object) const {
		return selection.find(object) != -1;
	}
};
