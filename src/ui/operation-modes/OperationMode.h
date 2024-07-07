#pragma once
#include <utilities/CompatibleObject.h>
#include "OperationModeBehavior.h"

class OperationMode : public CompatibleObject {
	csl::ut::MoveArray<hh::fnd::Reference<OperationModeBehavior>> behaviors{ GetAllocator() };
	OperationModeBehavior* draggingBehavior{ nullptr };
	OperationModeBehavior* singleFrameExclusiveMouseControlBehavior{ nullptr };

public:
	OperationMode(csl::fnd::IAllocator* allocator);

	void RenderOperationMode();
	virtual void Render();

	template<typename T>
	T* GetBehavior() {
		for (auto& behavior : behaviors)
			if (behavior->GetId() == T::id)
				return static_cast<T*>(&*behavior);

		return nullptr;
	}

	void InitBehaviors();
	void DeinitBehaviors();
	void ProcessAction(const ActionBase& action);
	bool CanTakeMouseControl(OperationModeBehavior* behavior);
	void ToggleDragging(OperationModeBehavior* behavior, bool canStart = true);
	bool IsDragging(OperationModeBehavior* behavior);
	void BeginSingleFrameExclusiveMouseControl(OperationModeBehavior* behavior);

	template<typename T, typename ...Args> void AddBehavior(Args&&... args) {
		behaviors.push_back(new (std::align_val_t(alignof(T)), GetAllocator()) T{ GetAllocator(), *this, std::forward<Args>(args)... });
	}
};
