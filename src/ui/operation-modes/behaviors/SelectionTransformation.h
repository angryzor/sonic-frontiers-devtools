#pragma once
#include <ui/common/editors/Basic.h>
#include <ui/operation-modes/OperationModeBehavior.h>
#include "ForwardDeclarations.h"
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
		Dispatch(SetSelectionTransformAction{ transform });
	}

	void TransformSelectionTransform(const TransformType transform) {
		Dispatch(SetSelectionTransformAction{ transform * GetSelectionTransform() });
	}
};

template<typename OpModeContext>
class SelectionTransformationBehavior : public SelectionTransformationBehaviorBase<SelectionTransformationBehaviorTraits<OpModeContext>::Projective> {
public:
	using Traits = SelectionTransformationBehaviorTraits<OpModeContext>;
	using ObjectType = typename SelectionBehaviorTraits<OpModeContext>::ObjectType;

private:
	Traits traits;

protected:
	enum class PivotType {
		AABB_CENTER,
		LOCAL,
	};
	inline static const char* pivotTypeNames[]{ "AABB Center", "Local" };

	csl::ut::MoveArray<ObjectType> transformableSelection{ SelectionTransformationBehavior::GetAllocator() };
	PivotType pivotType{};

	/*
	 * Takes the original selection and returns a subset of objects that should be affected by a transform operation.
	 *
	 * This includes removing objects that cannot be transformed, and objects that already have an ancestor present
	 * in the selection and would otherwise be transformed multiple times.
	 */
	void CalculateTransformableSelection() {
		auto& selection = this->operationMode.GetBehavior<SelectionBehavior<OpModeContext>>()->GetSelection();

		transformableSelection.clear();

		for (auto& object : selection)
			if (traits.HasTransform(object) && !HasParentInSelection(object, selection))
				transformableSelection.push_back(object);
	}

	bool HasParentInSelection(ObjectType object, const csl::ut::MoveArray<ObjectType>& selection) {
		if (traits.IsRoot(object))
			return false;

		ObjectType parent = traits.GetParent(object);

		for (auto& obj2 : selection)
			if (obj2 == parent)
				return true;

		return HasParentInSelection(parent, selection);
	}

	typename SelectionTransformationBehavior::TransformType GetPivotTransform(const typename SelectionTransformationBehavior::TransformType& selectionTransform, const typename SelectionTransformationBehavior::TransformType& objectTransform) {
		switch (pivotType) {
		case PivotType::AABB_CENTER: return SelectionTransformationBehavior::TransformType::Identity();
		case PivotType::LOCAL: return objectTransform * selectionTransform.inverse();
		}
	}

public:
	SelectionTransformationBehavior(csl::fnd::IAllocator* allocator, OperationMode<OpModeContext>& operationMode) : SelectionTransformationBehaviorBase<Traits::Projective>{ allocator, operationMode }, traits{ operationMode.GetContext() } {}

	virtual void ProcessAction(const ActionBase& action) override {
		switch (action.id) {
		case SelectionBehavior<OpModeContext>::SelectAction::id:
		case SelectionBehavior<OpModeContext>::DeselectAction::id:
		case SelectionBehavior<OpModeContext>::SelectAllAction::id:
		case SelectionBehavior<OpModeContext>::DeselectAllAction::id:
			CalculateTransformableSelection();
			break;
		case SelectionTransformationBehavior::SetSelectionTransformAction::id:
			auto selectionTransform = GetSelectionTransform();
			typename SelectionTransformationBehavior::TransformType deltaTransform{ static_cast<const SelectionTransformationBehavior::SetSelectionTransformAction&>(action).payload * selectionTransform.inverse() };

			for (auto& object : transformableSelection) {
				auto objectTransform = traits.GetSelectionSpaceTransform(object);
				typename SelectionTransformationBehavior::TransformType pivotTransform{ GetPivotTransform(selectionTransform, objectTransform) };

				traits.SetSelectionSpaceTransform(object, pivotTransform * deltaTransform * pivotTransform.inverse() * objectTransform);
			}

			this->Dispatch(typename SelectionTransformationBehavior::SelectionTransformChangedAction{});
			break;
		}
	}

	virtual void Render() override {
		if (ImGui::Begin("Main menu")) {
			ImGui::SameLine();
			ComboEnum("Pivot", pivotType, pivotTypeNames);
		}
		ImGui::End();
	}

	csl::ut::MoveArray<ObjectType>& GetTransformableSelection() const {
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

		auto* selAabb = this->operationMode.GetBehavior<SelectionAabbBehavior<OpModeContext>>();

		if (selAabb && transformableSelection.size() > 1 && selAabb->HaveAabb())
			return Eigen::Affine3f{ Eigen::Translation3f{ selAabb->GetAabb().Center() } };
		else
			return traits.GetSelectionSpaceTransform(transformableSelection[0]);
	}
};
