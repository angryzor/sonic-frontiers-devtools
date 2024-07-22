#pragma once
#include <optional>

namespace devtools::io::binary {
	namespace serialized_types {
		template<typename U, typename T> struct offset_t : public std::optional<U> {
		public:
			using std::optional<U>::optional;
		};

		template<typename T> using o64_t = offset_t<size_t, T>;
		template<typename T> using o32_t = offset_t<unsigned int, T>;
		
		template<typename T>
		struct MoveArray {
			o64_t<T> offset;
			size_t size;
			size_t capacity;
			size_t allocator{};
		};

		template<typename T>
		struct MoveArray32 {
			o64_t<T> offset;
			uint32_t size;
			uint32_t capacity;
			size_t allocator{};
		};

		template<typename T>
		struct TArray {
			o64_t<T> offset;
			size_t size;
			int64_t capacity;
		};
	};
}
