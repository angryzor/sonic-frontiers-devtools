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

void WorldPositionEditor(hh::fnd::WorldPosition& worldPos) {
	ImGui::DragFloat3("Position", worldPos.m_Position.data());
	ImGui::DragFloat4("Rotation", worldPos.m_Rotation.coeffs().data());
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

// This is completely nonstandard hack around the fact that these things don't have resizeable buffers
// (instead they are always allocated to the exact string length) but it shouldn't cause issues.
class ResizeableVariableString : public csl::ut::VariableString {
	static char dummy[1];
public:
	void resize(size_t newSize) {
		size_t allocatorAddr = reinterpret_cast<size_t>(m_pAllocator);
		csl::fnd::IAllocator* allocator = m_pAllocator == nullptr ? app::fnd::AppHeapManager::GetResidentAllocator() : reinterpret_cast<csl::fnd::IAllocator*>(allocatorAddr & ~1);

		char* oldStr = m_pStr;
		m_pStr = (char*)allocator->Alloc(newSize, 1);
		m_pAllocator = reinterpret_cast<csl::fnd::IAllocator*>(reinterpret_cast<size_t>(allocator) | 1);

		if (oldStr) {
			strcpy_s(m_pStr, newSize, oldStr);

			if (allocatorAddr & 1)
				allocator->Free(oldStr);
		}
		else {
			m_pStr[0] = '\0';
		}
	}

	size_t size() {
		return m_pStr == nullptr ? 0 : strlen(m_pStr);
	}

	char* c_str() {
		return m_pStr == nullptr ? dummy : m_pStr;
	}
};

char ResizeableVariableString::dummy[] = {'\0'};

static int VariableStringResizeCallback(ImGuiInputTextCallbackData* data)
{
	if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
	{
		auto my_str = static_cast<ResizeableVariableString*>(data->UserData);
		//IM_ASSERT(my_str->c_str() == data->Buf);
		my_str->resize(data->BufSize);
		data->Buf = my_str->c_str();
	}
	return 0;
}

void InputText(const char* label, csl::ut::VariableString *str, ImGuiInputTextFlags flags) {
	auto* rstr = static_cast<ResizeableVariableString*>(str);

	ImGui::InputText(label, rstr->c_str(), rstr->size() + 1, flags | ImGuiInputTextFlags_CallbackResize, VariableStringResizeCallback, rstr);
}
