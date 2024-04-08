namespace hl {
	template<bool swapOffsets = true>
	inline void endian_swap(::csl::math::Vector2& v) noexcept
	{
		endian_swap(v(0));
		endian_swap(v(1));
	}

	template<bool swapOffsets = true>
	inline void endian_swap(::csl::math::Vector3& v) noexcept
	{
		endian_swap(v(0));
		endian_swap(v(1));
		endian_swap(v(2));
	}

	template<bool swapOffsets = true>
	inline void endian_swap(::csl::math::Vector4& v) noexcept
	{
		endian_swap(v(0));
		endian_swap(v(1));
		endian_swap(v(2));
		endian_swap(v(3));
	}

	template<bool swapOffsets = true>
	inline void endian_swap(::csl::math::Matrix34& v) noexcept
	{
		endian_swap(v(0, 0));
		endian_swap(v(0, 1));
		endian_swap(v(0, 2));
		endian_swap(v(0, 3));
		endian_swap(v(1, 0));
		endian_swap(v(1, 1));
		endian_swap(v(1, 2));
		endian_swap(v(1, 3));
		endian_swap(v(2, 0));
		endian_swap(v(2, 1));
		endian_swap(v(2, 2));
		endian_swap(v(2, 3));
		endian_swap(v(3, 0));
		endian_swap(v(3, 1));
		endian_swap(v(3, 2));
		endian_swap(v(3, 3));
	}

	template<bool swapOffsets = true>
	inline void endian_swap(::csl::math::Matrix44& v) noexcept
	{
		endian_swap(v(0, 0));
		endian_swap(v(0, 1));
		endian_swap(v(0, 2));
		endian_swap(v(0, 3));
		endian_swap(v(1, 0));
		endian_swap(v(1, 1));
		endian_swap(v(1, 2));
		endian_swap(v(1, 3));
		endian_swap(v(2, 0));
		endian_swap(v(2, 1));
		endian_swap(v(2, 2));
		endian_swap(v(2, 3));
		endian_swap(v(3, 0));
		endian_swap(v(3, 1));
		endian_swap(v(3, 2));
		endian_swap(v(3, 3));
	}

	template<bool swapOffsets = true>
	inline void endian_swap(::csl::math::Position& v) noexcept
	{
		endian_swap(v(0));
		endian_swap(v(1));
		endian_swap(v(2));
	}

	template<bool swapOffsets = true>
	inline void endian_swap(::csl::ut::Color<uint8_t>& v) noexcept
	{
		endian_swap(v.r);
		endian_swap(v.g);
		endian_swap(v.b);
		endian_swap(v.a);
	}

	template<bool swapOffsets = true>
	inline void endian_swap(::csl::ut::Color<float>& v) noexcept
	{
		endian_swap(v.r);
		endian_swap(v.g);
		endian_swap(v.b);
		endian_swap(v.a);
	}

	template<bool swapOffsets = true>
	inline void endian_swap(::hh::game::ObjectId& v) noexcept
	{
	}
}
