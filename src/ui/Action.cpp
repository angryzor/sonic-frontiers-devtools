#include "Action.h"
#include <ui/Desktop.h>

void ActionDispatcher::Dispatch(const ActionBase& action)
{
	Desktop::instance->Dispatch(action);
}

void ActionProcessor::ProcessAction(const ActionBase& action)
{
}

ActionBase::ActionBase(ActionId id) : id{ id } {
}

AsyncActionBase* ActionBase::CreateAsync(csl::fnd::IAllocator* allocator) const & {
	assert(false && "Cannot create async version of this action.");
	return nullptr;
}

//AsyncActionBase* ActionBase::CreateAsync(csl::fnd::IAllocator* allocator) && {
//	assert(false && "Cannot create async version of this action.");
//	return nullptr;
//}
