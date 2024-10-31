#include "HSONTemplateGeneration.h"
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <reflection/RflRanges.h>
#include <fstream>
#include <codecvt>
#include <cmath>
#include <ranges>

namespace io::hson::templates {
	using BoolType = rfl::Literal<"bool">;
	using Int8Type = rfl::Literal<"int8">;
	using Uint8Type = rfl::Literal<"uint8">;
	using Int16Type = rfl::Literal<"int16">;
	using Uint16Type = rfl::Literal<"uint16">;
	using Int32Type = rfl::Literal<"int32">;
	using Uint32Type = rfl::Literal<"uint32">;
	using Int64Type = rfl::Literal<"int64">;
	using Uint64Type = rfl::Literal<"uint64">;
	using Float32Type = rfl::Literal<"float32">;
	using Vector2Type = rfl::Literal<"vector2">;
	using Vector3Type = rfl::Literal<"vector3">;
	using Vector4Type = rfl::Literal<"vector4">;
	using QuaternionType = rfl::Literal<"quaternion">;
	using ObjectIdType = rfl::Literal<"object_reference">;
	using StringType = rfl::Literal<"string">;
	using Float64Type = rfl::Literal<"float64">;
	using CharType = rfl::Literal<"char">;
	using ArrayType = rfl::Literal<"array">;

	template<typename T> struct hson_type {};
	template<> struct hson_type<bool> { static constexpr const char* type = "bool"; static constexpr std::optional<unsigned int> alignment = std::nullopt; using lit = BoolType; };
	template<> struct hson_type<int8_t> { static constexpr const char* type = "int8"; static constexpr std::optional<unsigned int> alignment = std::nullopt; using lit = Int8Type; };
	template<> struct hson_type<uint8_t> { static constexpr const char* type = "uint8"; static constexpr std::optional<unsigned int> alignment = std::nullopt; using lit = Uint8Type; };
	template<> struct hson_type<int16_t> { static constexpr const char* type = "int16"; static constexpr std::optional<unsigned int> alignment = std::nullopt; using lit = Int16Type; };
	template<> struct hson_type<uint16_t> { static constexpr const char* type = "uint16"; static constexpr std::optional<unsigned int> alignment = std::nullopt; using lit = Uint16Type; };
	template<> struct hson_type<int32_t> { static constexpr const char* type = "int32"; static constexpr std::optional<unsigned int> alignment = std::nullopt; using lit = Int32Type; };
	template<> struct hson_type<uint32_t> { static constexpr const char* type = "uint32"; static constexpr std::optional<unsigned int> alignment = std::nullopt; using lit = Uint32Type; };
	template<> struct hson_type<int64_t> { static constexpr const char* type = "int64"; static constexpr std::optional<unsigned int> alignment = std::nullopt; using lit = Int64Type; };
	template<> struct hson_type<uint64_t> { static constexpr const char* type = "uint64"; static constexpr std::optional<unsigned int> alignment = std::nullopt; using lit = Uint64Type; };
	template<> struct hson_type<float> { static constexpr const char* type = "float32"; static constexpr std::optional<unsigned int> alignment = std::nullopt; using lit = Float32Type; };
	template<> struct hson_type<csl::math::Vector2> { static constexpr const char* type = "vector2"; static constexpr std::optional<unsigned int> alignment = std::nullopt; using lit = Vector2Type; };
	template<> struct hson_type<csl::math::Vector3> { static constexpr const char* type = "vector3"; static constexpr std::optional<unsigned int> alignment = std::make_optional(16); using lit = Vector3Type; };
	template<> struct hson_type<csl::math::Vector4> { static constexpr const char* type = "vector4"; static constexpr std::optional<unsigned int> alignment = std::nullopt; using lit = Vector4Type; };
	template<> struct hson_type<csl::math::Quaternion> { static constexpr const char* type = "quaternion"; static constexpr std::optional<unsigned int> alignment = std::nullopt; using lit = QuaternionType; };
	template<> struct hson_type<hh::game::ObjectId> { static constexpr const char* type = "object_reference"; static constexpr std::optional<unsigned int> alignment = std::nullopt; using lit = ObjectIdType; };
	template<> struct hson_type<csl::ut::VariableString> { static constexpr const char* type = "string"; static constexpr std::optional<unsigned int> alignment = std::nullopt; using lit = StringType; };
	template<> struct hson_type<double> { static constexpr const char* type = "float64"; static constexpr std::optional<unsigned int> alignment = std::nullopt; using lit = Float64Type; };
	template<> struct hson_type<char> { static constexpr const char* type = "char"; static constexpr std::optional<unsigned int> alignment = std::nullopt; using lit = CharType; };


