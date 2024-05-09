#include <ui/common/SimpleWidgets.h>
#include "CameraManagerInspector.h"

using namespace hh::game;

CameraManagerInspector::CameraManagerInspector(csl::fnd::IAllocator* allocator) : StandaloneWindow{ allocator }
{
	SetTitle("CameraManager");
}

void CameraManagerInspector::RenderContents()
{
	auto* gameManager = GameManager::GetInstance();
	auto* cameraManager = gameManager->GetService<CameraManager>();


}
