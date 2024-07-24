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

	return { affine.translation(), Eigen::Quaternionf{ rotation }, scaling.diagonal() };
}

Ray3f ScreenPosToWorldRay(const Eigen::Vector2f& screenPos, const Eigen::Projective3f& inverseCameraMatrix)
{
	Eigen::Vector3f nearPoint{ (inverseCameraMatrix * Eigen::Vector4f{ screenPos.x(), screenPos.y(), 0.0f, 1.0f }).hnormalized() };
	Eigen::Vector3f farPoint{ (inverseCameraMatrix * Eigen::Vector4f{ screenPos.x(), screenPos.y(), 1.0f, 1.0f }).hnormalized() };

	return { nearPoint, farPoint };
}

Ray3f ScreenPosToWorldRay(const ImVec2 screenPos, const Eigen::Projective3f& inverseCameraMatrix)
{
	return ScreenPosToWorldRay(ImGuiCoordsToNDC(screenPos), inverseCameraMatrix);
}

FrustumRays ScreenRectToFrustumRays(const Eigen::Vector2f& screenRect1, const Eigen::Vector2f& screenRect2, const Eigen::Projective3f& inverseCameraMatrix)
{
	Eigen::Vector2f botLeft{ std::fminf(screenRect1.x(), screenRect2.x()), std::fminf(screenRect1.y(), screenRect2.y()) };
	Eigen::Vector2f topRight{ std::fmaxf(screenRect1.x(), screenRect2.x()), std::fmaxf(screenRect1.y(), screenRect2.y()) };

	Eigen::Vector2f botRight{ topRight.x(), botLeft.y() };
	Eigen::Vector2f topLeft{ botLeft.x(), topRight.y() };

	return {
		ScreenPosToWorldRay(topLeft, inverseCameraMatrix),
		ScreenPosToWorldRay(topRight, inverseCameraMatrix),
		ScreenPosToWorldRay(botRight, inverseCameraMatrix),
		ScreenPosToWorldRay(botLeft, inverseCameraMatrix),
	};
}

FrustumRays ScreenRectToFrustumRays(const ImVec2& screenRect1, const ImVec2& screenRect2, const Eigen::Projective3f& inverseCameraMatrix)
{
	return ScreenRectToFrustumRays(ImGuiCoordsToNDC(screenRect1), ImGuiCoordsToNDC(screenRect2), inverseCameraMatrix);
}

Frustum ScreenRectToFrustum(const Eigen::Vector2f& screenRect1, const Eigen::Vector2f& screenRect2, const Eigen::Projective3f& inverseCameraMatrix)
{
	return ScreenRectToFrustumRays(screenRect1, screenRect2, inverseCameraMatrix);
}

Frustum ScreenRectToFrustum(const ImVec2& screenRect1, const ImVec2& screenRect2, const Eigen::Projective3f& inverseCameraMatrix)
{
	return ScreenRectToFrustum(ImGuiCoordsToNDC(screenRect1), ImGuiCoordsToNDC(screenRect2), inverseCameraMatrix);
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
