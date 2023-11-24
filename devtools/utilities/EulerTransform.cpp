#include "../Pch.h"
#include "EulerTransform.h"

#define PI 3.14159265359f

EulerTransform::EulerTransform(const csl::math::Transform& transform) : position{ transform.position }, rotation{ transform.rotation.toRotationMatrix().canonicalEulerAngles(0, 1, 2) }, scale{ transform.scale } {}

EulerTransform::operator csl::math::Transform() {
	return {
		position,
		csl::math::Quaternion(
			Eigen::AngleAxisf(rotation[0], Eigen::Vector3f::UnitX()) *
			Eigen::AngleAxisf(rotation[1], Eigen::Vector3f::UnitY()) *
			Eigen::AngleAxisf(rotation[2], Eigen::Vector3f::UnitZ())
		),
		scale,
	};
}

bool EulerTransform::operator==(const EulerTransform& other) const {
	return position == other.position && rotation == other.rotation && scale == other.scale;
}

bool EulerTransform::operator!=(const EulerTransform& other) const {
	return !operator==(other);
}
