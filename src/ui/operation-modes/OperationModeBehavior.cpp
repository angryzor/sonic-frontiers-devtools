#include "OperationModeBehavior.h"

OperationModeBehavior::OperationModeBehavior(csl::fnd::IAllocator* allocator, OperationMode& operationMode) : CompatibleObject{ allocator }, operationMode{ operationMode } {}

OperationMode& OperationModeBehavior::GetOperationMode()
{
	return operationMode;
}
