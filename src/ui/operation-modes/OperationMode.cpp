#include "OperationMode.h"

OperationMode::OperationMode(csl::fnd::IAllocator* allocator) : CompatibleObject{ allocator }
{
}

void OperationMode::RenderOperationMode()
{
	for (auto& behavior : behaviors)
		behavior->Render();

	Render();

	singleFrameExclusiveMouseControlBehavior = nullptr;
}

void OperationMode::Render()
{
}

void OperationMode::InitBehaviors()
{
	for (auto& behavior : behaviors)
		behavior->Init();
}

void OperationMode::DeinitBehaviors()
{
	for (auto i = behaviors.end(); i != behaviors.begin();)
		(*(--i))->Deinit();
}

void OperationMode::ProcessAction(const ActionBase& action)
{
	for (auto& behavior : behaviors)
		behavior->ProcessAction(action);
}

bool OperationMode::CanTakeMouseControl(OperationModeBehavior* behavior)
{
	return (draggingBehavior == nullptr || draggingBehavior == behavior)
		&& (singleFrameExclusiveMouseControlBehavior == nullptr || singleFrameExclusiveMouseControlBehavior == behavior);
}

void OperationMode::ToggleDragging(OperationModeBehavior* behavior, bool canStart)
{
	if (!CanTakeMouseControl(behavior))
		return;

	if (!ImGui::GetIO().WantCaptureMouse && !ImGui::IsAnyItemHovered())
		if (ImGui::IsMouseDragging(ImGuiMouseButton_Left) && canStart)
			draggingBehavior = behavior;

	if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
		draggingBehavior = nullptr;
}

bool OperationMode::IsDragging(OperationModeBehavior* behavior)
{
	return draggingBehavior == behavior;
}

void OperationMode::BeginSingleFrameExclusiveMouseControl(OperationModeBehavior* behavior)
{
	singleFrameExclusiveMouseControlBehavior = behavior;
}
