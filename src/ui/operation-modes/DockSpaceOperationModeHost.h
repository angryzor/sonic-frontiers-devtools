#pragma once
#include "OperationMode.h"

template<typename T>
class DockSpaceOperationModeHost : public OperationModeHost {
	ImGuiID dockSpaceId;
	T operationMode;
public:
	DockSpaceOperationModeHost(ImGuiID dockspaceId) : dockSpaceId{ dockspaceId } {}

	virtual void Render();
	virtual void RenderPanel(PanelBase& panel) override {
		const ImGuiWindowFlags windowFlags = 0;

		auto traits = panel.GetPanelTraits();

		ImGui::SetNextWindowPos(traits.position, ImGuiCond_FirstUseEver, traits.pivot);
		ImGui::SetNextWindowSize(traits.size, ImGuiCond_FirstUseEver);

		if (ImGui::Begin(traits.title, NULL, windowFlags))
			panel.RenderPanel();

		ImGui::End();
	}
	virtual bool BeginSceneWindow(PanelBase& panel) override { return false; }
	virtual void EndSceneWindow() override {}
	virtual bool IsMouseOverSceneWindow() override { return false; }
};