	template<typename T>
	struct EnumValueDefT {
		T value{};
		std::optional<rfl::Object<std::string>> descriptions{};
	};

	template<typename T>
	struct EnumDefT {
		std::string type{};
		rfl::Object<EnumValueDefT<T>> values{};
	};

	using EnumDef = rfl::Variant<
		EnumDefT<int8_t>,
		EnumDefT<int16_t>,
		EnumDefT<int32_t>,
		EnumDefT<int64_t>,
		EnumDefT<uint8_t>,
		EnumDefT<uint16_t>,
		EnumDefT<uint32_t>,
		EnumDefT<uint64_t>
	>;

	template<typename T> struct rfl_range_rep {};
	template<> struct rfl_range_rep<RangeSint32> { using type = int32_t; };
	template<> struct rfl_range_rep<RangeUint32> { using type = uint32_t; };
	template<> struct rfl_range_rep<RangeSint64> { using type = int64_t; };
	template<> struct rfl_range_rep<RangeUint64> { using type = uint64_t; };
	template<> struct rfl_range_rep<RangeFloat> { using type = float; };
	template<> struct rfl_range_rep<RangeVector2> { using type = std::array<float, 2>; };
	template<> struct rfl_range_rep<RangeVector3> { using type = std::array<float, 3>; };
	template<> struct rfl_range_rep<RangeVector4> { using type = std::array<float, 4>; };

	template<typename T>
	using rfl_range_rep_t = typename rfl_range_rep<T>::type;

	using Ranges = rfl::Variant<
		rfl_range_rep_t<rfl_range_type_t<int8_t>>,
		rfl_range_rep_t<rfl_range_type_t<uint8_t>>,
		rfl_range_rep_t<rfl_range_type_t<int16_t>>,
		rfl_range_rep_t<rfl_range_type_t<uint16_t>>,
		rfl_range_rep_t<rfl_range_type_t<int32_t>>,
		rfl_range_rep_t<rfl_range_type_t<uint32_t>>,
		rfl_range_rep_t<rfl_range_type_t<int64_t>>,
		rfl_range_rep_t<rfl_range_type_t<uint64_t>>,
		rfl_range_rep_t<rfl_range_type_t<float>>,
		rfl_range_rep_t<rfl_range_type_t<csl::math::Vector2>>,
		rfl_range_rep_t<rfl_range_type_t<csl::math::Vector3>>,
		rfl_range_rep_t<rfl_range_type_t<csl::math::Vector4>>
	>;

	struct RangeProps {
		std::optional<Ranges> min_range{};
		std::optional<Ranges> max_range{};
		std::optional<Ranges> step{};
	};

	struct MemberDef {
		std::string name{};
		std::string type{};
		std::optional<std::string> subtype{};
		std::optional<unsigned int> array_size{};
		std::optional<unsigned int> alignment{};
		std::optional<rfl::Object<std::string>> descriptions{};
		rfl::Flatten<RangeProps> ranges;
	};

	struct RflClassDef {
		std::optional<std::string> parent{};
		std::vector<MemberDef> fields{};
	};

	struct ObjectDef {
		rfl::Rename<"struct", std::string> rflClassName{};
		std::string category{};
	};

	struct Template {
		unsigned int version{};
		std::string format{};
		std::map<std::string, EnumDef> enums{};
		std::map<std::string, RflClassDef> structs{};
		std::map<std::string, ObjectDef> objects{};
	};

	float RoundFloat(float value) {
		return roundf(value * 1000.0f) / 1000.0f;
	}

