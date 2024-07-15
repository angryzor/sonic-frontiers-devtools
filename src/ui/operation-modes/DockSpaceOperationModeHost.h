#pragma once
#include "OperationMode.h"

class DockSpaceOperationModeHost : public OperationModeHost {
	ImGuiID dockSpaceId;
public:
	DockSpaceOperationModeHost(ImGuiID dockspaceId) : dockSpaceId{ dockspaceId } {}

	void RenderPanel(PanelBase& panel) override;
	bool BeginSceneWindow(PanelBase& panel) override;
	void EndSceneWindow() override;
	bool IsMouseOverSceneWindow() override;
};
