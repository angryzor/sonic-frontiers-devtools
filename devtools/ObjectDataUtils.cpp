#include "Pch.h"
#include "ObjectDataUtils.h"
#include "MathUtils.h"

hh::game::ObjectTransformData TransformToObjectTransformData(const Eigen::Affine3f& transform) {
	Eigen::Matrix3f absoluteRotation;
	Eigen::Matrix3f absoluteScaling;

	transform.computeRotationScaling(&absoluteRotation, &absoluteScaling);

	return { { transform.translation() }, MatrixToEuler(absoluteRotation) };
}

Eigen::Affine3f ObjectTransformDataToTransform(hh::game::ObjectTransformData& transformData) {
	Eigen::Affine3f transform;

	transform.fromPositionOrientationScale(transformData.position, EulerToQuat(transformData.rotation), csl::math::Vector3{ 1.0f, 1.0f, 1.0f });

	return transform;
}
