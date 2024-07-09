#pragma once

#include <ui/operation-modes/OperationMode.h>
#include <ui/operation-modes/behaviors/SelectionAabb.h>
#include <ui/operation-modes/behaviors/SelectionTransformation.h>
#include <ui/operation-modes/behaviors/MousePicking.h>
#include <ui/operation-modes/behaviors/Delete.h>
#include <utilities/math/Ray.h>
#include <utilities/math/Frustum.h>
#include "SurfRideElement.h"
#include "Context.h"

namespace ui::operation_modes::modes::surfride_editor
{
	class SurfRideEditor : public OperationMode<Context>
	{
	public:
		SurfRideEditor(csl::fnd::IAllocator* allocator);
	};
}