	template<typename T> std::optional<rfl_range_rep_t<T>> TidyRange(const rfl_range_representation_t<T>& range) { return { range }; }
	template<> std::optional<rfl_range_rep_t<RangeFloat>> TidyRange<RangeFloat>(const rfl_range_representation_t<RangeFloat>& range) {
		return range > std::numeric_limits<float>::lowest() / 2
			&& range < std::numeric_limits<float>::max() / 2
			? std::make_optional<rfl_range_rep_t<RangeFloat>>(RoundFloat(range))
			: std::nullopt;
	}
	template<> std::optional<rfl_range_rep_t<RangeVector2>> TidyRange<RangeVector2>(const rfl_range_representation_t<RangeVector2>& range) {
		return range.x() > std::numeric_limits<float>::lowest() / 2 && range.y() > std::numeric_limits<float>::lowest() / 2
			&& range.x() < std::numeric_limits<float>::max() / 2 && range.y() < std::numeric_limits<float>::max() / 2
			? std::make_optional<rfl_range_rep_t<RangeVector2>>({ RoundFloat(range.x()), RoundFloat(range.y()) })
			: std::nullopt;
	}
	template<> std::optional<rfl_range_rep_t<RangeVector3>> TidyRange<RangeVector3>(const rfl_range_representation_t<RangeVector3>& range) {
		return range.x() > std::numeric_limits<float>::lowest() / 2 && range.y() > std::numeric_limits<float>::lowest() / 2 && range.z() > std::numeric_limits<float>::lowest() / 2
			&& range.x() < std::numeric_limits<float>::max() / 2 && range.y() < std::numeric_limits<float>::max() / 2 && range.z() < std::numeric_limits<float>::max() / 2
			? std::make_optional<rfl_range_rep_t<RangeVector3>>({ RoundFloat(range.x()), RoundFloat(range.y()), RoundFloat(range.z()) })
			: std::nullopt;
	}
	template<> std::optional<rfl_range_rep_t<RangeVector4>> TidyRange<RangeVector4>(const rfl_range_representation_t<RangeVector4>& range) {
		return range.x() > std::numeric_limits<float>::lowest() / 2 && range.y() > std::numeric_limits<float>::lowest() / 2 && range.z() > std::numeric_limits<float>::lowest() / 2 && range.w() > std::numeric_limits<float>::lowest() / 2
			&& range.x() < std::numeric_limits<float>::max() / 2 && range.y() < std::numeric_limits<float>::max() / 2 && range.z() < std::numeric_limits<float>::max() / 2 && range.w() < std::numeric_limits<float>::max() / 2
			? std::make_optional<rfl_range_rep_t<RangeVector4>>({ RoundFloat(range.x()), RoundFloat(range.y()), RoundFloat(range.z()), RoundFloat(range.w()) })
			: std::nullopt;
	}

	template<typename T>
	EnumValueDefT<T> GenerateEnumValueDefT(const hh::fnd::RflClassEnumMember& value) {
		rfl::Object<std::string> descs{};
		descs["ja"] = value.GetJapaneseName();

		EnumValueDefT<T> def{};
		def.value = static_cast<T>(value.GetIndex());
		def.descriptions = std::move(descs);
		return def;
	}

	template<typename T>
	EnumDef GenerateEnumDefT(const hh::fnd::RflClassEnum* enumClass) {
		EnumDefT<T> def{};
		def.type = hson_type<T>::type;

		auto* values = enumClass->GetValues();
		for (unsigned int i = 0; i < enumClass->GetValueCount(); i++) {
			auto& value = values[i];
			def.values[reinterpret_cast<const char*>(value.GetEnglishName())] = GenerateEnumValueDefT<T>(value);
		}

		return def;
	}

	EnumDef GenerateEnum(const hh::fnd::RflClassEnum* enumClass, hh::fnd::RflClassMember::Type type) {
		switch (type) {
		case hh::fnd::RflClassMember::TYPE_SINT8:
			return GenerateEnumDefT<int8_t>(enumClass);
		case hh::fnd::RflClassMember::TYPE_UINT8:
			return GenerateEnumDefT<uint8_t>(enumClass);
		case hh::fnd::RflClassMember::TYPE_SINT16:
			return GenerateEnumDefT<int16_t>(enumClass);
		case hh::fnd::RflClassMember::TYPE_UINT16:
			return GenerateEnumDefT<uint16_t>(enumClass);
		case hh::fnd::RflClassMember::TYPE_SINT32:
			return GenerateEnumDefT<int32_t>(enumClass);
		case hh::fnd::RflClassMember::TYPE_UINT32:
			return GenerateEnumDefT<uint32_t>(enumClass);
		case hh::fnd::RflClassMember::TYPE_SINT64:
			return GenerateEnumDefT<int64_t>(enumClass);
		case hh::fnd::RflClassMember::TYPE_UINT64:
			return GenerateEnumDefT<uint64_t>(enumClass);
		default:
			assert(false);
			return {};
		}
	}

