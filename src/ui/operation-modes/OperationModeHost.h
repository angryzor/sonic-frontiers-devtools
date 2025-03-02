#pragma once

class OperationModeHost {
public:
	virtual void RenderPanel(PanelBase& panel) = 0;
	virtual bool BeginSceneWindow() = 0;
	virtual void EndSceneWindow() = 0;
	virtual bool BeginOverlayWindow() = 0;
	virtual void EndOverlayWindow() = 0;
	virtual bool BeginMenuWindow() = 0;
	virtual void EndMenuWindow() = 0;
	virtual bool IsMouseOverSceneWindow() = 0;
	virtual void Dispatch(const ActionBase& action) = 0;
};
