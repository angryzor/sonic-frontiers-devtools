#include "ReflectionSerializer.h"
#include <hedgelib/csl/hl_csl_move_array.h>
#include <hedgelib/io/hl_file.h>

using namespace hh::fnd;

struct OffsetVarStr {
	hl::off64<const char> str;
	hl::off64<void> allocator;

	template<bool swapOffsets = true>
	void endian_swap() noexcept
	{
		hl::endian_swap<swapOffsets>(str);
	}
};

size_t ReflectionSerializer::EnqueueChunk(WorkQueueEntry chunk) {
	auto processed = processedPointers.Find(chunk.ptr);
	if (processed != processedPointers.end())
		return *processed;

	auto alignment = chunk.member->GetSubType() == RflClassMember::TYPE_VOID ? reinterpret_cast<RflClass * (*)(const void*)>(chunk.member->GetStructClass())(chunk.parentPtr)->GetAlignment() : chunk.member->GetSubTypeAlignment();
	auto misalignment = nextFreeAddress % alignment;

	chunk.dbgUnalignedOffset = nextFreeAddress;
	if (misalignment > 0)
		nextFreeAddress += alignment - misalignment;

	size_t offset = nextFreeAddress;
	chunk.dbgExpectedOffset = offset;
	chunk.id = curChunkId++;

	nextFreeAddress += chunk.member->GetSubType() == RflClassMember::TYPE_VOID ? reinterpret_cast<RflClass* (*)(const void*)>(chunk.member->GetStructClass())(chunk.parentPtr)->GetSizeInBytes() * chunk.size : chunk.member->GetSubTypeArraySizeInBytes(chunk.size);

	workQueue.push_back(chunk);

	return offset;
}

static size_t flagsToZeroMem{};

void ReflectionSerializer::WritePrimitive(const void* address, RflClassMember::Type type) {
	switch (type) {
	case RflClassMember::TYPE_BOOL:
		WriteCast<bool>(address);
		break;
	case RflClassMember::TYPE_SINT8:
		WriteCast<int8_t>(address);
		break;
	case RflClassMember::TYPE_UINT8:
		WriteCast<uint8_t>(address);
		break;
	case RflClassMember::TYPE_SINT16:
		WriteCast<int16_t>(address);
		break;
	case RflClassMember::TYPE_UINT16:
		WriteCast<uint16_t>(address);
		break;
	case RflClassMember::TYPE_SINT32:
		WriteCast<int32_t>(address);
		break;
	case RflClassMember::TYPE_UINT32:
		WriteCast<uint32_t>(address);
		break;
	case RflClassMember::TYPE_SINT64:
		WriteCast<int64_t>(address);
		break;
	case RflClassMember::TYPE_UINT64:
		WriteCast<uint64_t>(address);
		break;
	case RflClassMember::TYPE_FLOAT:
		WriteCast<float>(address);
		break;
	case RflClassMember::TYPE_VECTOR2:
		WriteCast<csl::math::Vector2>(address);
		break;
	case RflClassMember::TYPE_VECTOR3:
		WriteCast<csl::math::Vector3>(address);
		break;
	case RflClassMember::TYPE_VECTOR4:
		WriteCast<csl::math::Vector4>(address);
		break;
	case RflClassMember::TYPE_QUATERNION:
		WriteCast<csl::math::Vector4>(address);
		break;
	case RflClassMember::TYPE_MATRIX34:
		WriteCast<csl::math::Matrix34>(address);
		break;
	case RflClassMember::TYPE_MATRIX44:
		WriteCast<csl::math::Matrix44>(address);
		break;
	case RflClassMember::TYPE_OBJECT_ID:
		WriteCast<hh::game::ObjectId>(address);
		break;
	case RflClassMember::TYPE_POSITION:
		WriteCast<csl::math::Position>(address);
		break;
	case RflClassMember::TYPE_COLOR_BYTE:
		WriteCast<csl::ut::Color<uint8_t>>(address);
		break;
	case RflClassMember::TYPE_COLOR_FLOAT:
		WriteCast<csl::ut::Color<float>>(address);
		break;
	default:
		assert(!"unknown primitive type!");
	}
}