	template<typename T>
	MemberDef GenerateMemberT(const hh::fnd::RflClassMember* member, const char* typeName) {
		auto* caption = member->GetAttribute("Caption");
		std::optional<rfl::Object<std::string>> captionOpt{ std::nullopt };

		if (caption) {
			rfl::Object<std::string> descs{};
			descs["ja"] = *static_cast<const char* const*>(caption->GetData());
			captionOpt = std::move(descs);
		}

		return {
			member->GetName(),
			typeName ? typeName : hson_type<T>::type,
			std::nullopt,
			std::nullopt,
			hson_type<T>::alignment,
			std::move(captionOpt),
			RangeProps{},
		};
	}

	template<typename T>
	MemberDef GenerateRangeMemberT(const hh::fnd::RflClassMember* member, const char* typeName, const char* rangeAttrName) {
		auto* caption = member->GetAttribute("Caption");
		std::optional<rfl::Object<std::string>> captionOpt{ std::nullopt };

		if (caption) {
			rfl::Object<std::string> descs{};
			descs["ja"] = *static_cast<const char* const*>(caption->GetData());
			captionOpt = std::move(descs);
		}

		auto* rangeAttr = member->GetAttribute(rangeAttrName);
		if (rangeAttr == nullptr) {
			return {
				member->GetName(),
				typeName ? typeName : hson_type<T>::type,
				std::nullopt,
				std::nullopt,
				hson_type<T>::alignment,
				std::move(captionOpt),
				RangeProps{},
			};
		}
		else {
			auto* ranges = static_cast<const rfl_range_representation_t<rfl_range_type_t<T>>*>(rangeAttr->GetData());

			return {
				member->GetName(),
				typeName ? typeName : hson_type<T>::type,
				std::nullopt,
				std::nullopt,
				hson_type<T>::alignment,
				std::move(captionOpt),
				RangeProps{
					std::move(TidyRange<rfl_range_type_t<T>>(ranges[0])),
					std::move(TidyRange<rfl_range_type_t<T>>(ranges[1])),
					std::move(TidyRange<rfl_range_type_t<T>>(ranges[2])),
				},
			};
		}
	}

	template<typename T>
	MemberDef GenerateArrayMemberT(const hh::fnd::RflClassMember* member, const char* typeName) {
		MemberDef def = GenerateMemberT<T>(member, typeName);
		return { def.name, "array", def.type, std::nullopt, def.alignment, def.descriptions, def.ranges };
	}

	template<typename T>
	MemberDef GenerateRangeArrayMemberT(const hh::fnd::RflClassMember* member, const char* typeName, const char* rangeAttrName) {
		MemberDef def = GenerateRangeMemberT<T>(member, typeName, rangeAttrName);
		return { def.name, "array", def.type, std::nullopt, def.alignment, def.descriptions, def.ranges };
	}

	template<typename T>
	MemberDef GenerateCArrayMemberT(const hh::fnd::RflClassMember* member, const char* typeName, unsigned int size) {
		MemberDef def = GenerateMemberT<T>(member, typeName);
		return { def.name, "array", def.type, size, def.alignment, def.descriptions, def.ranges };
	}

	template<typename T>
	MemberDef GenerateRangeCArrayMemberT(const hh::fnd::RflClassMember* member, const char* typeName, const char* rangeAttrName, unsigned int size) {
		MemberDef def = GenerateRangeMemberT<T>(member, typeName, rangeAttrName);
		return { def.name, "array", def.type, size, def.alignment, def.descriptions, def.ranges };
	}

