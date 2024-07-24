#pragma once

struct RflDiffChange {
	enum class Type {
		UPDATE,
		ADD,
		REMOVE,
	};

	struct Index {
		enum class Type {
			PROPERTY,
			INDEX,
		};
		Type type;
		union {
			const char* propertyName;
			size_t arrayIdx;
		};
		Index(Type type, const char* data);
		Index(Type type, size_t data);
	};

	Type type;
	csl::ut::MoveArray<Index> path;
	const void* value;

	RflDiffChange(csl::fnd::IAllocator* allocator, Type type, const void* value);
	RflDiffChange(const RflDiffChange& other);
	RflDiffChange(RflDiffChange&& other) noexcept;

	RflDiffChange& operator=(RflDiffChange&& other) noexcept;
};

struct RflDiffResult {
	size_t score;
	csl::ut::MoveArray<RflDiffChange> changes;

	RflDiffResult(csl::fnd::IAllocator* allocator);
	RflDiffResult(csl::fnd::IAllocator* allocator, size_t score, RflDiffChange&& change);
	RflDiffResult(const RflDiffResult& other);
	RflDiffResult(RflDiffResult&& other) noexcept;

	RflDiffResult& operator=(RflDiffResult&& other) noexcept;

	RflDiffResult& operator+=(RflDiffResult&& other);
	RflDiffResult& operator+=(const RflDiffChange::Index& index);
};

RflDiffResult RflDiffStruct(csl::fnd::IAllocator* allocator, const void* obj1, const void* obj2, const hh::fnd::RflClass* rflClass);
