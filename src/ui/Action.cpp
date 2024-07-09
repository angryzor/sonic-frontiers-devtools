#include "Action.h"
#include <ui/Desktop.h>

void ActionDispatcher::Dispatch(const ActionBase& action)
{
	Desktop::instance->Dispatch(action);
}

void ActionProcessor::ProcessAction(const ActionBase& action)
{
}
