#pragma once

namespace ui::operation_modes::modes::bases::list_and_inspector {
	class ListAndInspector : public OperationMode<Context>
	{
	public:
		ListAndInspector(csl::fnd::IAllocator* allocator);
	};
}
