#pragma once

namespace hh::fnd {
	template<typename T> struct RflClassTraitsImpl {
		static const RflClass rflClass;
		static const RflTypeInfo typeInfo;
	};

	template<typename T> struct RflClassTraits {};

	template<typename T> struct RflPrimitiveTypeTraits {};
	template<> struct RflPrimitiveTypeTraits<void> { static constexpr RflClassMember::Type type = RflClassMember::Type::TYPE_VOID; };
	template<> struct RflPrimitiveTypeTraits<bool> { static constexpr RflClassMember::Type type = RflClassMember::Type::TYPE_BOOL; };
	template<> struct RflPrimitiveTypeTraits<int8_t> { static constexpr RflClassMember::Type type = RflClassMember::Type::TYPE_SINT8; };
	template<> struct RflPrimitiveTypeTraits<uint8_t> { static constexpr RflClassMember::Type type = RflClassMember::Type::TYPE_UINT8; };
	template<> struct RflPrimitiveTypeTraits<int16_t> { static constexpr RflClassMember::Type type = RflClassMember::Type::TYPE_SINT16; };
	template<> struct RflPrimitiveTypeTraits<uint16_t> { static constexpr RflClassMember::Type type = RflClassMember::Type::TYPE_UINT16; };
	template<> struct RflPrimitiveTypeTraits<int32_t> { static constexpr RflClassMember::Type type = RflClassMember::Type::TYPE_SINT32; };
	template<> struct RflPrimitiveTypeTraits<uint32_t> { static constexpr RflClassMember::Type type = RflClassMember::Type::TYPE_UINT32; };
	template<> struct RflPrimitiveTypeTraits<int64_t> { static constexpr RflClassMember::Type type = RflClassMember::Type::TYPE_SINT64; };
	template<> struct RflPrimitiveTypeTraits<uint64_t> { static constexpr RflClassMember::Type type = RflClassMember::Type::TYPE_UINT64; };
	template<> struct RflPrimitiveTypeTraits<float> { static constexpr RflClassMember::Type type = RflClassMember::Type::TYPE_FLOAT; };
	template<> struct RflPrimitiveTypeTraits<csl::math::Vector2> { static constexpr RflClassMember::Type type = RflClassMember::Type::TYPE_VECTOR2; };
	template<> struct RflPrimitiveTypeTraits<csl::math::Vector3> { static constexpr RflClassMember::Type type = RflClassMember::Type::TYPE_VECTOR3; };
	template<> struct RflPrimitiveTypeTraits<csl::math::Vector4> { static constexpr RflClassMember::Type type = RflClassMember::Type::TYPE_VECTOR4; };
	template<> struct RflPrimitiveTypeTraits<csl::math::Quaternion> { static constexpr RflClassMember::Type type = RflClassMember::Type::TYPE_QUATERNION; };
	template<> struct RflPrimitiveTypeTraits<csl::math::Matrix34> { static constexpr RflClassMember::Type type = RflClassMember::Type::TYPE_MATRIX34; };
	template<> struct RflPrimitiveTypeTraits<csl::math::Matrix44> { static constexpr RflClassMember::Type type = RflClassMember::Type::TYPE_MATRIX44; };
	template<> struct RflPrimitiveTypeTraits<csl::ut::VariableString> { static constexpr RflClassMember::Type type = RflClassMember::Type::TYPE_STRING; };
	template<> struct RflPrimitiveTypeTraits<hh::game::ObjectId> { static constexpr RflClassMember::Type type = RflClassMember::Type::TYPE_OBJECT_ID; };
	template<> struct RflPrimitiveTypeTraits<csl::math::Position> { static constexpr RflClassMember::Type type = RflClassMember::Type::TYPE_POSITION; };
	template<> struct RflPrimitiveTypeTraits<csl::ut::Color<uint8_t>> { static constexpr RflClassMember::Type type = RflClassMember::Type::TYPE_COLOR_BYTE; };
	template<> struct RflPrimitiveTypeTraits<csl::ut::Color<float>> { static constexpr RflClassMember::Type type = RflClassMember::Type::TYPE_COLOR_FLOAT; };

	using DynamicRflClassResolver = const hh::fnd::RflClass* (*)(const void*);
	using DynamicRflArraySizeResolver = size_t (*)(const void*);

	template<DynamicRflClassResolver classResolver> struct RflDynamicStructPointer {};
	template<DynamicRflClassResolver classResolver> struct RflDynamicStructSelfPointer {};
	template<DynamicRflClassResolver classResolver, DynamicRflArraySizeResolver arraySizeResolver> struct RflDynamicStructPointerArray {};
	template<DynamicRflClassResolver classResolver, DynamicRflArraySizeResolver arraySizeResolver> struct RflDynamicStructSelfPointerArray {};
	template<typename T, DynamicRflArraySizeResolver arraySizeResolver> struct RflDynamicPointerArray {};
	template<typename T, DynamicRflArraySizeResolver arraySizeResolver> struct RflDynamicInlineArray {};
}

template<> struct hh::fnd::RflClassTraits<SurfRide::Color> : hh::fnd::RflClassTraitsImpl<SurfRide::Color> {};
template<> struct hh::fnd::RflClassTraits<SurfRide::Colorf> : hh::fnd::RflClassTraitsImpl<SurfRide::Colorf> {};
