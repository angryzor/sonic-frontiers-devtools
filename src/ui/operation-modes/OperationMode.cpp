#include "OperationMode.h"

void OperationModeBase::Render() {
	if (host.BeginSceneWindow()) {
		singleFrameExclusiveMouseControlBehavior = nullptr;

		RenderScene();

		for (auto& behavior : behaviors)
			behavior->Render();
	}
	host.EndSceneWindow();

	while (actionsInFlight.size() > 0) {
		host.Dispatch(actionsInFlight[0]->GetAction());
		actionsInFlight.remove(0);
	}
}

void OperationModeBase::RenderScene()
{
}

void OperationModeBase::Dispatch(const ActionBase& action) {
	actionsInFlight.push_back(action.CreateAsync(GetAllocator()));
}

//void OperationModeBase::Dispatch(ActionBase&& action) {
//	actionsInFlight.push_back(std::move(action).CreateAsync(GetAllocator()));
//}

void OperationModeBase::ProcessAction(const ActionBase& action) {
	for (auto& behavior : behaviors)
		behavior->ProcessAction(action);
}

void OperationModeBase::InitBehaviors() {
	for (auto& behavior : behaviors)
		behavior->Init();
}

void OperationModeBase::DeinitBehaviors() {
	for (auto i = behaviors.end(); i != behaviors.begin();)
		(*(--i))->Deinit();
}

bool OperationModeBase::BeginOverlayWindow() {
	return host.BeginOverlayWindow();
}

void OperationModeBase::EndOverlayWindow() {
	return host.EndOverlayWindow();
}

bool OperationModeBase::IsMouseOverSceneWindow() {
	return host.IsMouseOverSceneWindow();
}

bool OperationModeBase::CanTakeMouseControl(OperationModeBehavior* behavior) {
	return (draggingBehavior == nullptr || draggingBehavior == behavior)
		&& (singleFrameExclusiveMouseControlBehavior == nullptr || singleFrameExclusiveMouseControlBehavior == behavior);
}

void OperationModeBase::ToggleDragging(OperationModeBehavior* behavior, bool canStart) {
	if (!CanTakeMouseControl(behavior))
		return;

	if (IsMouseOverSceneWindow())
		if (ImGui::IsMouseDragging(ImGuiMouseButton_Left) && canStart)
			draggingBehavior = behavior;

	if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
		draggingBehavior = nullptr;
}

bool OperationModeBase::IsDragging(OperationModeBehavior* behavior) {
	return draggingBehavior == behavior;
}

void OperationModeBase::BeginSingleFrameExclusiveMouseControl(OperationModeBehavior* behavior) {
	singleFrameExclusiveMouseControlBehavior = behavior;
}
