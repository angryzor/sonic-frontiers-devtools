#include "SWIF.h"
#include <io/stream.h>
#include <io/binary/serialization/resource-rfls/ResSurfRideProject.h>
#include <io/binary/serialization/Reflection.h>
#include <ui/Desktop.h>
#include <utilities/CompatibleObject.h>

namespace devtools::io::binary::containers::swif {
	static constexpr const unsigned int SWIF = 0x46495753;
	static constexpr const unsigned int SWTL = 0x4C545753;
	static constexpr const unsigned int SWPR = 0x52505753;
	static constexpr const unsigned int SEND = 0x444E4553;
	static constexpr const unsigned int SOF0 = 0x30464F53;

	class SWIFSerializer : public CompatibleObject {
		class SerializationBackend {
			SWIFSerializer& serializer;

		public:
			SerializationBackend(SWIFSerializer& serializer) : serializer{ serializer } {}

			static constexpr bool hasNativeStrings = false;

			template<typename T>
			void write(const T& obj) {
				serializer.stream.write(obj);
			}

			template<typename T>
			void write(const serialized_types::o64_t<T>& obj) {
				if (obj.has_value()) {
					serializer.addressLocations.push_back(static_cast<unsigned int>(serializer.stream.tellp()));
					serializer.stream.write(obj.value());
				}
				else
					serializer.stream.write(0ull);
			}

			template<typename T>
			void write(const serialized_types::o32_t<T>& obj) {
				assert(!"This container only supports 64-bit offsets");
			}

			template<typename T>
			void write(const serialized_types::MoveArray<T>& obj) {
				write(obj.offset);
				serializer.stream.write(obj.size);
				serializer.stream.write(obj.capacity);
				serializer.stream.write(obj.allocator);
			}

			template<typename T>
			void write(const serialized_types::MoveArray32<T>& obj) {
				write(obj.offset);
				serializer.stream.write(obj.size);
				serializer.stream.write(obj.capacity);
				serializer.stream.write(obj.allocator);
			}

			template<typename T>
			void write(const serialized_types::TArray<T>& obj) {
				write(obj.offset);
				serializer.stream.write(obj.size);
				serializer.stream.write(obj.capacity);
			}

			size_t tellp() {
				return serializer.stream.tellp();
			}

			void write_padding(size_t alignment) {
				serializer.stream.write_padding(alignment);
			}

			void write_string(const char* obj) {
				assert(obj != nullptr);
				serializer.stream.write(*obj, strlen(obj) + 1);
			}
		};

		binary_ostream& stream;
		csl::ut::MoveArray<unsigned int> addressLocations{ GetAllocator() };
		SerializationBackend backend{ *this };
		serialization::ReflectionSerializer<SerializationBackend> reflectionSerializer{ GetAllocator(), backend };
		size_t addressResolutionChunkOffset{};
		size_t chunksStart{};
		unsigned int chunkCount{};

		template<typename H, typename F>
		void WriteChunk(unsigned int magic, H&& header, F&& writeFunc) {
			size_t start = stream.tellp();

			SurfRide::SRS_CHUNK_HEADER chunkHeader{ magic, 0 };
			stream.write(chunkHeader);

			size_t dataStart = stream.tellp();

			stream.write(header);
			stream.write_padding(16);

			writeFunc();
			stream.write_padding(16);

			size_t end = stream.tellp();
			chunkHeader.chunkSize = static_cast<unsigned int>(end - dataStart);

			stream.seekp(start);
			stream.write(chunkHeader);
			stream.seekp(end);
		}

		template<typename H, typename F>
		void WriteMainChunk(unsigned int magic, H&& header, F&& writeFunc) {
			WriteChunk(magic, header, writeFunc);
			chunkCount++;
		}

		void WriteBinaryFileHeaderChunk() {
			WriteChunk(SWIF, SurfRide::SRS_BINARY_FILE_HEADER_CHUNK_HEADER{ 0, 0, 0, 0, 20120705 }, [&]() {});
		}

		void WriteTextureListChunk(const SurfRide::SRS_TEXTURELIST* textureLists, size_t textureListCount) {
			WriteMainChunk(SWTL, SurfRide::SRS_TEXTURELIST_CHUNK_HEADER{ 16, static_cast<unsigned int>(textureListCount) }, [&]() {
				reflectionSerializer.Serialize(textureLists, textureListCount);
			});
		}

		void WriteProjectChunk(const SurfRide::SRS_PROJECT* project) {
			WriteMainChunk(SWPR, SurfRide::SRS_PROJECT_CHUNK_HEADER{ 16 }, [&]() {
				reflectionSerializer.Serialize(project);
			});
		}

		void WriteAddressResolutionChunk() {
			addressResolutionChunkOffset = stream.tellp();

			WriteChunk(SOF0, SurfRide::SRS_ADDRESS_RESOLUTION_CHUNK_HEADER{ static_cast<unsigned int>(addressLocations.size()), 0 }, [&]() {
				for (unsigned int addressLocation : addressLocations)
					stream.write(addressLocation);
			});
		}

		void WriteEndChunk() {
			WriteChunk(SEND, SurfRide::SRS_END_CHUNK_HEADER{}, [&]() {});
		}
	public:
		SWIFSerializer(csl::fnd::IAllocator* allocator, binary_ostream& stream) : CompatibleObject{ allocator }, stream{ stream } {
			WriteBinaryFileHeaderChunk();

			chunksStart = stream.tellp();
		}

		~SWIFSerializer() {
			size_t chunksEnd = stream.tellp();

			WriteAddressResolutionChunk();
			WriteEndChunk();

			stream.seekp(8);
			stream.write(SurfRide::SRS_BINARY_FILE_HEADER_CHUNK_HEADER{ chunkCount, static_cast<unsigned int>(chunksStart), static_cast<unsigned int>(chunksEnd - chunksStart), static_cast<unsigned int>(addressResolutionChunkOffset), 20120705 });
		}

		void AddProject(const SurfRide::SRS_PROJECT* project) {
			// If adding 2 projects it will write 2 texture list chunks, but always refer to the first due to point disambiguation in the reflection serializer.
			// Ignoring this for now as this never happens in the current codebase.
			WriteTextureListChunk(project->textureLists, project->textureListCount);
			WriteProjectChunk(project);
		}
	};

	void Serialize(const wchar_t* filename, const SurfRide::SRS_PROJECT* project) {
		std::ofstream ostream{ filename, std::ios::binary };
		binary_ostream stream{ ostream };
		SWIFSerializer serializer{ Desktop::instance->GetAllocator(), stream };

		serializer.AddProject(project);
	}
}
