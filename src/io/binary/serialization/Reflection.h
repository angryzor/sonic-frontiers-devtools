#pragma once
#include <utilities/RflClassGenFwd.h>
#include <utilities/CompatibleObject.h>
#include <reflection/RflArrayAccessor.h>
#include <reflection/ReflectiveOperations.h>
#include <reflection/Concepts.h>
#include "Types.h"

namespace devtools::io::binary::serialization {
	static const hh::fnd::RflClass* GetStructMemberClass(const hh::fnd::RflClassMember* member, const void* obj, const void* parent) {
		return member->m_Flags & 0x10000
			? reinterpret_cast<hh::fnd::DynamicRflClassResolver>(member->GetStructClass())(parent)
			: member->m_Flags & 0x80000
			? reinterpret_cast<hh::fnd::DynamicRflClassResolver>(member->GetStructClass())(obj)
			: member->GetStructClass();
	}
	
	static size_t GetRflClassSize(const hh::fnd::RflClass* rflClass, const void* obj) {
		size_t baseSize = rflClass->GetSizeInBytes();
		auto memberCount = rflClass->m_pMembers.count;

		if (memberCount == 0)
			return baseSize;

		auto& lastMember = rflClass->m_pMembers.items[memberCount - 1];

		if (!(lastMember.m_Flags & 0x40000))
			return baseSize;

		auto unalignedSize = lastMember.m_Offset + reinterpret_cast<hh::fnd::DynamicRflArraySizeResolver>(lastMember.m_pEnum)(obj) * (baseSize - lastMember.m_Offset);

		return (unalignedSize + rflClass->GetAlignment() - 1) & ~(rflClass->GetAlignment() - 1);
	}

	template<typename B>
	class ReflectionSerializer : public CompatibleObject {
		struct WorkQueueEntry {
			enum class Type {
				MEMBER,
				STRING,
			};

			size_t id;
			Type type;
			size_t dbgExpectedOffset;
			const void* ptr;
			const void* parentPtr;
			const hh::fnd::RflClassMember* member;
			size_t arraySize;
			std::function<int(void*)> processFunc;
		};

		B& backend;
		size_t curChunkId;
		csl::ut::PointerMap<const void*, serialized_types::o64_t<void*>> processedPointers{ GetAllocator() };
		csl::ut::MoveArray<WorkQueueEntry> workQueue{ GetAllocator() };
		size_t nextFreeAddress{};
		const hh::fnd::RflClassMember* currentMember{};

		size_t AllocateBlock(size_t size, size_t alignment) {
			assert(size != 0);
			size_t offset = (nextFreeAddress + alignment - 1) & ~(alignment - 1);
			nextFreeAddress = offset + size;
			return offset;
		}

		template<typename F>
		serialized_types::o64_t<void*> EnqueueMember(const void* obj, const void* parent, const hh::fnd::RflClassMember* member, size_t arraySize, F f) {
			auto processed = processedPointers.Find(obj);
			if (processed != processedPointers.end())
				return *processed;

			// TODO: add to processed pointers

			auto alignment = member->GetSubType() == hh::fnd::RflClassMember::TYPE_STRUCT ? GetStructMemberClass(member, obj, parent)->GetAlignment() : member->GetSubTypeAlignment();
			auto bufferSize = member->GetSubType() == hh::fnd::RflClassMember::TYPE_STRUCT ? GetRflClassSize(GetStructMemberClass(member, obj, parent), obj) * arraySize : member->GetSubTypeArraySizeInBytes(arraySize);

			size_t offset = AllocateBlock(bufferSize, alignment);

			workQueue.push_back({ curChunkId++, WorkQueueEntry::Type::MEMBER, offset, obj, parent, member, arraySize, f });

			serialized_types::o64_t<void*> result{ offset };

			processedPointers.Insert(obj, result);

			return result;
		}

		serialized_types::o64_t<const char> EnqueueString(const char* str) {
			size_t offset = AllocateBlock(strlen(str) + 1, 1);

			workQueue.push_back({ curChunkId++, WorkQueueEntry::Type::STRING, offset, str });

			return { offset };
		}

		class SerializeChunk {
		public:
			constexpr static size_t arity = 1;
			typedef int result_type;

			ReflectionSerializer& serializer;
			size_t dbgStructStartLoc{};
			void* currentStructAddr{};

			SerializeChunk(ReflectionSerializer& serializer) : serializer{ serializer } {}