	RflClassDef GenerateRflClass(Template& templateDef, const std::string& curNamespace, const hh::fnd::RflClass* rflClass);
	MemberDef GeneratePrimitiveMember(const hh::fnd::RflClassMember* member, hh::fnd::RflClassMember::Type type, const char* typeName = nullptr) {
		switch (type) {
		case hh::fnd::RflClassMember::TYPE_BOOL:
			return GenerateMemberT<bool>(member, typeName);
		case hh::fnd::RflClassMember::TYPE_SINT8:
			return GenerateRangeMemberT<int8_t>(member, typeName, "RangeSint32");
		case hh::fnd::RflClassMember::TYPE_UINT8:
			return GenerateRangeMemberT<uint8_t>(member, typeName, "RangeUint32");
		case hh::fnd::RflClassMember::TYPE_SINT16:
			return GenerateRangeMemberT<int16_t>(member, typeName, "RangeSint32");
		case hh::fnd::RflClassMember::TYPE_UINT16:
			return GenerateRangeMemberT<uint16_t>(member, typeName, "RangeUint32");
		case hh::fnd::RflClassMember::TYPE_SINT32:
			return GenerateRangeMemberT<int32_t>(member, typeName, "RangeSint32");
		case hh::fnd::RflClassMember::TYPE_UINT32:
			return GenerateRangeMemberT<uint32_t>(member, typeName, "RangeUint32");
		case hh::fnd::RflClassMember::TYPE_SINT64:
			return GenerateRangeMemberT<int64_t>(member, typeName, "RangeSint64");
		case hh::fnd::RflClassMember::TYPE_UINT64:
			return GenerateRangeMemberT<uint64_t>(member, typeName, "RangeUint64");
		case hh::fnd::RflClassMember::TYPE_FLOAT:
			return GenerateRangeMemberT<float>(member, typeName, "RangeFloat");
		case hh::fnd::RflClassMember::TYPE_VECTOR2:
			return GenerateRangeMemberT<csl::math::Vector2>(member, typeName, "RangeVector2");
		case hh::fnd::RflClassMember::TYPE_VECTOR3:
			return GenerateRangeMemberT<csl::math::Vector3>(member, typeName, "RangeVector3");
		case hh::fnd::RflClassMember::TYPE_VECTOR4:
			return GenerateRangeMemberT<csl::math::Vector4>(member, typeName, "RangeVector4");
		case hh::fnd::RflClassMember::TYPE_QUATERNION:
			return GenerateMemberT<csl::math::Quaternion>(member, typeName);
		case hh::fnd::RflClassMember::TYPE_OBJECT_ID:
			return GenerateMemberT<hh::game::ObjectId>(member, typeName);
		case hh::fnd::RflClassMember::TYPE_STRING:
			return GenerateMemberT<csl::ut::VariableString>(member, typeName);
		default:
			assert(false);
			return { "", "", std::nullopt, std::nullopt, std::nullopt, std::nullopt, RangeProps{ } };
		}
	}

	MemberDef GenerateArrayPrimitiveMember(const hh::fnd::RflClassMember* member, hh::fnd::RflClassMember::Type type, const char* typeName = nullptr) {
		switch (type) {
		case hh::fnd::RflClassMember::TYPE_BOOL:
			return GenerateArrayMemberT<bool>(member, typeName);
		case hh::fnd::RflClassMember::TYPE_SINT8:
			return GenerateRangeArrayMemberT<int8_t>(member, typeName, "RangeSint32");
		case hh::fnd::RflClassMember::TYPE_UINT8:
			return GenerateRangeArrayMemberT<uint8_t>(member, typeName, "RangeUint32");
		case hh::fnd::RflClassMember::TYPE_SINT16:
			return GenerateRangeArrayMemberT<int16_t>(member, typeName, "RangeSint32");
		case hh::fnd::RflClassMember::TYPE_UINT16:
			return GenerateRangeArrayMemberT<uint16_t>(member, typeName, "RangeUint32");
		case hh::fnd::RflClassMember::TYPE_SINT32:
			return GenerateRangeArrayMemberT<int32_t>(member, typeName, "RangeSint32");
		case hh::fnd::RflClassMember::TYPE_UINT32:
			return GenerateRangeArrayMemberT<uint32_t>(member, typeName, "RangeUint32");
		case hh::fnd::RflClassMember::TYPE_SINT64:
			return GenerateRangeArrayMemberT<int64_t>(member, typeName, "RangeSint64");
		case hh::fnd::RflClassMember::TYPE_UINT64:
			return GenerateRangeArrayMemberT<uint64_t>(member, typeName, "RangeUint64");
		case hh::fnd::RflClassMember::TYPE_FLOAT:
			return GenerateRangeArrayMemberT<float>(member, typeName, "RangeFloat");
		case hh::fnd::RflClassMember::TYPE_VECTOR2:
			return GenerateRangeArrayMemberT<csl::math::Vector2>(member, typeName, "RangeVector2");
		case hh::fnd::RflClassMember::TYPE_VECTOR3:
			return GenerateRangeArrayMemberT<csl::math::Vector3>(member, typeName, "RangeVector2");
		case hh::fnd::RflClassMember::TYPE_VECTOR4:
			return GenerateRangeArrayMemberT<csl::math::Vector4>(member, typeName, "RangeVector2");
		case hh::fnd::RflClassMember::TYPE_QUATERNION:
			return GenerateArrayMemberT<csl::math::Quaternion>(member, typeName);
		case hh::fnd::RflClassMember::TYPE_OBJECT_ID:
			return GenerateArrayMemberT<hh::game::ObjectId>(member, typeName);
		case hh::fnd::RflClassMember::TYPE_STRING:
			return GenerateArrayMemberT<csl::ut::VariableString>(member, typeName);
		default:
			assert(false);
			return { "", "", std::nullopt, std::nullopt, std::nullopt, std::nullopt, RangeProps{ } };
		}
	}

