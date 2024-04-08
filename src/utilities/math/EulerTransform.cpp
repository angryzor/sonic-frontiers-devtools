#include "EulerTransform.h"
#include "MathUtils.h"

EulerTransform::EulerTransform(const csl::math::Transform& transform) : position{ transform.position }, rotation{ MatrixToEuler(transform.rotation.toRotationMatrix()) }, scale{ transform.scale } {}

EulerTransform::operator csl::math::Transform() {
	return { position, EulerToQuat(rotation), scale	};
}

bool EulerTransform::operator==(const EulerTransform& other) const {
	return position == other.position && rotation == other.rotation && scale == other.scale;
}

bool EulerTransform::operator!=(const EulerTransform& other) const {
	return !operator==(other);
}
