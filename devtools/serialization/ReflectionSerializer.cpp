#include "../Pch.h"
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

size_t ReflectionSerializer::EnqueueChunk(const WorkQueueEntry& chunk) {
	auto processed = processedPointers.Find(chunk.ptr);
	if (processed != processedPointers.end())
		return *processed;

	auto offset = nextFreeAddress;

	nextFreeAddress += chunk.member->GetSubTypeArraySizeInBytes(chunk.size);

	workQueue.push_back(chunk);

	return offset;
}

void ReflectionSerializer::WritePrimitive(void* address, RflClassMember::Type type) {
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
		WriteCast<uint32_t>(address);
		break;
	case RflClassMember::TYPE_POSITION:
		WriteCast<csl::math::Vector3>(address);
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

void ReflectionSerializer::WriteRflClass(void* obj, const RflClass& rflClass) {
	size_t dbgStructStartLoc = writer.tell();
	const RflClass* parent = rflClass.GetBaseType();

	if (parent != nullptr) {
		WriteRflClass(obj, *parent);
		writer.pad(parent->GetAlignment());
	}

	for (uint32_t i = 0; i < rflClass.m_pMembers.count; i++) {
		const RflClassMember& member = rflClass.m_pMembers.items[i];
		size_t constArrSizeOrZero = member.GetCstyleArraySize();
		size_t constArrSize = constArrSizeOrZero == 0 ? 1 : constArrSizeOrZero;
		size_t alignment = member.GetAlignment();

		for (size_t j = 0; j < constArrSize; j++) {
			size_t memberSingleSize = member.GetSingleSizeInBytes();
			void* address = reinterpret_cast<void*>(reinterpret_cast<size_t>(obj) + member.m_Offset + j * memberSingleSize);

			writer.pad(alignment);

			// Catch alignment issues.
			assert((writer.tell() - dbgStructStartLoc) == (reinterpret_cast<size_t>(address) - reinterpret_cast<size_t>(obj)));

			switch (member.GetType()) {
			case RflClassMember::TYPE_POINTER:
				writer.add_offset(writer.tell());
				writer.write_obj(EnqueueChunk({ address, &member, 1 })); 
				break;
			case RflClassMember::TYPE_ARRAY: {
				auto arr = static_cast<csl::ut::MoveArray<void*>*>(address);

				writer.add_offset(writer.tell());
				hl::csl::move_array64<void*> offsetarr{
					EnqueueChunk({ address, &member, arr->size() }),
					arr->size(),
					arr->capacity() | csl::ut::SIGN_BIT,
					0ui64,
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
				WritePrimitive(address, member.GetType());
			}
			writer.pad(alignment);

			// Catch alignment issues.
			assert((writer.tell() - dbgStructStartLoc) == (reinterpret_cast<size_t>(address) + memberSingleSize - reinterpret_cast<size_t>(obj)));
		}
	}
}

void ReflectionSerializer::Write(void* obj, const RflClass& rflClass) {
	writer.start(hl::bina::endian_flag::little, hl::bina::v2::ver_210);
	writer.start_data_block();

	nextFreeAddress = rflClass.GetSizeInBytes();
	WriteRflClass(obj, rflClass);

	writer.finish_data_block();
	writer.finish();
}

ReflectionSerializer::ReflectionSerializer(csl::fnd::IAllocator* allocator, hl::stream& stream)
	: BaseObject{ allocator }
	, stream{ stream }
{
}

void ReflectionSerializer::SerializeToFile(const hl::nchar* filename, void* obj, const RflClass& rflClass) {
	hl::file_stream stream{ filename, hl::file::mode::write };
	ReflectionSerializer serializer{ GetTempAllocator(GetAllocatorSystem()), stream };
	serializer.Write(obj, rflClass);
}