	MemberDef GenerateCArrayPrimitiveMember(const hh::fnd::RflClassMember* member, hh::fnd::RflClassMember::Type type, unsigned int size, const char* typeName = nullptr) {
		switch (type) {
		case hh::fnd::RflClassMember::TYPE_BOOL:
			return GenerateCArrayMemberT<bool>(member, typeName, size);
		case hh::fnd::RflClassMember::TYPE_SINT8:
			return GenerateRangeCArrayMemberT<int8_t>(member, typeName, "RangeSint32", size);
		case hh::fnd::RflClassMember::TYPE_UINT8:
			return GenerateRangeCArrayMemberT<uint8_t>(member, typeName, "RangeUint32", size);
		case hh::fnd::RflClassMember::TYPE_SINT16:
			return GenerateRangeCArrayMemberT<int16_t>(member, typeName, "RangeSint32", size);
		case hh::fnd::RflClassMember::TYPE_UINT16:
			return GenerateRangeCArrayMemberT<uint16_t>(member, typeName, "RangeUint32", size);
		case hh::fnd::RflClassMember::TYPE_SINT32:
			return GenerateRangeCArrayMemberT<int32_t>(member, typeName, "RangeSint32", size);
		case hh::fnd::RflClassMember::TYPE_UINT32:
			return GenerateRangeCArrayMemberT<uint32_t>(member, typeName, "RangeUint32", size);
		case hh::fnd::RflClassMember::TYPE_SINT64:
			return GenerateRangeCArrayMemberT<int64_t>(member, typeName, "RangeSint64", size);
		case hh::fnd::RflClassMember::TYPE_UINT64:
			return GenerateRangeCArrayMemberT<uint64_t>(member, typeName, "RangeUint64", size);
		case hh::fnd::RflClassMember::TYPE_FLOAT:
			return GenerateRangeCArrayMemberT<float>(member, typeName, "RangeFloat", size);
		case hh::fnd::RflClassMember::TYPE_VECTOR2:
			return GenerateRangeCArrayMemberT<csl::math::Vector2>(member, typeName, "RangeVector2", size);
		case hh::fnd::RflClassMember::TYPE_VECTOR3:
			return GenerateRangeCArrayMemberT<csl::math::Vector3>(member, typeName, "RangeVector2", size);
		case hh::fnd::RflClassMember::TYPE_VECTOR4:
			return GenerateRangeCArrayMemberT<csl::math::Vector4>(member, typeName, "RangeVector2", size);
		case hh::fnd::RflClassMember::TYPE_QUATERNION:
			return GenerateCArrayMemberT<csl::math::Quaternion>(member, typeName, size);
		case hh::fnd::RflClassMember::TYPE_OBJECT_ID:
			return GenerateCArrayMemberT<hh::game::ObjectId>(member, typeName, size);
		case hh::fnd::RflClassMember::TYPE_STRING:
			return GenerateCArrayMemberT<csl::ut::VariableString>(member, typeName, size);
		default:
			assert(false);
			return { "", "", std::nullopt, std::nullopt, std::nullopt, std::nullopt, RangeProps{ } };
		}
	}

	MemberDef GenerateSingleMember(Template& templateDef, const std::string& curNamespace, const hh::fnd::RflClassMember* member) {
		switch (member->GetType()) {
		case hh::fnd::RflClassMember::TYPE_STRUCT: {
			auto* rflClass = member->GetStructClass();

			templateDef.structs[rflClass->GetName()] = GenerateRflClass(templateDef, curNamespace, member->GetStructClass());
			return GenerateMemberT<bool>(member, rflClass->GetName());
		}
		case hh::fnd::RflClassMember::TYPE_ENUM: {
			auto* enumClass = member->GetEnumClass();
			auto namespacedName = curNamespace + enumClass->GetName();

			templateDef.enums[namespacedName] = GenerateEnum(enumClass, member->GetSubType());
			return GeneratePrimitiveMember(member, member->GetSubType(), namespacedName.c_str());
		}
		default:
			return GeneratePrimitiveMember(member, member->GetType());
		}
	}

