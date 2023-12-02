#pragma once

Eigen::Quaternionf EulerToQuat(const Eigen::Vector3f& vec);
Eigen::Vector3f MatrixToEuler(const Eigen::Matrix3f& mat);
