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

Ray3f ScreenPosToWorldRay(const ImVec2 screenPos, const Eigen::Matrix4f& inverseCameraMatrix)
{
	auto ndcPos = ImGuiCoordsToNDC(screenPos);

	Eigen::Vector3f nearPoint{ (inverseCameraMatrix * Eigen::Vector4f{ ndcPos.x(), ndcPos.y(), 0.0f, 1.0f }).hnormalized() };
	Eigen::Vector3f farPoint{ (inverseCameraMatrix * Eigen::Vector4f{ ndcPos.x(), ndcPos.y(), 1.0f, 1.0f }).hnormalized() };

	return { nearPoint, farPoint };
}

Frustum ScreenRectToFrustum(const ImVec2& screenRect1, const ImVec2& screenRect2, const Eigen::Matrix4f inverseCameraMatrix)
{
	auto& io = ImGui::GetIO();

	ImVec2 topLeft{ std::fminf(screenRect1.x, screenRect2.x), std::fminf(screenRect1.y, screenRect2.y) };
	ImVec2 botRight{ std::fmaxf(screenRect1.x, screenRect2.x), std::fmaxf(screenRect1.y, screenRect2.y) };

	ImVec2 topRight{ botRight.x, topLeft.y };
	ImVec2 botLeft{ topLeft.x, botRight.y };

	Ray3f topLeftRay{ ScreenPosToWorldRay(topLeft, inverseCameraMatrix) };
	Ray3f topRightRay{ ScreenPosToWorldRay(topRight, inverseCameraMatrix) };
	Ray3f botRightRay{ ScreenPosToWorldRay(botRight, inverseCameraMatrix) };
	Ray3f botLeftRay{ ScreenPosToWorldRay(botLeft, inverseCameraMatrix) };

	Eigen::Hyperplane<float, 3> planes[]{
		{ (botLeftRay.end - botLeftRay.start).cross(topLeftRay.start - botLeftRay.start), botLeftRay.start },
		{ (topLeftRay.end - topLeftRay.start).cross(topRightRay.start - topLeftRay.start), topLeftRay.start },
		{ (topRightRay.end - topRightRay.start).cross(botRightRay.start - topRightRay.start), topRightRay.start },
		{ (botRightRay.end - botRightRay.start).cross(botLeftRay.start - botRightRay.start), botRightRay.start },
		{ (botLeftRay.start - botRightRay.start).cross(topRightRay.start - botRightRay.start), botRightRay.start },
		{ (botRightRay.end - botLeftRay.end).cross(topLeftRay.end - botLeftRay.end), botLeftRay.end },
	};

	return planes;
}

Eigen::Vector2f ImGuiCoordsToNDC(const ImVec2& coords)
{
	auto& io = ImGui::GetIO();

	return { 2 * coords.x / io.DisplaySize.x - 1, 1 - 2 * coords.y / io.DisplaySize.y };
}

ImVec2 NDCCoordsToImGui(const Eigen::Vector2f& coords)
{
	auto& io = ImGui::GetIO();

	return { (coords.x() + 1.0f) * 0.5f * io.DisplaySize.x, (1.0f - coords.y()) * 0.5f * io.DisplaySize.y };
}
