#include "OperationModeBehavior.h"

OperationModeBehavior::OperationModeBehavior(csl::fnd::IAllocator* allocator, OperationModeBase& operationMode) : Component{ allocator }, operationMode{ operationMode } {}

void OperationModeBehavior::Init()
{
}

void OperationModeBehavior::Deinit()
{
}
