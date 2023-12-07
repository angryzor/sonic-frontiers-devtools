#pragma once

Eigen::Quaternionf EulerToQuat(const Eigen::Vector3f& vec);
Eigen::Vector3f MatrixToEuler(const Eigen::Matrix3f& mat);
Eigen::Affine3f TransformToAffine3f(const csl::math::Transform& transform);
csl::math::Transform Affine3fToTransform(const Eigen::Affine3f& transform);
