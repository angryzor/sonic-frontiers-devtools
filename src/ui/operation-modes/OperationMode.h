#pragma once
#include <ui/Component.h>
#include "OperationModeBehavior.h"
#include "Panel.h"

class OperationModeBase : public Component {
	OperationModeBehavior* draggingBehavior{ nullptr };
	OperationModeBehavior* singleFrameExclusiveMouseControlBehavior{ nullptr };

protected:
	struct BehaviorWithContextBase : public CompatibleObject {
		OperationModeBehavior* behavior;

		BehaviorWithContextBase(csl::fnd::IAllocator* allocator, OperationModeBehavior* behavior) : CompatibleObject{ allocator }, behavior { behavior } {}
	};
	csl::ut::MoveArray<hh::fnd::Reference<BehaviorWithContextBase>> behaviors{ GetAllocator() };

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
			if (behavior->behavior->GetId() == T::id)
				return static_cast<T*>(&*behavior->behavior);

		return nullptr;
	}
};

template<typename Context>
class OperationMode : public OperationModeBase {
	template<typename Behavior, typename Contexts, typename I = std::make_index_sequence<std::tuple_size<Contexts>::value>>
	struct BehaviorWithContext;

	template<typename Behavior, typename Contexts, size_t... Is>
	struct BehaviorWithContext<Behavior, Contexts, std::index_sequence<Is...>> : public BehaviorWithContextBase {
		Contexts contexts;
		Behavior inlineBehavior;

		BehaviorWithContext(csl::fnd::IAllocator* allocator, OperationModeBase& operationMode, Context& context)
			: contexts{ std::make_tuple(std::tuple_element_t<Is, Contexts>{ context }...) }
			, inlineBehavior{ allocator, operationMode, std::get<Is>(contexts)... }
			, BehaviorWithContextBase{allocator, &inlineBehavior} {}
	};

	friend class Panel<Context>;
	csl::ut::MoveArray<hh::fnd::Reference<Panel<Context>>> panels{ GetAllocator() };
	Context context{ GetAllocator() };
protected:
	Context& GetContext() { return context; }
public:
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

	template<typename T, typename ...Args>
	void AddBehavior() {
		behaviors.push_back(new (std::align_val_t(alignof(BehaviorWithContext<T, std::tuple<Args...>>)), GetAllocator()) BehaviorWithContext<T, std::tuple<Args...>>{ GetAllocator(), *this, context });
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
