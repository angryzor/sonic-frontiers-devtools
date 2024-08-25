#include "OperationModeBehavior.h"
#include "OperationMode.h"

OperationModeBehavior::OperationModeBehavior(csl::fnd::IAllocator* allocator, OperationModeBase& operationMode) : CompatibleObject{ allocator }, operationMode{ operationMode } {}

void OperationModeBehavior::Init() {
}

void OperationModeBehavior::Deinit() {
}

void OperationModeBehavior::Dispatch(const ActionBase& action) {
	operationMode.Dispatch(action);
}

void OperationModeBehavior::ProcessAction(const ActionBase& action) {
}

void OperationModeBehavior::Render() {
}
