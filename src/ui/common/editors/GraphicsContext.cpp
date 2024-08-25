#include "GraphicsContext.h"
#include <ui/common/editors/Basic.h>
#include <ui/common/viewers/Basic.h>

const char* projectionTypeNames[]{ "Perspective (FOV)", "Perspective (frustum)", "Orthogonal" };
bool Editor(const char* label, hh::gfnd::ViewportData& viewport)
{
	bool edited{};
	ImGui::PushID(label);

	ImGui::SeparatorText("Dimensions");
	ImGui::BeginGroup();
	edited |= Editor("X", viewport.viewportDimensions.x);
	edited |= Editor("Y", viewport.viewportDimensions.y);
	edited |= Editor("Resolution X", viewport.viewportDimensions.resX);
	edited |= Editor("Resolution Y", viewport.viewportDimensions.resY);
#ifndef DEVTOOLS_TARGET_SDK_wars
	edited |= Editor("Render resolution X", viewport.viewportDimensions.renderResX);
	edited |= Editor("Render resolution Y", viewport.viewportDimensions.renderResY);
#endif
	ImGui::EndGroup();

	ImGui::SeparatorText("View settings");
	if (BeginVectorViewerTable("LookAt position", false)) {
		Viewer("LookAt position", viewport.lookAtPos);
		EndVectorViewerTable();
	}

	ImGui::SeparatorText("Projection settings");
	Viewer("Projection type", projectionTypeNames[static_cast<uint8_t>(viewport.projectionType)]);

	switch (viewport.projectionType) {
	case hh::gfnd::ViewportData::ProjectionType::PERSPECTIVE_FOV:
		Viewer("Aspect ratio", viewport.projectionParameters.fov.aspectRatio);
		Viewer("FOV", viewport.projectionParameters.fov.fov);
		Viewer("Near clipping plane", viewport.projectionParameters.fov.nearClip);
		Viewer("Far clipping plane", viewport.projectionParameters.fov.farClip);
		break;
	default:
		Viewer("Top", viewport.projectionParameters.frustum.top);
		Viewer("Bottom", viewport.projectionParameters.frustum.bottom);
		Viewer("Left", viewport.projectionParameters.frustum.left);
		Viewer("Right", viewport.projectionParameters.frustum.right);
		Viewer("Near clipping plane", viewport.projectionParameters.frustum.nearClip);
		Viewer("Far clipping plane", viewport.projectionParameters.frustum.farClip);
		break;
	}

	ImGui::SeparatorText("View matrix");
	Viewer("View matrix", viewport.viewMatrix);

	//ImGui::SeparatorText("Inverse view matrix");
	//Viewer("Inverse view matrix", viewport.inverseViewMatrix);

	ImGui::SeparatorText("Projection matrix");
	Viewer("Projection matrix", viewport.projMatrix);

	//Editor("Unk5", viewport.unk5);
	//Editor("Unk7", viewport.unk7);
	//Editor("Unk8", viewport.unk8);

	ImGui::PopID();

	return edited;
}
