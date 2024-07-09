#pragma once

namespace ui::operation_modes {
	template<typename Context, typename Behavior, typename Operations = typename Behavior::Operations>
	class BehaviorContext : public Operations {
	protected:
		Context& context;

	public:
		BehaviorContext(Context& context) : context(context) {}
	};
}
