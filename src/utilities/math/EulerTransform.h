#pragma once

struct EulerTransform {
	csl::math::Vector3 position;
	csl::math::Vector3 rotation;
	csl::math::Vector3 scale;

	EulerTransform(const csl::math::Transform& transform);

	operator csl::math::Transform();

	bool operator==(const EulerTransform& other) const;

	bool operator!=(const EulerTransform& other) const;
};