			template<typename T, std::enable_if_t<!std::is_fundamental_v<T>, bool> = true>
			int VisitPrimitive(T* obj) {
				serializer.backend.write<T>(*obj);
				return 0;
			}
			template<typename T, std::enable_if_t<std::is_fundamental_v<T>, bool> = true>
			int VisitPrimitive(T* obj) {
				serializer.backend.write(serializer.currentMember->m_Flags & 0x100000 ? T{} : *obj);
				return 0;
			}
			int VisitPrimitive(const char** obj) {
				if constexpr (B::hasNativeStrings)
					serializer.backend.write(*obj);
				else
					serializer.backend.write(*obj == nullptr ? serialized_types::o64_t<const char>{} : serializer.EnqueueString(*obj));
				return 0;
			}
			int VisitPrimitive(csl::ut::VariableString* obj) {
				VisitPrimitive(reinterpret_cast<const char**>(obj));
				serializer.backend.write(0ull);
				return 0;
			}
			template<typename F>
			int VisitEnum(void* obj, hh::fnd::RflClassMember::Type type, const hh::fnd::RflClassEnum* enumClass, F f) {
				return f(obj);
			}
			template<typename F>
			int VisitFlags(void* obj, hh::fnd::RflClassMember::Type type, const hh::fnd::RflArray<const hh::fnd::RflClassEnumMember>* enumEntries, F f) {
				return f(obj);
			}
			template<typename F, typename C, typename D>
			int VisitArray(RflArrayAccessor<csl::ut::MoveArray>& arr, C c, D d, F f) {
				serializer.backend.write(serialized_types::MoveArray<void*>{
					arr[0] == nullptr ? serialized_types::o64_t<void*>{} : serializer.EnqueueMember(arr[0], currentStructAddr, serializer.currentMember, arr.size(), f),
					arr.size(),
					arr.capacity(),
					0ull,
				});
				return 0;
			}
	#ifdef DEVTOOLS_TARGET_SDK_wars
			template<typename F, typename C, typename D>
			int VisitArray(RflArrayAccessor<hh::TArray>& arr, C c, D d, F f) {
				serializer.backend.write(serialized_types::TArray<void*>{
					arr[0] == nullptr ? serialized_types::o64_t<void*>{} : serializer.EnqueueMember(arr[0], currentStructAddr, serializer.currentMember, arr.size(), f),
					arr.size(),
					static_cast<int64_t>(arr.capacity()),
				});
				return 0;
			}
	#endif
			template<typename F>
			int VisitPointer(void** obj, F f) {
				size_t arraySize = serializer.currentMember->m_Flags & 0x20000 ? reinterpret_cast<hh::fnd::DynamicRflArraySizeResolver>(serializer.currentMember->m_pEnum)(currentStructAddr) : 1;

				serializer.backend.write(*obj == nullptr ? serialized_types::o64_t<void*>{} : serializer.EnqueueMember(*obj, currentStructAddr, serializer.currentMember, arraySize, f));
				return 0;
			}
			template<typename F>
			int VisitClassMember(void* obj, const hh::fnd::RflClassMember* member, F f) {
				const hh::fnd::RflClassMember* prevMember = serializer.currentMember;
				serializer.currentMember = member;

				serializer.backend.write_padding(member->GetAlignment());

				// Catch alignment issues.
				assert((serializer.backend.tellp() - dbgStructStartLoc) == (reinterpret_cast<size_t>(obj) - reinterpret_cast<size_t>(currentStructAddr)));

				f(obj);

				// Catch alignment issues.
				assert((serializer.backend.tellp() - dbgStructStartLoc) == (reinterpret_cast<size_t>(obj) + member->GetSingleSizeInBytes() - reinterpret_cast<size_t>(currentStructAddr)));

				serializer.currentMember = prevMember;
				return 0;
			}
			template<typename F>
			int VisitArrayClassMember(void* obj, const hh::fnd::RflClassMember* member, size_t size, F f) {
				const hh::fnd::RflClassMember* prevMember = serializer.currentMember;
				serializer.currentMember = member;

				serializer.backend.write_padding(member->GetAlignment());

				f(obj);

				serializer.currentMember = prevMember;
				return 0;
			}
			template<typename F>
			int VisitArrayClassMemberItem(void* obj, const hh::fnd::RflClassMember* member, size_t idx, F f) {
				// Catch alignment issues.
				assert((serializer.backend.tellp() - dbgStructStartLoc) == (reinterpret_cast<size_t>(obj) - reinterpret_cast<size_t>(currentStructAddr)));

				f(obj);

				// Catch alignment issues.
				assert((serializer.backend.tellp() - dbgStructStartLoc) == (reinterpret_cast<size_t>(obj) + member->GetSingleSizeInBytes() - reinterpret_cast<size_t>(currentStructAddr)));
				return 0;
			}
			template<typename F>
			int VisitBaseStruct(void* obj, const hh::fnd::RflClass* rflClass, F f) {
				f(obj);

				serializer.backend.write_padding(rflClass->GetAlignment());
				return 0;
			}
			template<typename F>
			int VisitStruct(void* obj, const hh::fnd::RflClass* rflClass, F f) {
				size_t prevDbgStructStartLoc = dbgStructStartLoc;
				void* prevStructAddr = currentStructAddr;

				dbgStructStartLoc = serializer.backend.tellp();
				currentStructAddr = obj;

				f(obj);

				serializer.backend.write_padding(rflClass->GetAlignment());

				dbgStructStartLoc = prevDbgStructStartLoc;
				currentStructAddr = prevStructAddr;
				return 0;
			}
		};

