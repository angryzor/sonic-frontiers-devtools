#include "Pch.h"
#include "MathUtils.h"

Eigen::Quaternionf EulerToQuat(const Eigen::Vector3f& vec) {
	return Eigen::AngleAxisf(vec[1], Eigen::Vector3f::UnitY()) * Eigen::AngleAxisf(vec[0], Eigen::Vector3f::UnitX()) * Eigen::AngleAxisf(vec[2], Eigen::Vector3f::UnitZ())
}

Eigen::Vector3f MatrixToEuler(const Eigen::Matrix3f& mat) {
	auto absoluteEuler = mat.eulerAngles(1, 0, 2);

	return { absoluteEuler[1], absoluteEuler[0], absoluteEuler[2] };
}
