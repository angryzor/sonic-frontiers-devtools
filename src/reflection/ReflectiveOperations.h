#pragma once
#include "RflArrayAccessor.h"
#include <tuple>
#include <reflection/RflClassGenFwd.h>

namespace rflops {
	namespace traversals {
		template<typename V, typename = std::make_index_sequence<V::arity>>
		class rflop;
		template<typename V, std::size_t... S>
		class rflop<V, std::index_sequence<S...>> {
			template<std::size_t, typename T = void*> using RflObj = T;

			V v;

			template<typename U>
			typename V::result_type ProcessPrimitive(RflObj<S>... objs) {
				return v.VisitPrimitive(static_cast<U*>(objs)...);
			}

			typename V::result_type ProcessPrimitive(RflObj<S>... objs, const hh::fnd::RflClassMember::Type type) {
				switch (type) {
				case hh::fnd::RflClassMember::TYPE_BOOL: return ProcessPrimitive<bool>(objs...);
				case hh::fnd::RflClassMember::TYPE_SINT8: return ProcessPrimitive<int8_t>(objs...);
				case hh::fnd::RflClassMember::TYPE_UINT8: return ProcessPrimitive<uint8_t>(objs...);
				case hh::fnd::RflClassMember::TYPE_SINT16: return ProcessPrimitive<int16_t>(objs...);
				case hh::fnd::RflClassMember::TYPE_UINT16: return ProcessPrimitive<uint16_t>(objs...);
				case hh::fnd::RflClassMember::TYPE_SINT32: return ProcessPrimitive<int32_t>(objs...);
				case hh::fnd::RflClassMember::TYPE_UINT32: return ProcessPrimitive<uint32_t>(objs...);
				case hh::fnd::RflClassMember::TYPE_SINT64: return ProcessPrimitive<int64_t>(objs...);
				case hh::fnd::RflClassMember::TYPE_UINT64: return ProcessPrimitive<uint64_t>(objs...);
				case hh::fnd::RflClassMember::TYPE_FLOAT: return ProcessPrimitive<float>(objs...);
				case hh::fnd::RflClassMember::TYPE_VECTOR2: return ProcessPrimitive<csl::math::Vector2>(objs...);
				case hh::fnd::RflClassMember::TYPE_VECTOR3: return ProcessPrimitive<csl::math::Vector3>(objs...);
				case hh::fnd::RflClassMember::TYPE_VECTOR4: return ProcessPrimitive<csl::math::Vector4>(objs...);
				case hh::fnd::RflClassMember::TYPE_QUATERNION: return ProcessPrimitive<csl::math::Quaternion>(objs...);
				case hh::fnd::RflClassMember::TYPE_MATRIX34: return ProcessPrimitive<csl::math::Matrix34>(objs...);
				case hh::fnd::RflClassMember::TYPE_MATRIX44: return ProcessPrimitive<csl::math::Matrix44>(objs...);
				case hh::fnd::RflClassMember::TYPE_CSTRING: return ProcessPrimitive<const char*>(objs...);
				case hh::fnd::RflClassMember::TYPE_STRING: return ProcessPrimitive<csl::ut::VariableString>(objs...);
				case hh::fnd::RflClassMember::TYPE_OBJECT_ID: return ProcessPrimitive<hh::game::ObjectId>(objs...);
				case hh::fnd::RflClassMember::TYPE_COLOR_BYTE: return ProcessPrimitive<csl::ut::Color8>(objs...);
				case hh::fnd::RflClassMember::TYPE_COLOR_FLOAT: return ProcessPrimitive<csl::ut::Color<float>>(objs...);
				case hh::fnd::RflClassMember::TYPE_POSITION: return ProcessPrimitive<csl::math::Position>(objs...);
				default:
					assert(!"reflective operation assertion failed: unknown primitive type");
					return typename V::result_type{};
				}
			}

			typename V::result_type ProcessSingle(RflObj<S>... objs, const hh::fnd::RflClassMember* member, const hh::fnd::RflClassMember::Type type)
			{
				switch (type) {
				case hh::fnd::RflClassMember::TYPE_STRUCT:
					return ProcessStruct(objs..., member->m_pClass);
				default:
					return ProcessPrimitive(objs..., type);
				}
			}

			typename V::result_type ProcessArray(RflObj<S>... objs, const hh::fnd::RflClassMember* member)
			{
				std::tuple<RflObj<S, RflArrayAccessor<csl::ut::MoveArray>>...> arrs{ { *static_cast<csl::ut::MoveArray<void*>*>(objs), member }... };

				return v.VisitArray(
					std::get<S>(arrs)...,
					[=]() {
						void* obj = new (std::align_val_t(16), hh::fnd::MemoryRouter::GetModuleAllocator()) char[member->GetSubTypeSizeInBytes()];

						if (member->GetSubType() == hh::fnd::RflClassMember::TYPE_STRUCT)
							hh::fnd::RflTypeInfoRegistry::GetInstance()->ConstructObject(hh::fnd::MemoryRouter::GetModuleAllocator(), obj, member->GetStructClass()->GetName());

						return obj;
					},
					[=](void* obj) {
						if (member->GetSubType() == hh::fnd::RflClassMember::TYPE_STRUCT)
							hh::fnd::RflTypeInfoRegistry::GetInstance()->CleanupLoadedObject(obj, member->GetStructClass()->GetName());

						hh::fnd::MemoryRouter::GetModuleAllocator()->Free(obj);
					},
					[=](RflObj<S>... objs) { return ProcessSingle(objs..., member, member->GetSubType()); }
				);
			}

