#include "../Pch.h"
#include "SimpleWidgets.h"

void MatrixValues(const Eigen::Matrix4f& mat) {
	ImGui::Text("%f %f %f %f", mat(0, 0), mat(0, 1), mat(0, 2), mat(0, 3));
	ImGui::Text("%f %f %f %f", mat(1, 0), mat(1, 1), mat(1, 2), mat(1, 3));
	ImGui::Text("%f %f %f %f", mat(2, 0), mat(2, 1), mat(2, 2), mat(2, 3));
	ImGui::Text("%f %f %f %f", mat(3, 0), mat(3, 1), mat(3, 2), mat(3, 3));
}

void ViewportDataInfo(const hh::gfnd::ViewportData& viewport)
{
	ImGui::SeparatorText("Dimensions");
	ImGui::Text("X: %f", viewport.viewportDimensions.x);
	ImGui::Text("Y: %f", viewport.viewportDimensions.y);
	ImGui::Text("Width: %f", viewport.viewportDimensions.width);
	ImGui::Text("Height: %f", viewport.viewportDimensions.height);
	ImGui::Text("Render width: %f", viewport.viewportDimensions.maybeRenderWidth);
	ImGui::Text("Render height: %f", viewport.viewportDimensions.maybeRenderHeight);

	ImGui::SeparatorText("Projection settings");
	ImGui::Text("Aspect ratio: %f", viewport.aspectRatio);
	ImGui::Text("FOV: %f", viewport.fov);
	ImGui::Text("Near clipping plane: %f", viewport.nearClip);
	ImGui::Text("Far clipping plane: %f", viewport.farClip);

	ImGui::SeparatorText("View matrix");
	MatrixValues(viewport.viewMatrix);

	ImGui::SeparatorText("Inverse view matrix");
	MatrixValues(viewport.inverseViewMatrix);

	ImGui::SeparatorText("Projection matrix");
	MatrixValues(viewport.projMatrix);
}

int MyResizeCallback(ImGuiInputTextCallbackData* data)
{
	if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
	{
		ImVector<char>* my_str = (ImVector<char>*)data->UserData;
		IM_ASSERT(my_str->begin() == data->Buf);
		my_str->resize(data->BufSize); // NB: On resizing calls, generally data->BufSize == data->BufTextLen + 1
		data->Buf = my_str->begin();
	}
	return 0;
}

void InputText(const char* label, csl::ut::VariableString str, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = nullptr, void* user_data = nullptr) {
	
}