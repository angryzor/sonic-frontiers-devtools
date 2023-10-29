#include "Pch.h"
#include "Desktop.h"
#include "ToolBar.h"

Desktop::Desktop(csl::fnd::IAllocator* allocator) : BaseObject{ allocator }
{
}

void Desktop::Render() {
	ToolBar::Render();
	operationMode.Render();
}
