#include "RflDiff.h"
#include <queue>

using namespace hh::fnd;

RflDiffChange::Index::Index(Type type, const char* data) : type{ type }, propertyName{ data } {}
RflDiffChange::Index::Index(Type type, size_t data) : type{ type }, arrayIdx{ data } {}

RflDiffChange::RflDiffChange(csl::fnd::IAllocator* allocator, Type type, const void* value) : type{ type }, path{ allocator }, value{ value } {}
RflDiffChange::RflDiffChange(const RflDiffChange& other) : type{ other.type }, path{ other.path }, value{ other.value } {}
RflDiffChange::RflDiffChange(RflDiffChange&& other) noexcept : type{ other.type }, path{ std::move(other.path) }, value{ other.value } {}

RflDiffChange& RflDiffChange::operator=(RflDiffChange&& other) noexcept
{
	type = other.type;
	path = std::move(other.path);
	value = other.value;
	return *this;
}

RflDiffResult::RflDiffResult(csl::fnd::IAllocator* allocator) : score{}, changes{ allocator } {}
RflDiffResult::RflDiffResult(csl::fnd::IAllocator* allocator, size_t score, RflDiffChange&& change) : score{ score }, changes{ allocator } {
	changes.push_back(std::move(change));
}
RflDiffResult::RflDiffResult(const RflDiffResult& other) : score{ other.score }, changes{ other.changes } {}
RflDiffResult::RflDiffResult(RflDiffResult&& other) noexcept : score{ other.score }, changes{ std::move(other.changes) } {}

RflDiffResult& RflDiffResult::operator=(RflDiffResult&& other) noexcept
{
	score = other.score;
	changes = std::move(other.changes);
	return *this;
}

RflDiffResult& RflDiffResult::operator+=(RflDiffResult&& other) {
	score += other.score;
	for (auto& change : other.changes)
		changes.push_back(std::move(change));
	return *this;
}

RflDiffResult& RflDiffResult::operator+=(const RflDiffChange::Index& index) {
	for (auto& change : changes)
		change.path.push_back(index);
	return *this;
}

struct EditGraphNode {
	size_t x{};
	size_t y{};
};

struct DiffScore {
	RflDiffResult result;
	EditGraphNode node;

	DiffScore(csl::fnd::IAllocator* allocator) : result{ allocator } {}
	DiffScore(const DiffScore& other) : result{ other.result }, node{ other.node } {}
	DiffScore(DiffScore&& other) : result{ std::move(other.result) }, node{ other.node } {}

	DiffScore& operator=(DiffScore&& other) {
		result = std::move(other.result);
		node = other.node;
		return *this;
	}

	bool operator<(const DiffScore& other) const {
		return result.score < other.result.score;
	}
};

template<typename T>
bool Matches(const void* obj1, const void* obj2) {
	return *static_cast<const T*>(obj1) == *static_cast<const T*>(obj2);
}

size_t RflMaxScoreStruct(const void* obj1, const RflClass* rflClass);

size_t RflMaxScoreSingle(const void* obj1, const RflClassMember& member, const RflClassMember::Type type) {
	switch (type) {
	case RflClassMember::Type::STRUCT: return RflMaxScoreStruct(obj1, member.GetClass());
	default: return 1;
	}
}

size_t RflMaxScoreArray(const void* obj1, const RflClassMember& member)
{
	size_t score{ 0 };

	auto* arr1 = static_cast<const csl::ut::MoveArray<void*>*>(obj1);

	for (auto& item : *arr1)
		score += RflMaxScoreSingle(item, member, member.GetSubType());

	return score;
}

size_t RflMaxScoreClassMember(const void* obj1, const RflClassMember& member) {
	switch (member.GetType()) {
	case RflClassMember::Type::ARRAY: return RflMaxScoreArray(obj1, member);
	case RflClassMember::Type::POINTER: return RflMaxScoreSingle(*static_cast<const void* const*>(obj1), member, member.GetSubType());
	case RflClassMember::Type::ENUM: return RflMaxScoreSingle(obj1, member, member.GetSubType());
	case RflClassMember::Type::FLAGS: return RflMaxScoreSingle(obj1, member, member.GetSubType());
	case RflClassMember::Type::SIMPLE_ARRAY:
		assert(!"This RflClass member type (SIMPLE_ARRAY) is not implemented yet because it is unused.");
		return 0;
	default: return RflMaxScoreSingle(obj1, member, member.GetSubType());
	}
}

size_t RflMaxScoreStruct(const void* obj1, const RflClass* rflClass) {
	const RflClass* parent = rflClass->GetParent();

	size_t result{ 0 };

	if (parent != nullptr) {
		result += RflMaxScoreStruct(obj1, parent);
	}

	for (auto& member : rflClass->GetMembers()) {
		size_t constArrSizeOrZero = member.GetArrayLength();
		size_t constArrSize = constArrSizeOrZero == 0 ? 1 : constArrSizeOrZero;
		size_t baseAddr1 = reinterpret_cast<size_t>(obj1) + member.GetOffset();

		for (size_t j = 0; j < constArrSize; j++) {
			auto* obj1Member = reinterpret_cast<void*>(baseAddr1 + j * member.GetSingleSize());
			result += RflMaxScoreClassMember(obj1Member, member);
		}
	}

	return result;
}