	#ifdef DEVTOOLS_TARGET_SDK_wars
			typename V::result_type ProcessOldArray(RflObj<S>... objs, const hh::fnd::RflClassMember* member)
			{
				std::tuple<RflObj<S, RflArrayAccessor<hh::TArray>>...> arrs{ { *static_cast<hh::TArray<void*>*>(objs), member }... };

				return v.VisitArray(
					std::get<S>(arrs)...,
					[=]() {
						void* obj = new (std::align_val_t(16), hh::fnd::MemoryRouter::GetModuleAllocator()) char[member->GetSubTypeSizeInBytes()];

						if (member->GetSubType() == hh::fnd::RflClassMember::TYPE_STRUCT)
							hh::fnd::RflTypeInfoRegistry::GetInstance()->ConstructObject(hh::fnd::MemoryRouter::GetModuleAllocator(), obj, member->GetStructClass()->GetName());

						return obj;
					},
					[=](void* obj) {
						if (member->GetSubType() == hh::fnd::RflClassMember::TYPE_STRUCT)
							hh::fnd::RflTypeInfoRegistry::GetInstance()->CleanupLoadedObject(obj, member->GetStructClass()->GetName());

						hh::fnd::MemoryRouter::GetModuleAllocator()->Free(obj);
					},
					[=](RflObj<S>... objs) { return ProcessSingle(objs..., member, member->GetSubType() == hh::fnd::RflClassMember::TYPE_UINT32 ? hh::fnd::RflClassMember::TYPE_OBJECT_ID : member->GetSubType()); } // We're overriding this here because Forces seems to use TYPE_OLD_ARRAY,TYPE_UINT32 for object ID arrays...
				);
			}
	#endif

			typename V::result_type ProcessEnum(RflObj<S>... objs, const hh::fnd::RflClassMember* member) {
				return v.VisitEnum(objs..., member->GetSubType(), member->GetEnumClass(), [=](RflObj<S>... objs) { return ProcessSingle(objs..., member, member->GetSubType()); });
			}

			typename V::result_type ProcessFlags(RflObj<S>... objs, const hh::fnd::RflClassMember* member) {
				if (auto* enumData = member->GetAttribute("DisplayIndex"))
				{
					auto* enumEntries = reinterpret_cast<const hh::fnd::RflArray<const hh::fnd::RflClassEnumMember>*>(enumData->GetData());
					return v.VisitFlags(objs..., member->GetSubType(), enumEntries, [=](RflObj<S>... objs) { return ProcessSingle(objs..., member, member->GetSubType()); });
				}
				else
					return 0;
			}

			typename V::result_type ProcessPointer(RflObj<S>... objs, const hh::fnd::RflClassMember* member) {
				return v.VisitPointer(static_cast<RflObj<S>*>(objs)..., [=](RflObj<S>... objs) { return ProcessSingle(objs..., member, member->GetSubType()); });
			}

			typename V::result_type ProcessSingleClassMember(RflObj<S>... objs, const hh::fnd::RflClassMember* member) {
				switch (member->GetType()) {
				case hh::fnd::RflClassMember::TYPE_ARRAY: return ProcessArray(objs..., member);
	#ifdef DEVTOOLS_TARGET_SDK_wars
				case hh::fnd::RflClassMember::TYPE_OLD_ARRAY: return ProcessOldArray(objs..., member);
	#endif
				case hh::fnd::RflClassMember::TYPE_POINTER: return ProcessPointer(objs..., member);
				case hh::fnd::RflClassMember::TYPE_ENUM: return ProcessEnum(objs..., member);
				case hh::fnd::RflClassMember::TYPE_FLAGS: return ProcessFlags(objs..., member);
				case hh::fnd::RflClassMember::TYPE_SIMPLE_ARRAY:
					assert(!"This RflClass member type (SIMPLE_ARRAY) is not implemented yet because it is unused.");
					return typename V::result_type{};
				default: return ProcessSingle(objs..., member, member->GetType());
				}
			}

			typename V::result_type ProcessClassMember(RflObj<S>... objs, RflObj<S>... parents, const hh::fnd::RflClassMember* member) {
				size_t constArrSizeOrZero = member->m_Flags & 0x40000 ? std::min({ reinterpret_cast<hh::fnd::DynamicRflArraySizeResolver>(member->m_pEnum)(parents)... }) : member->GetCstyleArraySize();

				if (constArrSizeOrZero == 0)
					return v.VisitClassMember(objs..., member, [=](RflObj<S>... objs) { return ProcessSingleClassMember(objs..., member); });
				else
					return v.VisitArrayClassMember(objs..., member, constArrSizeOrZero, [=](RflObj<S>... objs) {
						typename V::result_type arrayResult{};
						for (size_t j = 0; j < constArrSizeOrZero; j++)
							arrayResult |= v.VisitArrayClassMemberItem(
								reinterpret_cast<void*>(reinterpret_cast<size_t>(objs) + j * member->GetSingleSizeInBytes())...,
								member,
								j,
								[=](RflObj<S>... objs) { return ProcessSingleClassMember(objs..., member); }
							);
						return arrayResult;
					});
			}