void ReflectionSerializer::WriteRflClass(const void* obj, const RflClass& rflClass) {
	size_t dbgStructStartLoc = writer.tell();
	const RflClass* parent = rflClass.GetBaseType();

	if (parent != nullptr) {
		WriteRflClass(obj, *parent);
	}

	for (uint32_t i = 0; i < rflClass.m_pMembers.count; i++) {
		const RflClassMember& member = rflClass.m_pMembers.items[i];
		size_t constArrSizeOrZero = member.GetCstyleArraySize();
		size_t constArrSize = constArrSizeOrZero == 0 ? 1 : constArrSizeOrZero;
		size_t alignment = member.GetAlignment();

		writer.pad(alignment);

		for (size_t j = 0; j < constArrSize; j++) {
			size_t memberSingleSize = member.GetSingleSizeInBytes();
			void* address = reinterpret_cast<void*>(reinterpret_cast<size_t>(obj) + member.m_Offset + j * memberSingleSize);

			// Catch alignment issues.
			assert((writer.tell() - dbgStructStartLoc) == (reinterpret_cast<size_t>(address) - reinterpret_cast<size_t>(obj)));

			switch (member.GetType()) {
			case RflClassMember::TYPE_POINTER:
				writer.add_offset(writer.tell());
				writer.write_obj(EnqueueChunk({ *static_cast<void**>(address), obj, &member, 1 })); 
				break;
			case RflClassMember::TYPE_ARRAY: {
				auto arr = static_cast<csl::ut::MoveArray<void*>*>(address);

				writer.add_offset(writer.tell());
				hl::csl::move_array64<void*> offsetarr{
					EnqueueChunk({ arr->begin(), obj, &member, arr->size() }),
					arr->size(),
					arr->capacity(),
					0ull,
				};

				writer.write_obj(offsetarr);
				break;
			}
			case RflClassMember::TYPE_SIMPLE_ARRAY:
				// Array that is 16 bytes large. I don't quite know what their layout would be (maybe 32b movearray?)
				// and the binary contains none of these, so just assert for now.
				assert(!"This RflClass member type (SIMPLE_ARRAY) is not implemented yet because it is unused.");
				break;
			case RflClassMember::TYPE_ENUM:
				WritePrimitive(address, member.GetSubType());
				break;
			case RflClassMember::TYPE_STRUCT:
				WriteRflClass(address, *member.GetStructClass());
				break;
			case RflClassMember::TYPE_FLAGS:
				WritePrimitive(address, member.GetSubType());
				break;
			case RflClassMember::TYPE_CSTRING: {
				auto str = *static_cast<const char**>(address);

				if (str != nullptr)
					writer.add_string(str, writer.tell());

				writer.write_obj(hl::off64<const char>{ nullptr });
				break;
			}
			case RflClassMember::TYPE_STRING: {
				auto str = *static_cast<const char**>(address);

				if (str != nullptr)
					writer.add_string(str, writer.tell());

				writer.write_obj(OffsetVarStr{ nullptr, nullptr });
				break;
			}
			default:
				if (member.GetSubType() == RflClassMember::TYPE_FLAGS)
					WritePrimitive(&flagsToZeroMem, member.GetType());
				else
					WritePrimitive(address, member.GetType());
				break;
			}

			// Catch alignment issues.
			assert((writer.tell() - dbgStructStartLoc) == (reinterpret_cast<size_t>(address) + memberSingleSize - reinterpret_cast<size_t>(obj)));
		}
	}

	writer.pad(rflClass.GetAlignment());
}

void ReflectionSerializer::Write(const void* obj, const RflClass& rflClass) {
	writer.start(hl::bina::endian_flag::little, hl::bina::v2::ver_210);
	writer.start_data_block();

	nextFreeAddress = rflClass.GetSizeInBytes();
	WriteRflClass(obj, rflClass);

	size_t chunkId = 0;
	while (!workQueue.empty()) {
		WorkQueueEntry chunk = *workQueue.begin();
		workQueue.remove(0);

		switch (chunk.member->GetSubType()) {
		case RflClassMember::TYPE_VOID: {
			const RflClass* rflClass = reinterpret_cast<RflClass* (*)(const void*)>(chunk.member->GetStructClass())(chunk.parentPtr);
			size_t itemSize = rflClass->GetSizeInBytes();
			size_t alignment = rflClass->GetAlignment();

			writer.pad(alignment);
			assert(chunk.dbgExpectedOffset == writer.tell() - 0x40);

			for (size_t i = 0; i < chunk.size; i++)
				WriteRflClass(reinterpret_cast<void*>(reinterpret_cast<size_t>(chunk.ptr) + i * itemSize), *rflClass);

			break;
		}
		case RflClassMember::TYPE_STRUCT: {
			size_t itemSize = chunk.member->GetSubTypeSizeInBytes();
			size_t alignment = chunk.member->GetSubTypeAlignment();

			writer.pad(alignment);
			assert(chunk.dbgExpectedOffset == writer.tell() - 0x40);

			for (size_t i = 0; i < chunk.size; i++)
				WriteRflClass(reinterpret_cast<void*>(reinterpret_cast<size_t>(chunk.ptr) + i * itemSize), *chunk.member->GetStructClass());

			break;
		}
		default: {
			size_t itemSize = chunk.member->GetSubTypeSizeInBytes();
			size_t alignment = chunk.member->GetSubTypeAlignment();

			writer.pad(alignment);
			assert(chunk.dbgExpectedOffset == writer.tell() - 0x40);

			for (size_t i = 0; i < chunk.size; i++)
				WritePrimitive(reinterpret_cast<void*>(reinterpret_cast<size_t>(chunk.ptr) + i * itemSize), chunk.member->GetSubType());

			break;
		}
		}
		chunkId++;
	}

	writer.finish_data_block();
	writer.finish();
}

ReflectionSerializer::ReflectionSerializer(csl::fnd::IAllocator* allocator, hl::stream& stream)
	: CompatibleObject{ allocator }
	, stream{ stream }
{
}

void ReflectionSerializer::SerializeToFile(const hl::nchar* filename, const void* obj, const RflClass& rflClass) {
	hl::file_stream stream{ filename, hl::file::mode::write };
	ReflectionSerializer serializer{ hh::fnd::MemoryRouter::GetModuleAllocator(), stream };
	serializer.Write(obj, rflClass);
}