		rflops::traversals::rflop<SerializeChunk> serializeChunkOp{ *this };

		void ProcessChunk(const WorkQueueEntry& chunk) {
			switch (chunk.type) {
			case WorkQueueEntry::Type::STRING: {
				if constexpr (!B::hasNativeStrings)
					backend.write_string(static_cast<const char*>(chunk.ptr));
				break;
			}
			case WorkQueueEntry::Type::MEMBER:
				switch (chunk.member->GetSubType()) {
				case hh::fnd::RflClassMember::TYPE_STRUCT: {
					const hh::fnd::RflClass* rflClass = GetStructMemberClass(chunk.member, chunk.ptr, chunk.parentPtr);
					size_t itemSize = GetRflClassSize(rflClass, chunk.ptr);
					size_t alignment = rflClass->GetAlignment();

					backend.write_padding(alignment);
					assert(chunk.dbgExpectedOffset == backend.tellp());

					for (size_t i = 0; i < chunk.arraySize; i++) {
						auto* obj = reinterpret_cast<void*>(reinterpret_cast<size_t>(chunk.ptr) + i * itemSize);
						auto* objRflClass = GetStructMemberClass(chunk.member, obj, chunk.parentPtr);

						assert(GetRflClassSize(objRflClass, obj) == itemSize);

						serializeChunkOp(obj, objRflClass);
					}

					break;
				}
				default: {
					size_t itemSize = chunk.member->GetSubTypeSizeInBytes();
					size_t alignment = chunk.member->GetSubTypeAlignment();

					backend.write_padding(alignment);
					assert(chunk.dbgExpectedOffset == backend.tellp());

					const hh::fnd::RflClassMember* prevMember = currentMember;
					currentMember = chunk.member;

					for (size_t i = 0; i < chunk.arraySize; i++)
						chunk.processFunc(reinterpret_cast<void*>(reinterpret_cast<size_t>(chunk.ptr) + i * itemSize));

					currentMember = prevMember;

					break;
				}
				}
				break;
			}
		}

	public:
		ReflectionSerializer(csl::fnd::IAllocator* allocator, B& backend) : CompatibleObject{ allocator }, backend { backend } {}

		ReflectionSerializer(const ReflectionSerializer& other) = delete;

		void Serialize(const void* obj, const hh::fnd::RflClass* rflClass, size_t count = 1) {
			nextFreeAddress = backend.tellp();

			auto classSize = GetRflClassSize(rflClass, obj);

			AllocateBlock(classSize * count, rflClass->GetAlignment());

			for (size_t i = 0; i < count; i++)
				serializeChunkOp(reinterpret_cast<void*>(reinterpret_cast<size_t>(obj) + i * classSize), rflClass);

			size_t chunkId = 0;
			while (!workQueue.empty()) {
				WorkQueueEntry chunk = *workQueue.begin();
				workQueue.remove(0);

				ProcessChunk(chunk);

				chunkId++;
			}
		}

		template<devtools::rfl::RflClass T>
		void Serialize(const T* obj, size_t count = 1) {
			Serialize(obj, &RESOLVE_STATIC_VARIABLE(T::rflClass), count);
		}

		template<typename T, typename = hh::fnd::RflClassTraits<T>>
		void Serialize(const T* obj, size_t count = 1) {
			Serialize(obj, &hh::fnd::RflClassTraits<T>::rflClass, count);
		}
	};
}