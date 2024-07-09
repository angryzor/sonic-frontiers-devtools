#include "OperationMode.h"

void OperationModeBase::Render()
{
	singleFrameExclusiveMouseControlBehavior = nullptr;

	for (auto& behavior : behaviors)
		behavior->behavior->Render();
}

void OperationModeBase::ProcessAction(const ActionBase& action)
{
	for (auto& behavior : behaviors)
		behavior->behavior->ProcessAction(action);
}

void OperationModeBase::InitBehaviors()
{
	for (auto& behavior : behaviors)
		behavior->behavior->Init();
}

void OperationModeBase::DeinitBehaviors()
{
	for (auto i = behaviors.end(); i != behaviors.begin();)
		(*(--i))->behavior->Deinit();
}

bool OperationModeBase::CanTakeMouseControl(OperationModeBehavior* behavior)
{
	return (draggingBehavior == nullptr || draggingBehavior == behavior)
		&& (singleFrameExclusiveMouseControlBehavior == nullptr || singleFrameExclusiveMouseControlBehavior == behavior);
}

void OperationModeBase::ToggleDragging(OperationModeBehavior* behavior, bool canStart)
{
	if (!CanTakeMouseControl(behavior))
		return;

	if (!ImGui::GetIO().WantCaptureMouse && !ImGui::IsAnyItemHovered())
		if (ImGui::IsMouseDragging(ImGuiMouseButton_Left) && canStart)
			draggingBehavior = behavior;

	if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
		draggingBehavior = nullptr;
}

bool OperationModeBase::IsDragging(OperationModeBehavior* behavior)
{
	return draggingBehavior == behavior;
}

void OperationModeBase::BeginSingleFrameExclusiveMouseControl(OperationModeBehavior* behavior)
{
	singleFrameExclusiveMouseControlBehavior = behavior;
}
