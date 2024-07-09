#pragma once
#include <ui/Action.h>
#include <ui/Shortcuts.h>
#include <ui/Component.h>

class OperationModeBase;

class OperationModeBehavior : public Component {
protected:
	OperationModeBase& operationMode;

public:
	OperationModeBehavior(csl::fnd::IAllocator* allocator, OperationModeBase& operationMode);

	virtual unsigned int GetId() = 0;
	virtual void Init();
	virtual void Deinit();
};
