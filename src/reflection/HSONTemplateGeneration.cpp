#include "HSONTemplateGeneration.h"
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <reflection/RflRanges.h>
#include <fstream>
#include <codecvt>
#include <cmath>


namespace io::hson::templates {

	class Flags {
		uint32_t flag;
	};

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
	using Matrix34Type = rfl::Literal<"matrix34">;
	using Matrix44Type = rfl::Literal<"matrix44">;
	using ObjectIdType = rfl::Literal<"object_reference">;
	using ColorByteType = rfl::Literal<"color8">;
	using ColorFloatType = rfl::Literal<"colorf">;
	using StringType = rfl::Literal<"string">;
	using FlagsType = rfl::Literal<"flags">;
	using Float64Type = rfl::Literal<"float64">;
	using CharType = rfl::Literal<"char">;
	using ArrayType = rfl::Literal<"array">;

	template<typename T> struct hson_type {};
	template<> struct hson_type<bool> { static constexpr const char* type = "bool"; using lit = BoolType; };
	template<> struct hson_type<int8_t> { static constexpr const char* type = "int8"; using lit = Int8Type; };
	template<> struct hson_type<uint8_t> { static constexpr const char* type = "uint8"; using lit = Uint8Type; };
	template<> struct hson_type<int16_t> { static constexpr const char* type = "int16"; using lit = Int16Type; };
	template<> struct hson_type<uint16_t> { static constexpr const char* type = "uint16"; using lit = Uint16Type; };
	template<> struct hson_type<int32_t> { static constexpr const char* type = "int32"; using lit = Int32Type; };
	template<> struct hson_type<uint32_t> { static constexpr const char* type = "uint32"; using lit = Uint32Type; };
	template<> struct hson_type<int64_t> { static constexpr const char* type = "int64"; using lit = Int64Type; };
	template<> struct hson_type<uint64_t> { static constexpr const char* type = "uint64"; using lit = Uint64Type; };
	template<> struct hson_type<float> { static constexpr const char* type = "float32"; using lit = Float32Type; };
	template<> struct hson_type<csl::math::Vector2> { static constexpr const char* type = "vector2"; using lit = Vector2Type; };
	template<> struct hson_type<csl::math::Vector3> { static constexpr const char* type = "vector3"; using lit = Vector3Type; };
	template<> struct hson_type<csl::math::Vector4> { static constexpr const char* type = "vector4"; using lit = Vector4Type; };
	template<> struct hson_type<csl::math::Quaternion> { static constexpr const char* type = "quaternion"; using lit = QuaternionType; };
	template<> struct hson_type<csl::math::Matrix34> { static constexpr const char* type = "matrix34"; using lit = Matrix34Type; };
	template<> struct hson_type<csl::math::Matrix44> { static constexpr const char* type = "matrix44"; using lit = Matrix44Type; };
	template<> struct hson_type<hh::game::ObjectId> { static constexpr const char* type = "object_reference"; using lit = ObjectIdType; };
	template<> struct hson_type<csl::ut::Color<uint8_t>> { static constexpr const char* type = "color8"; using lit = ColorByteType; };
	template<> struct hson_type<csl::ut::Color<float>> { static constexpr const char* type = "colorf"; using lit = ColorFloatType; };
	template<> struct hson_type<Flags> { static constexpr const char* type = "flags"; using lit = FlagsType; };
	template<> struct hson_type<csl::ut::VariableString> { static constexpr const char* type = "string"; using lit = StringType; };
	template<> struct hson_type<double> { static constexpr const char* type = "float64"; using lit = Float64Type; };
	template<> struct hson_type<char> { static constexpr const char* type = "char"; using lit = CharType; };


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

	template<typename T>
	struct MemberDefT {
		std::string name{};
		std::string type{};
		std::optional<std::string> subtype{};
		std::optional<unsigned int> array_size{};
		std::optional<rfl::Object<std::string>> descriptions{};
	};

	template<typename T>
	struct RangeMemberDefT {
		std::string name{};
		std::string type{};
		std::optional<std::string> subtype{};
		std::optional<rfl_range_rep_t<rfl_range_type_t<T>>> min_range{};
		std::optional<rfl_range_rep_t<rfl_range_type_t<T>>> max_range{};
		std::optional<rfl_range_rep_t<rfl_range_type_t<T>>> step{};
		std::optional<unsigned int> array_size{};
		std::optional<rfl::Object<std::string>> descriptions{};
	};

