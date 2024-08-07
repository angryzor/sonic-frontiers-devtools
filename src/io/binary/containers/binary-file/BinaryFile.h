#pragma once

#include <reflection/Concepts.h>

namespace devtools::io::binary::containers {
	class BinaryFile {
	public:
		static void Serialize(const wchar_t* filename, void* obj, const hh::fnd::RflClass* rflClass);
		template<rfl::RflClass T> static void Serialize(const wchar_t* filename, T* obj) {
			Serialize(filename, obj, &RESOLVE_STATIC_VARIABLE(T::rflClass));
		}
	};
}