RflDiffResult RflDiffStruct(csl::fnd::IAllocator* allocator, const void* obj1, const void* obj2, const RflClass* rflClass);

bool PrimitiveMatches(const void* obj1, const void* obj2, const RflClassMember::Type type) {
	switch (type) {
	case RflClassMember::Type::BOOL: return Matches<bool>(obj1, obj2);
	case RflClassMember::Type::SINT8: return Matches<int8_t>(obj1, obj2);
	case RflClassMember::Type::UINT8: return Matches<uint8_t>(obj1, obj2);
	case RflClassMember::Type::SINT16: return Matches<int16_t>(obj1, obj2);
	case RflClassMember::Type::UINT16: return Matches<uint16_t>(obj1, obj2);
	case RflClassMember::Type::SINT32: return Matches<int32_t>(obj1, obj2);
	case RflClassMember::Type::UINT32: return Matches<uint32_t>(obj1, obj2);
	case RflClassMember::Type::SINT64: return Matches<int64_t>(obj1, obj2);
	case RflClassMember::Type::UINT64: return Matches<uint64_t>(obj1, obj2);
	case RflClassMember::Type::FLOAT: return Matches<float>(obj1, obj2);
	case RflClassMember::Type::VECTOR2: return Matches<csl::math::Vector2>(obj1, obj2);
	case RflClassMember::Type::VECTOR3: return Matches<csl::math::Vector3>(obj1, obj2);
	case RflClassMember::Type::VECTOR4: return Matches<csl::math::Vector4>(obj1, obj2);
	case RflClassMember::Type::QUATERNION: return Matches<csl::math::Quaternion>(obj1, obj2);
	case RflClassMember::Type::MATRIX34: return Matches<csl::math::Matrix34>(obj1, obj2);
	case RflClassMember::Type::MATRIX44: return Matches<csl::math::Matrix44>(obj1, obj2);
	case RflClassMember::Type::CSTRING: return strcmp(static_cast<const char*>(obj1), static_cast<const char*>(obj2)) == 0;
	case RflClassMember::Type::STRING: return strcmp(static_cast<const csl::ut::VariableString*>(obj1)->c_str(), static_cast<const csl::ut::VariableString*>(obj2)->c_str()) == 0;
	case RflClassMember::Type::OBJECT_ID: return Matches<hh::game::ObjectId>(obj1, obj2);
	case RflClassMember::Type::COLOR_BYTE: return Matches<csl::ut::Color8>(obj1, obj2);
	case RflClassMember::Type::COLOR_FLOAT: return Matches<csl::ut::Color<float>>(obj1, obj2);
	case RflClassMember::Type::POSITION: return Matches<csl::math::Vector3>(obj1, obj2);
	default:
		assert(!"rfl editor assertion failed: unknown primitive type");
		return false;
	}
}

RflDiffResult RflDiffSingle(csl::fnd::IAllocator* allocator, const void* obj1, const void* obj2, const RflClassMember& member, const RflClassMember::Type type)
{
	switch (type) {
	case RflClassMember::Type::STRUCT: return RflDiffStruct(allocator, obj1, obj2, member.GetClass());
	default:
		{
			return PrimitiveMatches(obj1, obj2, type) ? RflDiffResult{ allocator } : RflDiffResult{ allocator, 1, { allocator, RflDiffChange::Type::UPDATE, obj2 } };
		}
	}
}

