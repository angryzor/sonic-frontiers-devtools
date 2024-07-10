#pragma once

namespace ui::operation_modes::behaviors {
	template<typename T, typename C>
	class Placement : public Behavior<T> {
	public:
		using Behavior::Behavior;

		const C* objectClassToPlace{};


	};
}