	using MemberDef = rfl::Variant<
		MemberDefT<bool>,
		RangeMemberDefT<int8_t>,
		RangeMemberDefT<uint8_t>,
		RangeMemberDefT<int16_t>,
		RangeMemberDefT<uint16_t>,
		RangeMemberDefT<int32_t>,
		RangeMemberDefT<uint32_t>,
		RangeMemberDefT<int64_t>,
		RangeMemberDefT<uint64_t>,
		RangeMemberDefT<float>,
		RangeMemberDefT<csl::math::Vector2>,
		RangeMemberDefT<csl::math::Vector3>,
		RangeMemberDefT<csl::math::Vector4>,
		MemberDefT<csl::math::Quaternion>,
		MemberDefT<csl::math::Matrix34>,
		MemberDefT<csl::math::Matrix44>,
		MemberDefT<hh::game::ObjectId>,
		MemberDefT<csl::ut::VariableString>,
		MemberDefT<Flags>,
		MemberDefT<csl::ut::Color<uint8_t>>,
		MemberDefT<csl::ut::Color<float>>
	>;

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
	MemberDefT<T> GenerateMemberT(const hh::fnd::RflClassMember* member, const char* typeName) {
		auto* caption = member->GetAttribute("Caption");
		auto arraySize = member->GetCstyleArraySize();
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
			arraySize > 1 ? std::make_optional(arraySize) : std::nullopt,
			std::move(captionOpt),
		};
	}

	template<typename T>
	RangeMemberDefT<T> GenerateRangeMemberT(const hh::fnd::RflClassMember* member, const char* typeName, const char* rangeAttrName) {
		auto* caption = member->GetAttribute("Caption");
		auto arraySize = member->GetCstyleArraySize();
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
				std::nullopt,
				std::nullopt,
				arraySize > 1 ? std::make_optional(arraySize) : std::nullopt,
				std::move(captionOpt),
			};
		}
		else {
			auto* ranges = static_cast<const rfl_range_representation_t<rfl_range_type_t<T>>*>(rangeAttr->GetData());

			return {
				member->GetName(),
				typeName ? typeName : hson_type<T>::type,
				std::nullopt,
				std::move(TidyRange<rfl_range_type_t<T>>(ranges[0])),
				std::move(TidyRange<rfl_range_type_t<T>>(ranges[1])),
				std::move(TidyRange<rfl_range_type_t<T>>(ranges[2])),
				arraySize > 1 ? std::make_optional(arraySize) : std::nullopt,
				std::move(captionOpt),
			};
		}
	}

	template<typename T>
	MemberDefT<T> GenerateArrayMemberT(const hh::fnd::RflClassMember* member, const char* typeName) {
		MemberDefT<T> def = GenerateMemberT<T>(member, typeName);
		return { def.name, "array", def.type, def.array_size, def.descriptions };
	}

	template<typename T>
	RangeMemberDefT<T> GenerateRangeArrayMemberT(const hh::fnd::RflClassMember* member, const char* typeName, const char* rangeAttrName) {
		RangeMemberDefT<T> def = GenerateRangeMemberT<T>(member, typeName, rangeAttrName);
		return { def.name, "array", def.type, def.min_range, def.max_range, def.step, def.array_size, def.descriptions };
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
		case hh::fnd::RflClassMember::TYPE_MATRIX34:
			return GenerateMemberT<csl::math::Matrix34>(member, typeName);
		case hh::fnd::RflClassMember::TYPE_MATRIX44:
			return GenerateMemberT<csl::math::Matrix44>(member, typeName);
		case hh::fnd::RflClassMember::TYPE_OBJECT_ID:
			return GenerateMemberT<hh::game::ObjectId>(member, typeName);
		case hh::fnd::RflClassMember::TYPE_STRING:
			return GenerateMemberT<csl::ut::VariableString>(member, typeName);
		case hh::fnd::RflClassMember::TYPE_FLAGS:
			return GenerateMemberT<Flags>(member, typeName);
		case hh::fnd::RflClassMember::TYPE_COLOR_BYTE:
			return GenerateMemberT<csl::ut::Color<uint8_t>>(member, typeName);
		case hh::fnd::RflClassMember::TYPE_COLOR_FLOAT:
			return GenerateMemberT<csl::ut::Color<float>>(member, typeName);
		default:
			assert(false);
			return {};
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
		case hh::fnd::RflClassMember::TYPE_MATRIX34:
			return GenerateArrayMemberT<csl::math::Matrix34>(member, typeName);
		case hh::fnd::RflClassMember::TYPE_MATRIX44:
			return GenerateArrayMemberT<csl::math::Matrix44>(member, typeName);
		case hh::fnd::RflClassMember::TYPE_OBJECT_ID:
			return GenerateArrayMemberT<hh::game::ObjectId>(member, typeName);
		case hh::fnd::RflClassMember::TYPE_STRING:
			return GenerateArrayMemberT<csl::ut::VariableString>(member, typeName);
		case hh::fnd::RflClassMember::TYPE_FLAGS:
			return GenerateArrayMemberT<Flags>(member, typeName);
		case hh::fnd::RflClassMember::TYPE_COLOR_BYTE:
			return GenerateArrayMemberT<csl::ut::Color<uint8_t>>(member, typeName);
		case hh::fnd::RflClassMember::TYPE_COLOR_FLOAT:
			return GenerateArrayMemberT<csl::ut::Color<float>>(member, typeName);
		default:
			assert(false);
			return {};
		}
	}

	MemberDef GenerateSingleMember(Template& templateDef, const std::string& curNamespace, const hh::fnd::RflClassMember* member) {
		switch (member->GetType()) {
		case hh::fnd::RflClassMember::TYPE_STRUCT: {
			auto* rflClass = member->GetStructClass();
			auto namespacedName = rflClass->GetName();

			templateDef.structs[namespacedName] = GenerateRflClass(templateDef, "", member->GetStructClass());
			return GenerateMemberT<bool>(member, namespacedName);
		}
		case hh::fnd::RflClassMember::TYPE_ENUM: {
			auto* enumClass = member->GetEnumClass();
			auto namespacedName = curNamespace + enumClass->GetName();

			templateDef.enums[namespacedName] = GenerateEnum(enumClass, member->GetSubType());
			return GeneratePrimitiveMember(member, member->GetSubType(), namespacedName.c_str());
		}
		default:
			//std::cout << (int)member->GetType() << std::endl;
			return GeneratePrimitiveMember(member, member->GetType());
		}
	}

	MemberDef GenerateArrayMember(Template& templateDef, const std::string& curNamespace, const hh::fnd::RflClassMember* member) {
		switch (member->GetSubType()) {
		case hh::fnd::RflClassMember::TYPE_STRUCT: {
			auto* rflClass = member->GetStructClass();
			auto namespacedName = rflClass->GetName();

			templateDef.structs[namespacedName] = GenerateRflClass(templateDef, "", member->GetStructClass());
			return GenerateArrayMemberT<bool>(member, namespacedName);
		}
		default:
			return GenerateArrayPrimitiveMember(member, member->GetSubType());
		}
	}

	MemberDef GenerateMember(Template& templateDef, const std::string& curNamespace, const hh::fnd::RflClassMember* member) {
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

	RflClassDef GenerateRflClass(Template& templateDef, const std::string& curNamespace, const hh::fnd::RflClass* rflClass) {
		if (templateDef.structs.contains(rflClass->GetName()))
			return templateDef.structs[rflClass->GetName()];
		else
			templateDef.structs[rflClass->GetName()] = RflClassDef{};

		auto rflClassDef = templateDef.structs[rflClass->GetName()];
		auto* parent = rflClass->m_pParent;

		if (parent) {
			templateDef.structs[parent->GetName()] = GenerateRflClass(templateDef, "", parent);
			rflClassDef.parent = std::move(parent->GetName());
		}

		for (size_t i = 0; i < rflClass->m_pMembers.count; i++) {
			rflClassDef.fields.push_back(GenerateMember(templateDef, curNamespace + rflClass->GetName() + std::string{ "::" }, &rflClass->m_pMembers.items[i]));
		}

		return rflClassDef;
	}


	void GenerateHSONTemplate(const std::string& filename, const csl::ut::MoveArray<const hh::game::GameObjectClass*> objects) {
		Template templateDef{};
		templateDef.version = 1;
#ifdef DEVTOOLS_TARGET_SDK_wars
		templateDef.format = "gedit_v2";
#else
		templateDef.format = "gedit_v3";
#endif

		for (auto& objClass : objects) {
			auto* rflClass = objClass->spawnerDataRflClass;

			if (rflClass == nullptr)
				continue;

			templateDef.structs[rflClass->GetName()] = GenerateRflClass(templateDef, "", rflClass);
			templateDef.objects[objClass->name] = { rflClass->GetName(), static_cast<const char*>(objClass->GetAttributeValue("category")) };
		}

		std::ofstream ofs{ filename, std::ios::trunc };
		rfl::json::write(templateDef, ofs, YYJSON_WRITE_PRETTY_TWO_SPACES);
	}

	void GenerateRFLTemplate(const std::string& filename, const csl::ut::StringMap<const hh::fnd::RflClass*>& rflClasses, bool includeSpawnerData) {
		Template templateDef{};
		templateDef.version = 1;
		templateDef.format = "rfl";

		std::map<std::string, const hh::fnd::RflClass*> spawnerData{};
		if (!includeSpawnerData) {
			for (auto& objClass : hh::game::GameObjectSystem::GetInstance()->gameObjectRegistry->GetGameObjectClasses()) {
				auto* rflClass = objClass->spawnerDataRflClass;

				if (rflClass == nullptr)
					continue;
				spawnerData[rflClass->GetName()] = rflClass;
			}
		}
		for (auto* rflClass : rflClasses) {
			if (rflClass == nullptr)
				continue;

			if (!spawnerData.contains(rflClass->GetName()))
				templateDef.structs[rflClass->GetName()] = GenerateRflClass(templateDef, "", rflClass);
		}

		std::ofstream ofs{ filename, std::ios::trunc };
		rfl::json::write(templateDef, ofs, YYJSON_WRITE_PRETTY_TWO_SPACES);
	}

	void GenerateTemplate(const std::string& filename, TemplateType type) {
		switch (type) {
		case HSON:
			GenerateHSONTemplate(filename, hh::game::GameObjectSystem::GetInstance()->gameObjectRegistry->GetGameObjectClasses());
			break;
		case RFL:
			GenerateRFLTemplate(filename, hh::fnd::RflClassNameRegistry::GetInstance()->GetItems(), false);
			break;
		}
	}
}
