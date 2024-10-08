#pragma once
#include <ui/Action.h>
#include <utilities/CompatibleObject.h>
#include "Panel.h"
#include "OperationModeHost.h"

class OperationModeBehavior;
class OperationModeBase : public CompatibleObject {
	OperationModeHost& host;
	OperationModeBehavior* draggingBehavior{ nullptr };
	OperationModeBehavior* singleFrameExclusiveMouseControlBehavior{ nullptr };

protected:
	csl::ut::MoveArray<hh::fnd::Reference<OperationModeBehavior>> behaviors{ GetAllocator() };

public:
	OperationModeBase(csl::fnd::IAllocator* allocator, OperationModeHost& host) : CompatibleObject{ allocator }, host{ host } {}

	void Dispatch(const ActionBase& action);
	virtual void ProcessAction(const ActionBase& action);
	virtual void Render();
	virtual void RenderScene();

	void InitBehaviors();
	void DeinitBehaviors();
	bool BeginOverlayWindow();
	void EndOverlayWindow();
	bool IsMouseOverSceneWindow();
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

template<typename Context>
class BehaviorTraitsImpl {
protected:
	typename Context& context;

public:
	BehaviorTraitsImpl(typename Context& context) : context{ context } {}
};

template<typename _Context>
class OperationMode : public OperationModeBase {
public:
	using Context = _Context;

private:
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
			if (behaviors[i]->GetId() == T::id) {
				behaviors.remove(i);
				break;
			}
	}

	template<typename T>
	void AddPanel() {
		panels.push_back(new (std::align_val_t(alignof(T)), GetAllocator()) T{ GetAllocator(), *this });
	}
};
