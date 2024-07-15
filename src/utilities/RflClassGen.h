#pragma once

namespace hh::fnd {
	template<typename T, typename U> using Always = T;

	template<typename T, typename = void> struct RflTypeTraits {
		inline static const RflClass* GetRflClass() { return nullptr; }
		inline static const RflClassEnum* GetRflClassEnum() { return nullptr; }
		static constexpr RflClassMember::Type type = RflPrimitiveTypeTraits<T>::type;
		static constexpr RflClassMember::Type subtype = RflClassMember::Type::TYPE_VOID;
		static constexpr size_t arrayLen = 0;
		static constexpr unsigned int flags = 0;
	};
	template<typename T> struct RflTypeTraits<T, std::enable_if_t<std::is_enum_v<T>>> {
		inline static const RflClass* GetRflClass() { return nullptr; }
		inline static const RflClassEnum* GetRflClassEnum(){ return nullptr; }
		static constexpr RflClassMember::Type type = RflClassMember::Type::TYPE_ENUM;
		static constexpr RflClassMember::Type subtype = RflPrimitiveTypeTraits<std::underlying_type_t<T>>::type;
		static constexpr size_t arrayLen = 0;
		static constexpr unsigned int flags = 0;
	};
	template<typename T> struct RflTypeTraits<T, std::enable_if_t<std::is_same_v<decltype(RflClassTraits<T>::rflClass), const RflClass>>> {
		inline static const RflClass* GetRflClass() { return &RflClassTraits<T>::rflClass; }
		inline static const RflClassEnum* GetRflClassEnum(){ return nullptr; }
		static constexpr RflClassMember::Type type = RflClassMember::Type::TYPE_STRUCT;
		static constexpr RflClassMember::Type subtype = RflClassMember::Type::TYPE_VOID;
		static constexpr size_t arrayLen = 0;
		static constexpr unsigned int flags = 0;
	};
	template<typename T> struct RflTypeTraits<T*> {
		inline static const RflClass* GetRflClass() { return RflTypeTraits<T>::GetRflClass(); }
		inline static const RflClassEnum* GetRflClassEnum(){ return RflTypeTraits<T>::GetRflClassEnum(); }
		static constexpr RflClassMember::Type type = RflClassMember::Type::TYPE_POINTER;
		static constexpr RflClassMember::Type subtype = RflTypeTraits<T>::type;
		static constexpr size_t arrayLen = 0;
		static constexpr unsigned int flags = RflTypeTraits<T>::flags;
	};
	template<> struct RflTypeTraits<const char*> {
		inline static const RflClass* GetRflClass() { return nullptr; }
		inline static const RflClassEnum* GetRflClassEnum(){ return nullptr; }
		static constexpr RflClassMember::Type type = RflClassMember::Type::TYPE_CSTRING;
		static constexpr RflClassMember::Type subtype = RflClassMember::Type::TYPE_VOID;
		static constexpr size_t arrayLen = 0;
		static constexpr unsigned int flags = 0;
	};
	template<typename T> struct RflTypeTraits<csl::ut::MoveArray<T>> {
		inline static const RflClass* GetRflClass() { return RflTypeTraits<T>::GetRflClass(); }
		inline static const RflClassEnum* GetRflClassEnum(){ return RflTypeTraits<T>::GetRflClassEnum(); }
		static constexpr RflClassMember::Type type = RflClassMember::Type::TYPE_ARRAY;
		static constexpr RflClassMember::Type subtype = RflTypeTraits<T>::type;
		static constexpr size_t arrayLen = 0;
		static constexpr unsigned int flags = 0;
	};
#ifdef DEVTOOLS_TARGET_SDK_wars
	template<typename T> struct RflTypeTraits<hh::TArray<T>> {
		static constexpr RflClassMember::Type type = RflClassMember::Type::TYPE_OLD_ARRAY;
		static constexpr RflClassMember::Type subtype = RflTypeTraits<T>::type;
		static constexpr size_t arrayLen = 0;
		static constexpr unsigned int flags = 0;
	};
#endif
	template<typename T> struct RflTypeTraits<csl::ut::Bitset<T>> {
		inline static const RflClass* GetRflClass() { return RflTypeTraits<T>::GetRflClass(); }
		inline static const RflClassEnum* GetRflClassEnum(){ return RflTypeTraits<T>::GetRflClassEnum(); }
		static constexpr RflClassMember::Type type = RflClassMember::Type::TYPE_FLAGS;
		static constexpr RflClassMember::Type subtype = RflTypeTraits<T>::type;
		static constexpr size_t arrayLen = 0;
		static constexpr unsigned int flags = 0;
	};
	template<typename T, size_t Len> struct RflTypeTraits<T(&)[Len]> {
		inline static const RflClass* GetRflClass() { return RflTypeTraits<T>::GetRflClass(); }
		inline static const RflClassEnum* GetRflClassEnum(){ return RflTypeTraits<T>::GetRflClassEnum(); }
		static constexpr RflClassMember::Type type = RflTypeTraits<T>::type;
		static constexpr RflClassMember::Type subtype = RflTypeTraits<T>::subtype;
		static constexpr size_t arrayLen = Len;
		static constexpr unsigned int flags = 0;
	};
	template<typename Class, typename Parent> struct RflTypeTraits<RflDynamicStructPointer<Class, Parent>> {
		inline static const RflClass* GetRflClass() { return reinterpret_cast<const RflClass*>(&RflDynamicStructPointer<Class, Parent>::Resolve); }
		inline static const RflClassEnum* GetRflClassEnum(){ return nullptr; }
		static constexpr RflClassMember::Type type = RflClassMember::Type::TYPE_POINTER;
		static constexpr RflClassMember::Type subtype = RflClassMember::Type::TYPE_STRUCT;
		static constexpr size_t arrayLen = 0;
		static constexpr unsigned int flags = 0x10000;
	};
	template<typename T, typename Parent> struct RflTypeTraits<RflDynamicPointerArray<T, Parent>> {
		inline static const RflClass* GetRflClass() { return RflTypeTraits<T>::GetRflClass(); }
		inline static const RflClassEnum* GetRflClassEnum() { return reinterpret_cast<const RflClassEnum*>(&RflDynamicPointerArray<T, Parent>::Resolve); }
		static constexpr RflClassMember::Type type = RflClassMember::Type::TYPE_POINTER;
		static constexpr RflClassMember::Type subtype = RflTypeTraits<T>::type;
		static constexpr size_t arrayLen = 0;
		static constexpr unsigned int flags = RflTypeTraits<T>::flags | 0x20000;
	};
	template<typename T, typename Parent> struct RflTypeTraits<RflDynamicInlineArray<T, Parent>> {
		inline static const RflClass* GetRflClass() { return RflTypeTraits<T>::GetRflClass(); }
		inline static const RflClassEnum* GetRflClassEnum() { return reinterpret_cast<const RflClassEnum*>(&RflDynamicInlineArray<T, Parent>::Resolve); }
		static constexpr RflClassMember::Type type = RflTypeTraits<T>::type;
		static constexpr RflClassMember::Type subtype = RflTypeTraits<T>::subtype;
		static constexpr size_t arrayLen = 0;
		static constexpr unsigned int flags = RflTypeTraits<T>::flags | 0x40000;
	};

