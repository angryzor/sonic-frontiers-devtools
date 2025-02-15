#pragma once
#include <ui/Action.h>
#include <ui/Shortcuts.h>
#include <utilities/CompatibleObject.h>

class OperationModeBase;
class OperationModeBehavior : public CompatibleObject {
protected:
	OperationModeBase& operationMode;

public:
	OperationModeBehavior(csl::fnd::IAllocator* allocator, OperationModeBase& operationMode);

	virtual unsigned int GetId() = 0;
	virtual void Init();
	virtual void Deinit();
	void Dispatch(const ActionBase& action);
	//void Dispatch(ActionBase&& action);
	virtual void ProcessAction(const ActionBase& action);
	virtual void Render();
};
