#pragma once
#include "RflMoveArrayAccessor.h"

namespace rflops {
	namespace traversals {
		template<typename V, typename = std::make_index_sequence<V::arity>>
		class rflop;
		template<typename V, std::size_t... S>
		class rflop<V, std::index_sequence<S...>> {
			template<std::size_t, typename T = void*> using RflObj = T;

			template<typename U>
			static typename V::result_type ProcessPrimitive(RflObj<S>... objs) {
				return V::VisitPrimitive(static_cast<U*>(objs)...);
			}

			static typename V::result_type ProcessPrimitive(RflObj<S>... objs, const hh::fnd::RflClassMember::Type type) {
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
				case hh::fnd::RflClassMember::TYPE_POSITION: return ProcessPrimitive<csl::math::Vector3>(objs...);
				default:
					assert(!"rfl editor assertion failed: unknown primitive type");
					return false;
				}
			}

			static typename V::result_type ProcessSingle(RflObj<S>... objs, const hh::fnd::RflClassMember* member, const hh::fnd::RflClassMember::Type type)
			{
				switch (type) {
				case hh::fnd::RflClassMember::TYPE_STRUCT:
					return ProcessStruct(objs..., member->m_pClass);
				default:
					return ProcessPrimitive(objs..., type);
				}
			}

			static typename V::result_type ProcessArray(RflObj<S>... objs, const hh::fnd::RflClassMember* member)
			{
				return V::VisitArray(
					RflMoveArrayAccessor{ *static_cast<csl::ut::MoveArray<void*>*>(objs), member }...,
					[member]() {
						void* obj = new (std::align_val_t(16), hh::fnd::MemoryRouter::GetModuleAllocator()) char[member->GetSubTypeSizeInBytes()];

						if (member->GetSubType() == hh::fnd::RflClassMember::TYPE_STRUCT)
							hh::fnd::RflTypeInfoRegistry::GetInstance()->ConstructObject(hh::fnd::MemoryRouter::GetModuleAllocator(), obj, member->GetStructClass()->GetName());
						
						return obj;
					},
					[member](void* obj) {
						if (member->GetSubType() == hh::fnd::RflClassMember::TYPE_STRUCT)
							hh::fnd::RflTypeInfoRegistry::GetInstance()->CleanupLoadedObject(obj, member->GetStructClass()->GetName());

						hh::fnd::MemoryRouter::GetModuleAllocator()->Free(obj);
					},
					[member](RflObj<S>... objs) { return ProcessSingle(objs..., member, member->GetSubType()); }
				);
			}

			static typename V::result_type ProcessEnum(RflObj<S>... objs, const hh::fnd::RflClassMember* member) {
				return V::VisitEnum(objs..., member->GetSubType(), member->GetEnumClass(), [member](RflObj<S>... objs) { return ProcessSingle(objs..., member, member->GetSubType()); });
			}

			static typename V::result_type ProcessFlags(RflObj<S>... objs, const hh::fnd::RflClassMember* member) {
				auto* enumEntries = reinterpret_cast<const hh::fnd::RflArray<const hh::fnd::RflClassEnumMember>*>(member->GetAttribute("DisplayIndex")->GetData());

				return V::VisitFlags(objs..., member->GetSubType(), enumEntries, [member](RflObj<S>... objs) { return ProcessSingle(objs..., member, member->GetSubType()); });
			}

			static typename V::result_type ProcessClassMember(RflObj<S>... objs, const hh::fnd::RflClassMember* member) {
				switch (member->GetType()) {
				case hh::fnd::RflClassMember::TYPE_ARRAY: return ProcessArray(objs..., member);
				case hh::fnd::RflClassMember::TYPE_POINTER: return ProcessSingle(*static_cast<void**>(objs)..., member, member->GetSubType());
				case hh::fnd::RflClassMember::TYPE_ENUM: return ProcessEnum(objs..., member);
				case hh::fnd::RflClassMember::TYPE_FLAGS: return ProcessFlags(objs..., member);
				case hh::fnd::RflClassMember::TYPE_SIMPLE_ARRAY:
					assert(!"This RflClass member type (SIMPLE_ARRAY) is not implemented yet because it is unused.");
					return typename V::result_type{};
				default: return ProcessSingle(objs..., member, member->GetType());
				}
			}

