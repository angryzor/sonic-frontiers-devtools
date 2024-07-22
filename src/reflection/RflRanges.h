#pragma once

struct RangeSint32 {
	static constexpr const char* name = "RangeSint32";
	using representation_type = int32_t;
};

struct RangeUint32 {
	static constexpr const char* name = "RangeUint32";
	using representation_type = uint32_t;
};

struct RangeSint64 {
	static constexpr const char* name = "RangeSint64";
	using representation_type = int64_t;
};

struct RangeUint64 {
	static constexpr const char* name = "RangeUint64";
	using representation_type = uint64_t;
};

struct RangeFloat {
	static constexpr const char* name = "RangeFloat";
	using representation_type = float;
};

struct RangeVector2 {
	static constexpr const char* name = "RangeVector2";
	using representation_type = csl::math::Vector2; // Already unaligned.
};

struct RangeVector3 {
	static constexpr const char* name = "RangeVector3";
	using representation_type = Eigen::Vector3f; // Represented by unaligned vector.
};

struct RangeVector4 {
	static constexpr const char* name = "RangeVector4";
	using representation_type = Eigen::Matrix<float, 4, 1, Eigen::DontAlign>; // Represented by unaligned vector.
};

template<typename T> using rfl_range_representation_t = typename T::representation_type;

template<typename T> struct rfl_range_type {};
template<> struct rfl_range_type<int8_t> { using type = RangeSint32; };
template<> struct rfl_range_type<uint8_t> { using type = RangeUint32; };
template<> struct rfl_range_type<int16_t> { using type = RangeSint32; };
template<> struct rfl_range_type<uint16_t> { using type = RangeUint32; };
template<> struct rfl_range_type<int32_t> { using type = RangeSint32; };
template<> struct rfl_range_type<uint32_t> { using type = RangeUint32; };
template<> struct rfl_range_type<int64_t> { using type = RangeSint64; };
template<> struct rfl_range_type<uint64_t> { using type = RangeUint64; };
template<> struct rfl_range_type<float> { using type = RangeFloat; };
template<> struct rfl_range_type<csl::math::Vector2> { using type = RangeVector2; };
template<> struct rfl_range_type<csl::math::Vector3> { using type = RangeVector3; };
template<> struct rfl_range_type<csl::math::Vector4> { using type = RangeVector4; };

template<typename T> using rfl_range_type_t = typename rfl_range_type<T>::type;
