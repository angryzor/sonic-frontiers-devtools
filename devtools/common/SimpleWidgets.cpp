#include "../Pch.h"
#include "SimpleWidgets.h"

void MatrixValues(const Eigen::Matrix4f& mat) {
	ImGui::Text("%f %f %f %f", mat(0, 0), mat(0, 1), mat(0, 2), mat(0, 3));
	ImGui::Text("%f %f %f %f", mat(1, 0), mat(1, 1), mat(1, 2), mat(1, 3));
	ImGui::Text("%f %f %f %f", mat(2, 0), mat(2, 1), mat(2, 2), mat(2, 3));
	ImGui::Text("%f %f %f %f", mat(3, 0), mat(3, 1), mat(3, 2), mat(3, 3));
}
