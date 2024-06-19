#include <hedgelib/hl_internal.h>
#include "EndianSwaps.h"
#include <hedgelib/io/hl_bina.h>
#include <utilities/CompatibleObject.h>

struct WorkQueueEntry {
	size_t id;
	const void* ptr;
	const void* parentPtr;
	const hh::fnd::RflClassMember* member;
	size_t size;
	size_t dbgUnalignedOffset;
	size_t dbgExpectedOffset;

	WorkQueueEntry(const void* ptr, const void* parentPtr, const hh::fnd::RflClassMember* member, size_t size) : ptr{ ptr }, parentPtr{ parentPtr }, member{ member }, size{ size }, dbgExpectedOffset{ 0 } {}
};

class ReflectionSerializer : public CompatibleObject {
	hl::stream& stream;
	hl::bina::v2::writer64 writer{ stream };
	hl::off_table offTable{};
	hl::str_table strTable{};

	size_t curChunkId;
	size_t nextFreeAddress;
	csl::ut::PointerMap<const void*, size_t> processedPointers{ GetAllocator() };
	csl::ut::MoveArray<WorkQueueEntry> workQueue{ GetAllocator() };

	size_t EnqueueChunk(WorkQueueEntry chunk);
	template<typename T>
	void WriteCast(const void* obj) {
		writer.write_obj<T>(*static_cast<const T*>(obj), alignof(T));
	}
	void WritePrimitive(const void* address, hh::fnd::RflClassMember::Type type);
	void WriteRflClassMember(const void* obj, const hh::fnd::RflClassMember& member);
	void WriteRflClass(const void* obj, const hh::fnd::RflClass& rflClass);
	void Write(const void* obj, const hh::fnd::RflClass& rflClass);
	ReflectionSerializer(csl::fnd::IAllocator* allocator, hl::stream& stream);
public:
	static void SerializeToFile(const hl::nchar* filename, const void* obj, const hh::fnd::RflClass& rflClass);
	template<typename T>
	static void SerializeToFile(const hl::nchar* filename, T* obj) {
		SerializeToFile(filename, obj, RESOLVE_STATIC_VARIABLE(T::rflClass));
	}
};
