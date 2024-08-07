#include "OperationMode.h"

void OperationModeBase::Render()
{
	singleFrameExclusiveMouseControlBehavior = nullptr;

	for (auto& behavior : behaviors)
		behavior->Render();
}

void OperationModeBase::ProcessAction(const ActionBase& action)
{
	for (auto& behavior : behaviors)
		behavior->ProcessAction(action);
}

void OperationModeBase::InitBehaviors()
{
	for (auto& behavior : behaviors)
		behavior->Init();
}

void OperationModeBase::DeinitBehaviors()
{
	for (auto i = behaviors.end(); i != behaviors.begin();)
		(*(--i))->Deinit();
}

bool OperationModeBase::IsMouseOverSceneWindow()
{
	return host.IsMouseOverSceneWindow();
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

	if (IsMouseOverSceneWindow())
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
