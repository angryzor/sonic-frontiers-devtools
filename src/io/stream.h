#pragma once

namespace devtools::io {
	class binary_istream {
		std::istream& stream;
	public:
		binary_istream(std::istream& stream) : stream{ stream } {}

		template<typename T>
		void read(T& obj) {
			stream.read(reinterpret_cast<char*>(&obj), sizeof(T));
		}
	};

	class binary_ostream {
		std::ostream& stream;
	public:
		binary_ostream(std::ostream& stream) : stream{ stream } {}

		template<typename T>
		void write(const T& obj, size_t count = 1) {
			stream.write(reinterpret_cast<const char*>(&obj), sizeof(T) * count);
		}

		void write_padding(size_t alignment) {
			size_t pos = stream.tellp();
			size_t offset = (pos + alignment - 1) & ~(alignment - 1);
			for (size_t i = 0; i < offset - pos; i++)
				stream.put(0);
		}

		void seekp(size_t loc) {
			stream.seekp(loc);
		}

		size_t tellp() {
			return stream.tellp();
		}
	};
}
