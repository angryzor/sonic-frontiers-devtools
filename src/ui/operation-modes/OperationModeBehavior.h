#pragma once
#include <ui/Action.h>
#include <ui/Shortcuts.h>
#include <utilities/CompatibleObject.h>

class OperationMode;

class OperationModeBehavior : public CompatibleObject {
protected:
	OperationMode& operationMode;

public:
	OperationModeBehavior(csl::fnd::IAllocator* allocator, OperationMode& operationMode);

	OperationMode& GetOperationMode();

	virtual unsigned int GetId() = 0;
	virtual void Init() {}
	virtual void Deinit() {}
	virtual void ProcessAction(const ActionBase& action) {}
	virtual void Render() {}
};
