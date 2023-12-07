#include "Pch.h"
#include "MathUtils.h"

Eigen::Quaternionf EulerToQuat(const Eigen::Vector3f& vec) {
	return Eigen::AngleAxisf(vec[1], Eigen::Vector3f::UnitY()) * Eigen::AngleAxisf(vec[0], Eigen::Vector3f::UnitX()) * Eigen::AngleAxisf(vec[2], Eigen::Vector3f::UnitZ());
}

Eigen::Vector3f MatrixToEuler(const Eigen::Matrix3f& mat) {
	auto absoluteEuler = mat.eulerAngles(1, 0, 2);

	return { absoluteEuler[1], absoluteEuler[0], absoluteEuler[2] };
}

Eigen::Affine3f TransformToAffine3f(const csl::math::Transform& transform) {
	Eigen::Affine3f affine;
	affine.fromPositionOrientationScale(transform.position, transform.rotation, transform.scale);
	return affine;
}

csl::math::Transform Affine3fToTransform(const Eigen::Affine3f& affine) {
	Eigen::Matrix3f rotation;
	Eigen::Matrix3f scaling;

	affine.computeRotationScaling(&rotation, &scaling);

	return { { affine.translation() }, Eigen::Quaternionf{ rotation }, { scaling.diagonal() } };
}
