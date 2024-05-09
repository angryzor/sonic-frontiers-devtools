#include "GraphicsContext.h"
#include <ui/common/editors/Basic.h>
#include <ui/common/viewers/Basic.h>

bool Editor(const char* label, hh::gfnd::ViewportData& viewport)
{
	bool edited{};
	ImGui::PushID(label);

	ImGui::SeparatorText("Dimensions");
	ImGui::BeginGroup();
	edited |= Editor("X", viewport.viewportDimensions.x);
	edited |= Editor("Y", viewport.viewportDimensions.y);
	edited |= Editor("Width", viewport.viewportDimensions.width);
	edited |= Editor("Height", viewport.viewportDimensions.height);
	edited |= Editor("Render width", viewport.viewportDimensions.maybeRenderWidth);
	edited |= Editor("Render height", viewport.viewportDimensions.maybeRenderHeight);
	ImGui::EndGroup();

	ImGui::SeparatorText("Projection settings");
	Viewer("Aspect ratio", viewport.aspectRatio);
	Viewer("FOV", viewport.fov);
	Viewer("Near clipping plane", viewport.nearClip);
	Viewer("Far clipping plane", viewport.farClip);

	ImGui::SeparatorText("View matrix");
	Viewer("View matrix", viewport.viewMatrix);

	ImGui::SeparatorText("Inverse view matrix");
	Viewer("Inverse view matrix", viewport.inverseViewMatrix);

	ImGui::SeparatorText("Projection matrix");
	Viewer("Projection matrix", viewport.projMatrix);

	ImGui::PopID();

	return edited;
}