			typename V::result_type ProcessClassMembers(RflObj<S>... objs, const hh::fnd::RflClass* rflClass) {
				const hh::fnd::RflClass* parent = rflClass->GetBaseType();

				typename V::result_type result{};

				if (parent != nullptr)
					result |= ProcessBaseStruct(objs..., parent);

				for (size_t i = 0; i < rflClass->m_pMembers.count; i++) {
					auto* member = &rflClass->m_pMembers.items[i];

					result |= ProcessClassMember(reinterpret_cast<void*>(reinterpret_cast<size_t>(objs) + member->m_Offset)..., objs..., member);
				}

				return result;
			}

			typename V::result_type ProcessBaseStruct(RflObj<S>... objs, const hh::fnd::RflClass* rflClass) {
				return v.VisitBaseStruct(objs..., rflClass, [=](RflObj<S>... objs) { return ProcessClassMembers(objs..., rflClass); });
			}

			typename V::result_type ProcessStruct(RflObj<S>... objs, const hh::fnd::RflClass* rflClass) {
				return v.VisitStruct(objs..., rflClass, [=](RflObj<S>... objs) { return ProcessClassMembers(objs..., rflClass); });
			}

		public:
			template<typename ...Args>
			rflop(Args&& ...args) : v{ std::forward<Args>(args)... } {}

			typename V::result_type operator()(RflObj<S>... objs, const hh::fnd::RflClass* rflClass) {
				return ProcessStruct(objs..., rflClass);
			}

			template<typename ...Args>
			static typename V::result_type Apply(RflObj<S>... objs, const hh::fnd::RflClass* rflClass, Args&& ...args) {
				return rflop<V>{ std::forward<Args>(args)... }(objs..., rflClass);
			}
			template<typename R, typename ...Args>
			static typename V::result_type Apply(RflObj<S, R*>... objs, Args&& ...args) {
				return rflop<V>(objs..., &R::rflClass, std::forward<Args>(args)...);
			}
		};
	}

	namespace visitors {
		class Copy {
		public:
			constexpr static size_t arity = 2;
			typedef int result_type;
			template<typename T>
			int VisitPrimitive(T* obj1, T* obj2) {
				*obj1 = *obj2;
				return 0;
			}
			int VisitPrimitive(csl::ut::VariableString* obj1, csl::ut::VariableString* obj2) {
				if (obj1->GetAllocator())
					obj1->Set(obj2->c_str());
				else
					obj1->Set(obj2->c_str(), -1, hh::fnd::MemoryRouter::GetModuleAllocator());

				return 0;
			}
			// Copy currently doesn't handle pointer-based data
			int VisitPrimitive(const char* obj1, const char* obj2) {
				assert(false);
				return 0;
			}
			template<typename F>
			int VisitPointer(void** obj1, void** obj2, F f) {
				assert(false);
				return 0;
			}
			template<typename F, template<typename> typename A, typename C, typename D>
			int VisitArray(RflArrayAccessor<A>& arr1, RflArrayAccessor<A>& arr2, C c, D d, F f) {
				arr1.clear();

				for (auto item : arr2) {
					arr1.emplace_back();
					f(arr1[arr1.size() - 1], item);
				}

				return 0;
			}
			template<typename F>
			int VisitEnum(void* obj1, void* obj2, hh::fnd::RflClassMember::Type type, const hh::fnd::RflClassEnum* enumClass, F f) {
				return f(obj1, obj2);
			}
			template<typename F>
			int VisitFlags(void* obj1, void* obj2, hh::fnd::RflClassMember::Type type, const hh::fnd::RflArray<const hh::fnd::RflClassEnumMember>* enumEntries, F f) {
				return f(obj1, obj2);
			}
			template<typename F>
			int VisitClassMember(void* obj1, void* obj2, const hh::fnd::RflClassMember* member, F f) {
				return f(obj1, obj2);
			}
			template<typename F>
			int VisitArrayClassMember(void* obj1, void* obj2, const hh::fnd::RflClassMember* member, size_t size, F f) {
				return f(obj1, obj2);
			}
			template<typename F>
			int VisitArrayClassMemberItem(void* obj1, void* obj2, const hh::fnd::RflClassMember* member, size_t idx, F f) {
				return f(obj1, obj2);
			}
			template<typename F>
			int VisitBaseStruct(void* obj1, void* obj2, const hh::fnd::RflClass* rflClass, F f) {
				return f(obj1, obj2);
			}
			template<typename F>
			int VisitStruct(void* obj1, void* obj2, const hh::fnd::RflClass* rflClass, F f) {
				return f(obj1, obj2);
			}
		};
	};

	using Copy = traversals::rflop<visitors::Copy>;
}
