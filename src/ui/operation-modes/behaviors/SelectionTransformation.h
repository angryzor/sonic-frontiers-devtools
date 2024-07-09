#pragma once
#include <ui/Desktop.h>
#include <ui/operation-modes/OperationModeBehavior.h>
#include "Selection.h"
#include "SelectionAabb.h"

template<bool Projective = false>
class SelectionTransformationBehaviorBase : public OperationModeBehavior {
public:
	typedef std::conditional_t<Projective, Eigen::Projective3f, Eigen::Affine3f> TransformType;

	static constexpr unsigned int id = 7;
	virtual unsigned int GetId() override { return id; }

	using SetSelectionTransformAction = Action<ActionId::SET_SELECTION_TRANSFORM, TransformType>;
	using SelectionTransformChangedAction = Action<ActionId::SELECTION_TRANSFORM_CHANGED, TransformType>;

	using OperationModeBehavior::OperationModeBehavior;

	virtual bool HasTransformableSelection() const = 0;
	virtual TransformType GetSelectionTransform() const = 0;

	void SetSelectionTransform(const TransformType transform) {
		Desktop::instance->Dispatch(SetSelectionTransformAction{ transform });
	}

	void TransformSelectionTransform(const TransformType transform) {
		Desktop::instance->Dispatch(SetSelectionTransformAction{ transform * GetSelectionTransform() });
	}
};

template<typename T, bool Projective = false>
class SelectionTransformationBehavior : public SelectionTransformationBehaviorBase<Projective> {
public:
	class Operations {
	public:
		virtual bool HasTransform(const T object) = 0;
		virtual bool IsRoot(const T object) = 0;
		virtual T GetParent(const T object) = 0;
		virtual typename SelectionTransformationBehavior::TransformType GetSelectionSpaceTransform(const T object) = 0;
		virtual void SetSelectionSpaceTransform(T object, const typename SelectionTransformationBehavior::TransformType& transform) = 0;
	};

private:
	Operations& operations;

protected:
	csl::ut::MoveArray<T> transformableSelection{ SelectionTransformationBehavior::GetAllocator() };

	/*
	 * Takes the original selection and returns a subset of objects that should be affected by a transform operation.
	 *
	 * This includes removing objects that cannot be transformed, and objects that already have an ancestor present
	 * in the selection and would otherwise be transformed multiple times.
	 */
	void CalculateTransformableSelection() {
		auto& selection = this->operationMode.GetBehavior<SelectionBehavior<T>>()->GetSelection();

		transformableSelection.clear();

		for (auto& object : selection)
			if (operations.HasTransform(object) && !HasParentInSelection(object, selection))
				transformableSelection.push_back(object);
	}

	bool HasParentInSelection(T object, const csl::ut::MoveArray<T>& selection) {
		if (operations.IsRoot(object))
			return false;

		T parent = operations.GetParent(object);

		for (auto& obj2 : selection)
			if (obj2 == parent)
				return true;

		return HasParentInSelection(parent, selection);
	}

public:
	SelectionTransformationBehavior(csl::fnd::IAllocator* allocator, OperationModeBase& operationMode, Operations& operations) : SelectionTransformationBehaviorBase<Projective>{ allocator, operationMode }, operations{ operations } {}

	virtual void ProcessAction(const ActionBase& action) override {
		switch (action.id) {
		case SelectionBehavior<T>::SelectAction::id:
		case SelectionBehavior<T>::DeselectAction::id:
		case SelectionBehavior<T>::SelectAllAction::id:
		case SelectionBehavior<T>::DeselectAllAction::id:
			CalculateTransformableSelection();
			break;
		case SelectionTransformationBehavior::SetSelectionTransformAction::id:
			typename SelectionTransformationBehavior::TransformType deltaTransform{ static_cast<const SelectionTransformationBehavior::SetSelectionTransformAction&>(action).payload * GetSelectionTransform().inverse() };

			for (auto& object : transformableSelection)
				operations.SetSelectionSpaceTransform(object, deltaTransform * operations.GetSelectionSpaceTransform(object));

			this->Dispatch(typename SelectionTransformationBehavior::SelectionTransformChangedAction{});
			break;
		}
	}

	csl::ut::MoveArray<T>& GetTransformableSelection() const {
		return transformableSelection;
	}

	virtual bool HasTransformableSelection() const override {
		return transformableSelection.size() > 0;
	}

	/*
	 * Calculates a pivot based on the selection. If there is an AABB, we take the center, otherwise fall back to the first item.
	 */
	virtual typename SelectionTransformationBehavior::TransformType GetSelectionTransform() const override {
		assert(transformableSelection.size() > 0);

		auto* selAabb = this->operationMode.GetBehavior<SelectionAabbBehavior<T>>();

		if (selAabb && transformableSelection.size() > 1 && selAabb->HaveAabb())
			return Eigen::Affine3f{ Eigen::Translation3f{ selAabb->GetAabb().Center() } };
		else
			return operations.GetSelectionSpaceTransform(transformableSelection[0]);
	}
};