	template<typename T> constexpr const RflClassMember CreateRflClassMember(const char* name, unsigned int offset) {
		return {
			name,
			RflTypeTraits<T>::GetRflClass(),
			RflTypeTraits<T>::GetRflClassEnum(),
			RflTypeTraits<T>::type,
			RflTypeTraits<T>::subtype,
			RflTypeTraits<T>::arrayLen,
			RflTypeTraits<T>::flags,
			offset
		};
	}

	template<typename T> struct RflClassMemberSize { static constexpr size_t size = sizeof(T); };
	template<typename Class, typename Parent> struct RflClassMemberSize<RflDynamicStructPointer<Class, Parent>> { static constexpr size_t size = sizeof(void*); };
	template<typename T, typename Parent> struct RflClassMemberSize<RflDynamicPointerArray<T, Parent>> { static constexpr size_t size = sizeof(void*); };
	template<typename T, typename Parent> struct RflClassMemberSize<RflDynamicInlineArray<T, Parent>> { static constexpr size_t size = 0; };

	template<typename T> struct RflClassMemberAlignment { static constexpr size_t align = alignof(T); };
	template<typename Class, typename Parent> struct RflClassMemberAlignment<RflDynamicStructPointer<Class, Parent>> { static constexpr size_t align = alignof(void*); };
	template<typename T, typename Parent> struct RflClassMemberAlignment<RflDynamicPointerArray<T, Parent>> { static constexpr size_t align = alignof(void*); };
	template<typename T, typename Parent> struct RflClassMemberAlignment<RflDynamicInlineArray<T, Parent>> { static constexpr size_t align = alignof(void*); };

	template<typename ...T> constexpr std::array<const RflClassMember, std::tuple_size_v<std::tuple<T...>>> CreateRflClassMembers(Always<const char*, T>... name) {
		unsigned int offset{ 0 };
		unsigned int o{};

		return { CreateRflClassMember<T>(name, (o = (offset + RflClassMemberAlignment<T>::align - 1) & ~(RflClassMemberAlignment<T>::align - 1), offset = o + RflClassMemberSize<T>::size, o))... };
	}

	template<typename T> constexpr const RflClass CreateRflClass(const char* name, const RflClassMember* members) {
		return {
			name,
			sizeof(T),
			nullptr,
			0,
			members,
			0,
			nullptr
		};
	}
}

auto srsColorMembers = hh::fnd::CreateRflClassMembers<int, int, int, int>("r", "g", "b", "a");
auto srsColorfMembers = hh::fnd::CreateRflClassMembers<float, float, float, float>("a", "r", "g", "b");

const hh::fnd::RflClass hh::fnd::RflClassTraits<SurfRide::Color>::rflClass{ "SurfRideColor", nullptr, sizeof(SurfRide::Color), nullptr, 0, srsColorMembers.data(), srsColorMembers.size(), nullptr};
const hh::fnd::RflClass hh::fnd::RflClassTraits<SurfRide::Colorf>::rflClass{ "SurfRideColorf", nullptr, sizeof(SurfRide::Colorf), nullptr, 0, srsColorfMembers.data(), srsColorfMembers.size(), nullptr};