			static typename V::result_type ProcessStructMembers(RflObj<S>... objs, const hh::fnd::RflClass* rflClass) {
				const hh::fnd::RflClass* parent = rflClass->GetBaseType();

				typename V::result_type result{};

				if (parent != nullptr)
					result |= ProcessStructMembers(objs..., parent);

				for (size_t i = 0; i < rflClass->m_pMembers.count; i++) {
					const hh::fnd::RflClassMember* member = &rflClass->m_pMembers.items[i];
					size_t constArrSizeOrZero = member->GetCstyleArraySize();

					if (constArrSizeOrZero == 0)
						result |= V::VisitClassMember(objs..., member, [member](RflObj<S>... objs) { return ProcessClassMember(reinterpret_cast<void*>(reinterpret_cast<size_t>(objs) + member->m_Offset)..., member); });
					else
						result |= V::VisitArrayClassMember(objs..., member, constArrSizeOrZero, [member, constArrSizeOrZero](RflObj<S>... objs) {
							typename V::result_type arrayResult{};
							for (size_t j = 0; j < constArrSizeOrZero; j++)
								arrayResult |= V::VisitArrayClassMemberItem(objs..., j, [member, j](RflObj<S>... objs){ return ProcessClassMember(reinterpret_cast<void*>(reinterpret_cast<size_t>(objs) + member->m_Offset + j * member->GetSingleSizeInBytes())..., member); });
							return arrayResult;
						});
				}

				return result;
			}

			static typename V::result_type ProcessStruct(RflObj<S>... objs, const hh::fnd::RflClass* rflClass) {
				return V::VisitStruct(objs..., rflClass, [rflClass](RflObj<S>... objs) { return ProcessStructMembers(objs..., rflClass); });
			}


		public:
			static typename V::result_type Apply(RflObj<S>... objs, const hh::fnd::RflClass* rflClass) {
				return ProcessStruct(objs..., rflClass);
			}
			template<typename R>
			static typename V::result_type Apply(RflObj<S, R*>... objs) {
				return Apply(objs..., &R::rflClass);
			}
		};
	}

	namespace visitors {
		class Copy {
		public:
			constexpr static size_t arity = 2;
			typedef int result_type;
			template<typename T>
			static int VisitPrimitive(T* obj1, T* obj2) {
				*obj1 = *obj2;
				return 0;
			}
			static int VisitPrimitive(csl::ut::VariableString* obj1, csl::ut::VariableString* obj2) {
				if (obj1->GetAllocator())
					obj1->Set(obj2->c_str());
				else
					obj1->Set(obj2->c_str(), -1, hh::fnd::MemoryRouter::GetModuleAllocator());

				return 0;
			}
			static int VisitPrimitive(const char* obj1, const char* obj2) {
				assert(false);
				return 0;
			}
			template<typename F, typename C, typename D>
			static int VisitArray(RflMoveArrayAccessor& arr1, RflMoveArrayAccessor& arr2, C c, D d, F f) {
				arr1.clear();

				for (auto item : arr2) {
					arr1.emplace_back();
					f(arr1[arr1.size() - 1], item);
				}

				return 0;
			}
			template<typename F>
			static int VisitEnum(void* obj1, void* obj2, hh::fnd::RflClassMember::Type type, const hh::fnd::RflClassEnum* enumClass, F f) {
				return f(obj1, obj2);
			}
			template<typename F>
			static int VisitFlags(void* obj1, void* obj2, hh::fnd::RflClassMember::Type type, const hh::fnd::RflArray<const hh::fnd::RflClassEnumMember>* enumEntries, F f) {
				return f(obj1, obj2);
			}
			template<typename F>
			static int VisitClassMember(void* obj1, void* obj2, const hh::fnd::RflClassMember* member, F f) {
				return f(obj1, obj2);
			}
			template<typename F>
			static int VisitArrayClassMember(void* obj1, void* obj2, const hh::fnd::RflClassMember* member, size_t size, F f) {
				return f(obj1, obj2);
			}
			template<typename F>
			static int VisitArrayClassMemberItem(void* obj1, void* obj2, size_t idx, F f) {
				return f(obj1, obj2);
			}
			template<typename F>
			static int VisitStruct(void* obj1, void* obj2, const hh::fnd::RflClass* rflClass, F f) {
				return f(obj1, obj2);
			}
		};
	};

	using Copy = traversals::rflop<visitors::Copy>;
}
