#pragma once
#include <utilities/math/Ray.h>
#include <utilities/math/Frustum.h>
#include <ui/operation-modes/OperationModeBehavior.h>
#include <ui/Desktop.h>
#include "ForwardDeclarations.h"

template<typename OpModeContext>
class SelectionBehavior : public OperationModeBehavior {
public:
	using ObjectType = typename SelectionBehaviorTraits<OpModeContext>::ObjectType;

protected:
	csl::ut::MoveArray<ObjectType> selection{ GetAllocator() };

public:
	static constexpr unsigned int id = 5;
	virtual unsigned int GetId() override { return id; }

	using SelectAction = Action<ActionId::SELECT, csl::ut::MoveArray<ObjectType>>;
	using DeselectAction = Action<ActionId::DESELECT, csl::ut::MoveArray<ObjectType>>;
	using SelectAllAction = Action<ActionId::SELECT_ALL>;
	using DeselectAllAction = Action<ActionId::DESELECT_ALL>;

	struct SelectionChangedPayload {
		const csl::ut::MoveArray<ObjectType> previousSelection;
		const csl::ut::MoveArray<ObjectType> currentSelection;
		const csl::ut::MoveArray<ObjectType> selected;
		const csl::ut::MoveArray<ObjectType> deselected;
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

			csl::ut::MoveArray<ObjectType> previousSelection{ GetAllocator() };
			for (auto& object : selection)
				previousSelection.push_back(object);

			for (auto& object : selected)
				selection.push_back(object);

			Dispatch(SelectionChangedAction{ { previousSelection, selection, selected, {} } });
			break;
		}
		case DeselectAction::id: {
			auto& deselected = static_cast<const DeselectAction&>(action).payload;

			csl::ut::MoveArray<ObjectType> previousSelection{ GetAllocator() };
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
			csl::ut::MoveArray<ObjectType> previousSelection{ GetAllocator() };
			for (auto& object : selection)
				previousSelection.push_back(object);

			selection.clear();

			Dispatch(SelectionChangedAction{ { previousSelection, selection, {}, previousSelection } });
			break;
		}
		}
	}

	void Select(ObjectType object) {
		DeselectAll();
		AddToSelection(object);
	}

	void Select(const csl::ut::MoveArray<ObjectType>& objects) {
		DeselectAll();
		AddToSelection(objects);
	}

	void AddToSelection(ObjectType object) {
		csl::ut::MoveArray<ObjectType> objects{ GetAllocator() };
		objects.push_back(object);
		AddToSelection(objects);
	}

	void AddToSelection(const csl::ut::MoveArray<ObjectType>& objects) {
		Dispatch(SelectAction{ objects });
	}

	void ToggleSelection(ObjectType object) {
		csl::ut::MoveArray<ObjectType> objects{ GetAllocator() };
		objects.push_back(object);
		ToggleSelection(objects);
	}

	void ToggleSelection(const csl::ut::MoveArray<ObjectType>& objects) {
		csl::ut::MoveArray<ObjectType> objectsToSelect{ GetAllocator() };
		csl::ut::MoveArray<ObjectType> objectsToDeselect{ GetAllocator() };

		for (auto object : objects)
			(selection.find(object) == -1 ? objectsToSelect : objectsToDeselect).push_back(object);

		Dispatch(DeselectAction{ objectsToDeselect });
		Dispatch(SelectAction{ objectsToSelect });
	}

	void Deselect(ObjectType object) {
		csl::ut::MoveArray<ObjectType> objects{ GetAllocator() };
		objects.push_back(object);
		Deselect(objects);
	}

	void Deselect(const csl::ut::MoveArray<ObjectType>& objects) {
		Dispatch(DeselectAction{ objects });
	}

	void SelectAll() {
		//Dispatch(SelectAllAction{});
	}

	void DeselectAll() {
		Dispatch(DeselectAllAction{});
	}

	const csl::ut::MoveArray<ObjectType>& GetSelection() const {
		return selection;
	}
	
	bool IsSelected(ObjectType object) const {
		return selection.find(object) != -1;
	}
};
