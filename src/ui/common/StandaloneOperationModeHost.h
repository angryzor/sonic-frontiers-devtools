#pragma once
#include <ui/common/StandaloneWindow.h>
#include <ui/operation-modes/OperationMode.h>

template<typename T>
class StandaloneOperationModeHost : public StandaloneWindow, public OperationModeHost {
public:
	T operationMode;

	StandaloneOperationModeHost(csl::fnd::IAllocator* allocator)
		: StandaloneWindow(allocator), operationMode{ GetAllocator(), *this }
	{
		SetTitle("Operation mode host");
	}

	virtual void RenderContents() override {
		auto pos = ImGui::GetCursorPos();

		ImGui::BeginChild("Scene", ImVec2(0, 0), ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoMove);
		ImGui::EndChild();

		ImGui::SetCursorPos(pos);

		operationMode.Render();
	}

	virtual void RenderPanel(PanelBase& panel) override {
		const ImGuiWindowFlags windowFlags = 0;

		auto traits = panel.GetPanelTraits();

		ImGui::SetNextWindowPos(traits.position, ImGuiCond_FirstUseEver, traits.pivot);
		ImGui::SetNextWindowSize(traits.size, ImGuiCond_FirstUseEver);

		if (ImGui::Begin(traits.title, NULL, windowFlags))
			panel.RenderPanel();

		ImGui::End();
	}
	virtual bool BeginSceneWindow() override {
		return ImGui::BeginChild("Scene");
	}
	virtual void EndSceneWindow() override {
		return ImGui::EndChild();
	}
	virtual bool BeginOverlayWindow() override {
		return true;
	}
	virtual void EndOverlayWindow() override {
	}
	virtual bool BeginMenuWindow() override {
		return true;
	}
	virtual void EndMenuWindow() override {
	}
	virtual bool IsMouseOverSceneWindow() override {
		bool isHovered{};
		if (ImGui::Begin(title)) {
			isHovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_RootWindow | ImGuiHoveredFlags_ChildWindows);
		}
		ImGui::End();
		return isHovered;
	}
	virtual void Dispatch(const ActionBase& action) override { operationMode.ProcessAction(action); }
};