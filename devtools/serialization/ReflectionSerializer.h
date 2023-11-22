#include <hedgelib/hl_internal.h>
#include "EndianSwaps.h"
#include <hedgelib/io/hl_bina.h>

struct WorkQueueEntry {
	void* ptr;
	void* parentPtr;
	const hh::fnd::RflClassMember* member;
	size_t size;
};

class ReflectionSerializer : public hh::fnd::BaseObject {
	hl::stream& stream;
	hl::bina::v2::writer64 writer{ stream };
	hl::off_table offTable{};
	hl::str_table strTable{};

	size_t nextFreeAddress;
	csl::ut::PointerMap<void*, size_t> processedPointers{ GetAllocator() };
	csl::ut::MoveArray<WorkQueueEntry> workQueue{ GetAllocator() };

	size_t EnqueueChunk(const WorkQueueEntry& chunk);
	template<typename T>
	void WriteCast(void* obj) {
		writer.write_obj<T>(*static_cast<T*>(obj), alignof(T));
	}
	void WritePrimitive(void* address, hh::fnd::RflClassMember::Type type);
	void WriteRflClassMember(void* obj, const hh::fnd::RflClassMember& member);
	void WriteRflClass(void* obj, const hh::fnd::RflClass& rflClass);
	void Write(void* obj, const hh::fnd::RflClass& rflClass);
	ReflectionSerializer(csl::fnd::IAllocator* allocator, hl::stream& stream);
public:
	static void SerializeToFile(const hl::nchar* filename, void* obj, const hh::fnd::RflClass& rflClass);
};
