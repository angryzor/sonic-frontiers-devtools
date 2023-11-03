#include "../../Pch.h"
#include "ObjectInspection.h"
#include "../../GameServiceInspector.h"

using namespace hh::fnd;
using namespace hh::game;

ObjectInspection::ObjectInspection(csl::fnd::IAllocator* allocator, Desktop& desktop) : OperationMode{ allocator }, desktop{ desktop }
{
}

void ObjectInspection::Render() {
	auto* gameManager = GameManager::GetInstance();
	if (!gameManager)
		return;

	objectList.Render();
	objectInspector.Render();
	GameServiceInspector::Render();
}
