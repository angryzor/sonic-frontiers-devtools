#include "StandaloneOperationModeHost.h"

StandaloneOperationModeHost::StandaloneOperationModeHost(csl::fnd::IAllocator* allocator, OperationModeBase& operationMode)
	: StandaloneWindow{ allocator }, operationMode{ operationMode } {}

void StandaloneOperationModeHost::RenderContents() {
	operationMode.Render();
}

void StandaloneOperationModeHost::RenderPanel(PanelBase& panel)
{

}

bool StandaloneOperationModeHost::BeginSceneWindow(PanelBase& panel)
{
	return false;
}

void StandaloneOperationModeHost::EndSceneWindow()
{
}

bool StandaloneOperationModeHost::IsMouseOverSceneWindow()
{
	return false;
}
