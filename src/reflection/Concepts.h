#pragma once

namespace devtools::rfl {
	template<typename T>
	concept RflClass = requires {
		{ T::rflClass };
		{ T::typeInfo };
	};
}
