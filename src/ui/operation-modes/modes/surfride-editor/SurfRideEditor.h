#pragma once
#include <ui/operation-modes/OperationMode.h>
#include "Context.h"

namespace ui::operation_modes::modes::surfride_editor
{
	class SurfRideEditor : public OperationMode<Context>
	{
	public:
		SurfRideEditor(csl::fnd::IAllocator* allocator);
	};
}
