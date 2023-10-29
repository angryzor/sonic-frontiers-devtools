#include "../../Pch.h"
#include "ObjectInspection.h"

using namespace hh::fnd;
using namespace hh::game;

ObjectInspection::ObjectInspection(csl::fnd::IAllocator* allocator) : OperationMode{ allocator }
{
}

void ObjectInspection::Render() {
	auto* gameManager = GameManager::GetInstance();
	if (!gameManager)
		return;

	objectList.Render();
	objectInspector.Render();
}
