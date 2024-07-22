#pragma once
#include <hedgelib/hl_internal.h>

namespace hl {
	namespace csl {
		struct variable_string {
			hl::off64<const char> str;
			hl::off64<void> allocator;

			template<bool swapOffsets = true>
			void endian_swap() noexcept
			{
				hl::endian_swap<swapOffsets>(str);
			}
		};
	}

	namespace hh {
		template<typename T>
		struct tarray
		{
			using value_type = T;
			using size_type = size_t;
			using pointer = T*;
			using const_pointer = const T*;
			using reference = typename off64<T>::reference;
			using const_reference = typename off64<T>::const_reference;
			using iterator = pointer;
			using const_iterator = const_pointer;
			using reverse_iterator = std::reverse_iterator<iterator>;
			using const_reverse_iterator = std::reverse_iterator<const_iterator>;

			off64<T> dataPtr;
			size_t count;
			int64_t capacity;

			template<bool swapOffsets = true>
			void endian_swap() noexcept
			{
				hl::endian_swap<swapOffsets>(dataPtr);
				hl::endian_swap<swapOffsets>(count);
				hl::endian_swap<swapOffsets>(capacity);
			}
		};
	}
}