	MemberDef GenerateArrayMember(Template& templateDef, const std::string& curNamespace, const hh::fnd::RflClassMember* member) {
		switch (member->GetSubType()) {
		case hh::fnd::RflClassMember::TYPE_STRUCT: {
			auto* rflClass = member->GetStructClass();

			templateDef.structs[rflClass->GetName()] = GenerateRflClass(templateDef, curNamespace, member->GetStructClass());
			return GenerateArrayMemberT<bool>(member, rflClass->GetName());
		}
		default:
			return GenerateArrayPrimitiveMember(member, member->GetSubType());
		}
	}

	MemberDef GenerateCArrayMember(Template& templateDef, const std::string& curNamespace, const hh::fnd::RflClassMember* member) {
		switch (member->GetType()) {
		case hh::fnd::RflClassMember::TYPE_STRUCT: {
			auto* rflClass = member->GetStructClass();

			templateDef.structs[rflClass->GetName()] = GenerateRflClass(templateDef, curNamespace, member->GetStructClass());
			return GenerateCArrayMemberT<bool>(member, rflClass->GetName(), member->GetCstyleArraySize());
		}
		default:
			return GenerateCArrayPrimitiveMember(member, member->GetType(), member->GetCstyleArraySize());
	}
	}

	MemberDef GenerateMember(Template& templateDef, const std::string& curNamespace, const hh::fnd::RflClassMember* member) {
		if (member->GetCstyleArraySize() > 0)
			return GenerateCArrayMember(templateDef, curNamespace, member);

		switch (member->GetType()) {
#ifdef DEVTOOLS_TARGET_SDK_wars
		case hh::fnd::RflClassMember::TYPE_OLD_ARRAY:
#else
		case hh::fnd::RflClassMember::TYPE_ARRAY:
#endif
			return GenerateArrayMember(templateDef, curNamespace, member);
		default:
			return GenerateSingleMember(templateDef, curNamespace, member);
		}
	}

	bool HasMember(const std::string& memberName, const hh::fnd::RflClass* rflClass) {
		for (size_t i = 0; i < rflClass->m_pMembers.count; i++)
			if (memberName == rflClass->m_pMembers.items[i].m_pName)
				return true;

		if (rflClass->m_pParent)
			return HasMember(memberName, rflClass->m_pParent);

		return false;
	}

	RflClassDef GenerateRflClass(Template& templateDef, const std::string& curNamespace, const hh::fnd::RflClass* rflClass) {
		RflClassDef rflClassDef{};

		auto* parent = rflClass->m_pParent;

		if (parent) {
			templateDef.structs[parent->GetName()] = GenerateRflClass(templateDef, curNamespace, parent);
			rflClassDef.parent = parent->GetName();
		}

		for (size_t i = 0; i < rflClass->m_pMembers.count; i++) {
			auto field = GenerateMember(templateDef, rflClass->GetName() + std::string{ "::" }, &rflClass->m_pMembers.items[i]);

			if (parent && HasMember(field.name, parent))
				field.name = rflClass->GetName() + std::string{ "::" } + field.name;

			rflClassDef.fields.push_back(std::move(field));
		}

		return rflClassDef;
	}

	void GenerateTemplate(const std::string& filename) {
		Template templateDef{};
		templateDef.version = 1;
#ifdef DEVTOOLS_TARGET_SDK_wars
		templateDef.format = "gedit_v2";
#else
		templateDef.format = "gedit_v3";
#endif

		for (auto& objClass : hh::game::GameObjectSystem::GetInstance()->gameObjectRegistry->GetGameObjectClasses()) {
			auto* rflClass = objClass->spawnerDataRflClass;

			if (rflClass == nullptr)
				continue;

			templateDef.structs[rflClass->GetName()] = GenerateRflClass(templateDef, "", rflClass);
			templateDef.objects[objClass->name] = { rflClass->GetName(), static_cast<const char*>(objClass->GetAttributeValue("category")) };
		}

		std::ofstream ofs{ filename, std::ios::trunc };
		rfl::json::write(templateDef, ofs, YYJSON_WRITE_PRETTY_TWO_SPACES);
	}
}
