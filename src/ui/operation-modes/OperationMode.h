#pragma once
#include <ui/Component.h>
#include "OperationModeBehavior.h"
#include "Panel.h"

//template<typename Context, typename T>
//class WithContext : public T {
//protected:
//	Context& context;
//
//public:
//	WithContext(Context& context) : context(context), T{} {}
//};
//
//template<typename Context, typename Behavior>
//class BehaviorContext : public WithContext<Context, Behavior::Operations> {
//	using WithContext::WithContext;
//};

template<typename Context>
class BehaviorTraitsImpl {
protected:
	typename Context& context;

public:
	BehaviorTraitsImpl(typename Context& context) : context{ context } {}
};

class OperationModeBase : public Component {
	OperationModeBehavior* draggingBehavior{ nullptr };
	OperationModeBehavior* singleFrameExclusiveMouseControlBehavior{ nullptr };

protected:
	csl::ut::MoveArray<hh::fnd::Reference<OperationModeBehavior>> behaviors{ GetAllocator() };

public:
	using Component::Component;

	virtual void ProcessAction(const ActionBase& action) override;
	virtual void Render() override;

	void InitBehaviors();
	void DeinitBehaviors();
	bool CanTakeMouseControl(OperationModeBehavior* behavior);
	void ToggleDragging(OperationModeBehavior* behavior, bool canStart = true);
	bool IsDragging(OperationModeBehavior* behavior);
	void BeginSingleFrameExclusiveMouseControl(OperationModeBehavior* behavior);

	template<typename T>
	T* GetBehavior() {
		for (auto& behavior : behaviors)
			if (behavior->GetId() == T::id)
				return static_cast<T*>(&*behavior);

		return nullptr;
	}
};

template<typename _Context>
class OperationMode : public OperationModeBase {
public:
	using Context = _Context;

private:
	//template<typename Behavior, typename TraitImpls, typename I = std::make_index_sequence<std::tuple_size<TraitImpls>::value>>
	//struct BehaviorWithTraitsImpls;

	//template<typename Behavior, typename TraitImpls, size_t... Is>
	//struct BehaviorWithTraitsImpls<Behavior, TraitImpls, std::index_sequence<Is...>> : public BehaviorWithTraitsImplsBase {
	//	Behavior inlineBehavior;
	//	TraitImpls traitImpls;

	//	BehaviorWithTraitsImpls(csl::fnd::IAllocator* allocator, OperationMode& operationMode)
	//		: traitImpls{ std::make_tuple(std::tuple_element_t<Is, TraitImpls>{ operationMode.context }...) }
	//		, inlineBehavior{ allocator, operationMode, std::get<Is>(traitImpls)... }
	//		, BehaviorWithTraitsImplsBase{ allocator, &inlineBehavior } {}
	//};

	friend class Panel<Context>;
	csl::ut::MoveArray<hh::fnd::Reference<Panel<Context>>> panels{ GetAllocator() };
	Context context{ GetAllocator() };
public:
	Context& GetContext() { return context; }
	using OperationModeBase::OperationModeBase;

	virtual void ProcessAction(const ActionBase& action) override {
		OperationModeBase::ProcessAction(action);

		for (auto& panel : panels)
			panel->ProcessAction(action);

	}

	virtual void Render() override {
		OperationModeBase::Render();

		for (auto& panel : panels)
			panel->Render();
	}

	//template<typename Behavior>
	//void AddBehavior() {
	//	behaviors.push_back(new (std::align_val_t(Behavior), GetAllocator()) BehaviorWithTraitsImpls<Behavior, std::tuple<>>{ GetAllocator(), * this });
	//}

	//template<template<typename> typename Behavior, typename ...Args>
	//void AddBehavior() {
	//	behaviors.push_back(new (std::align_val_t(alignof(BehaviorWithTraitsImpls<Behavior<Context>, std::tuple<Args...>>)), GetAllocator()) BehaviorWithTraitsImpls<Behavior<Context>, std::tuple<Args...>>{ GetAllocator(), * this });
	//}

	template<typename Behavior>
	void AddBehavior() {
		behaviors.push_back(new (std::align_val_t(alignof(Behavior)), GetAllocator()) Behavior{ GetAllocator(), *this });
	}

	template<template<typename> typename Behavior>
	void AddBehavior() {
		behaviors.push_back(new (std::align_val_t(alignof(Behavior<Context>)), GetAllocator()) Behavior<Context>{ GetAllocator(), *this });
	}

	template<typename T>
	void RemoveBehavior() {
		for (size_t i = 0; i < behaviors.size(); i++)
			if (behaviors[i]->behavior->GetId() == T::id) {
				behaviors.remove(i);
				break;
			}
	}

	template<typename T>
	void AddPanel() {
		panels.push_back(new (std::align_val_t(alignof(T)), GetAllocator()) T{ GetAllocator(), *this });
	}
};
