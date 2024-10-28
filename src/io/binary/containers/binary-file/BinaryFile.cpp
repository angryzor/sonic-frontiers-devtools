#include <hedgelib/hl_internal.h>
#include "EndianSwaps.h"
#include <hedgelib/io/hl_bina.h>
#include <hedgelib/csl/hl_csl_move_array.h>
#include <hedgelib/io/hl_file.h>
#include <io/binary/serialization/Reflection.h>
#include <io/binary/serialization/resource-rfls/ResReflection.h>
#include <ui/Desktop.h>
#include "BinaryFile.h"
#include "Types.h"

namespace devtools::io::binary::containers {
	struct Backend {
		hl::bina::v2::writer64& writer;

	public:
		static constexpr bool hasNativeStrings = true;

		size_t tellp() const {
			return writer.tell();
		}
		void write_padding(size_t size) {
			return writer.pad(size);
		}
		template<typename T> void write(const T& obj) {
			writer.write_obj(obj, alignof(T));
		}
		template<> void write(const char* const& obj) {
			if (obj != nullptr)
				writer.add_string(obj, writer.tell());

			writer.write_obj(hl::off64<const char>{ nullptr });
		}
		template<typename T> void write(const serialized_types::o64_t<T>& obj) {
			writer.add_offset(writer.tell());
			writer.write_obj(hl::off64<T>{ obj.has_value() ? obj.value() : 0 });
		}
		template<typename T> void write(const serialized_types::o32_t<T>& obj) {
			writer.add_offset(writer.tell());
			writer.write_obj(hl::off32<T>{ obj.has_value() ? obj.value() : 0 });
		}
		template<typename T> void write(const serialized_types::MoveArray<T>& obj) {
			writer.add_offset(writer.tell());
			writer.write_obj(hl::csl::move_array64<T>{ obj.offset.has_value() ? obj.offset.value() : 0, obj.size, obj.capacity, obj.allocator });
		}
		template<typename T> void write(const serialized_types::MoveArray32<T>& obj) {
			writer.add_offset(writer.tell());
			writer.write_obj(hl::csl::move_array32<T>{ obj.offset.has_value() ? obj.offset.value() : 0, obj.size, obj.capacity, obj.allocator });
		}
		template<typename T> void write(const serialized_types::TArray<T>& obj) {
			writer.add_offset(writer.tell());
			writer.write_obj(hl::hh::tarray<T>{ obj.offset.has_value() ? obj.offset.value() : 0, obj.size, obj.capacity });
		}
	};

	void BinaryFile::Serialize(const wchar_t* filename, void* obj, const hh::fnd::RflClass* rflClass) {
		hl::file_stream stream{ filename, hl::file::mode::write };
		hl::bina::v2::writer64 writer{ stream };

		writer.start(hl::bina::endian_flag::little, hl::bina::v2::ver_210);
		writer.start_data_block();

		Backend backend{ writer };
		serialization::ReflectionSerializer<Backend> serializer{ Desktop::instance->GetAllocator(), backend, writer.tell() };

		serializer.Serialize(obj, rflClass);

		writer.finish_data_block();
		writer.finish();
	}

	void BinaryFile::SerializeRef2(const wchar_t* filename, void* obj, const hh::fnd::RflClass* rflClass) {
		hl::file_stream stream{ filename, hl::file::mode::write };
		hl::bina::v2::writer64 writer{ stream };

		writer.start(hl::bina::endian_flag::little, hl::bina::v2::ver_210);
		writer.start_data_block();

		Backend backend{ writer };
		serialization::ReflectionSerializer<Backend> serializer{ Desktop::instance->GetAllocator(), backend, writer.tell() };

		ResReflectionHeader header{ rflClass->nameHash };
		serializer.Serialize(&header, &hh::fnd::RflClassTraits<ResReflectionHeader>::rflClass);
		serializer.Serialize(obj, rflClass);

		writer.finish_data_block();
		writer.finish();
	}
}