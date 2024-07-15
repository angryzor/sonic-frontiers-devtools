#pragma once
#include <ui/common/StandaloneWindow.h>
#include <ui/operation-modes/OperationMode.h>

class StandaloneOperationModeHost : public StandaloneWindow, public OperationModeHost {
	OperationModeBase& operationMode;
public:
	StandaloneOperationModeHost(csl::fnd::IAllocator* allocator, OperationModeBase& operationMode);

	virtual void RenderContents() override;

	// Inherited via OperationModeHost
	void RenderPanel(PanelBase& panel) override;
	bool BeginSceneWindow(PanelBase& panel) override;
	void EndSceneWindow() override;
	bool IsMouseOverSceneWindow() override;
};