RflDiffResult RflDiffArray(csl::fnd::IAllocator* allocator, const void* obj1, const void* obj2, const RflClassMember& member)
{
	std::priority_queue<DiffScore> pq;
	RflDiffResult result{ allocator };

	auto* arr1 = static_cast<const csl::ut::MoveArray<void*>*>(obj1);
	auto* arr2 = static_cast<const csl::ut::MoveArray<void*>*>(obj2);

	csl::ut::MoveArray<size_t> maxes1;
	csl::ut::MoveArray<size_t> maxes2;

	for (size_t i = 0; i < arr1->size(); i++)
		maxes1.push_back(RflMaxScoreSingle(reinterpret_cast<void*>(reinterpret_cast<size_t>(arr1->begin()) + i * member.GetSubTypeSize()), member, member.GetSubType()));
	for (size_t i = 0; i < arr2->size(); i++)
		maxes2.push_back(RflMaxScoreSingle(reinterpret_cast<void*>(reinterpret_cast<size_t>(arr2->begin()) + i * member.GetSubTypeSize()), member, member.GetSubType()));

	while (!pq.empty()) {
		DiffScore ds{ std::move(pq.top()) };
		pq.pop();

		if (ds.node.x == arr1->size() && ds.node.y == arr2->size())
			return ds.result;

		if (ds.node.x + 1 < arr1->size()) {
			DiffScore deleteScore{ ds };

			RflDiffResult deleteResult{ allocator, maxes1[ds.node.x], { allocator, RflDiffChange::Type::REMOVE, nullptr } };
			deleteResult += RflDiffChange::Index{ RflDiffChange::Index::Type::INDEX, ds.node.x };

			deleteScore.result += std::move(deleteResult);
			deleteScore.node.x++;

			pq.push(std::move(deleteScore));
		}

		if (ds.node.y + 1 < arr2->size()) {
			DiffScore addScore{ ds };

			RflDiffResult addResult{ allocator, maxes2[ds.node.y], { allocator, RflDiffChange::Type::ADD, reinterpret_cast<void*>(reinterpret_cast<size_t>(arr2->begin()) + ds.node.y * member.GetSubTypeSize()) } };
			addResult += RflDiffChange::Index{ RflDiffChange::Index::Type::INDEX, ds.node.x };

			addScore.result += std::move(addResult);
			addScore.node.y++;

			pq.push(std::move(addScore));
		}

		DiffScore updateScore{ ds };

		RflDiffResult updateResult{
			RflDiffSingle(
				allocator,
				reinterpret_cast<void*>(reinterpret_cast<size_t>(arr1->begin()) + ds.node.x * member.GetSubTypeSize()),
				reinterpret_cast<void*>(reinterpret_cast<size_t>(arr2->begin()) + ds.node.y * member.GetSubTypeSize()),
				member,
				member.GetSubType()
			)
		};
		updateResult += RflDiffChange::Index{ RflDiffChange::Index::Type::INDEX, ds.node.x };

		updateScore.result += std::move(updateResult);
		updateScore.node.x++;
		updateScore.node.y++;

		pq.push(std::move(updateScore));
	}

	return result;
}

RflDiffResult RflDiffClassMember(csl::fnd::IAllocator* allocator, const void* obj1, const void* obj2, const RflClassMember& member) {
	switch (member->GetType()) {
	case RflClassMember::Type::ARRAY: return RflDiffArray(allocator, obj1, obj2, member);
	case RflClassMember::Type::POINTER: return RflDiffSingle(allocator, *static_cast<const void* const*>(obj1), *static_cast<const void* const*>(obj2), member, member.GetSubType());
	case RflClassMember::Type::ENUM: return RflDiffSingle(allocator, obj1, obj2, member, member.GetSubType());
	case RflClassMember::Type::FLAGS: return RflDiffSingle(allocator, obj1, obj2, member, member.GetSubType());
	case RflClassMember::Type::SIMPLE_ARRAY:
		assert(!"This RflClass member type (SIMPLE_ARRAY) is not implemented yet because it is unused.");
		return RflDiffResult{ allocator };
	default: return RflDiffSingle(allocator, obj1, obj2, member, member.GetType());
	}
}

RflDiffResult RflDiffStruct(csl::fnd::IAllocator* allocator, const void* obj1, const void* obj2, const RflClass* rflClass) {
	const RflClass* parent = rflClass->GetParent();

	RflDiffResult result{ allocator };

	if (parent != nullptr) {
		result += RflDiffStruct(allocator, obj1, obj2, parent);
	}

	for (auto& member : rflClass->GetMembers()) {
		size_t constArrSizeOrZero = member.GetArrayLength();
		size_t baseAddr1 = reinterpret_cast<size_t>(obj1) + member.GetOffset();
		size_t baseAddr2 = reinterpret_cast<size_t>(obj2) + member.GetOffset();

		if (constArrSizeOrZero == 0) {
			auto* obj1Member = reinterpret_cast<void*>(baseAddr1);
			auto* obj2Member = reinterpret_cast<void*>(baseAddr2);

			RflDiffResult memberResult{ RflDiffClassMember(allocator, obj1Member, obj2Member, member) };
			memberResult += RflDiffChange::Index{ RflDiffChange::Index::Type::PROPERTY, member.GetName() };
			result += std::move(memberResult);
		}
		else {
			for (size_t j = 0; j < constArrSizeOrZero; j++) {
				auto* obj1Member = reinterpret_cast<void*>(baseAddr1 + j * member.GetSingleSize());
				auto* obj2Member = reinterpret_cast<void*>(baseAddr2 + j * member.GetSingleSize());

				RflDiffResult memberResult{ RflDiffClassMember(allocator, obj1Member, obj2Member, member) };
				memberResult += RflDiffChange::Index{ RflDiffChange::Index::Type::INDEX, j };
				memberResult += RflDiffChange::Index{ RflDiffChange::Index::Type::PROPERTY, member.GetName() };
				result += std::move(memberResult);
			}
		}
	}

	return result;
}
