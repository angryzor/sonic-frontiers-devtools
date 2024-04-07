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

static char dummy[1] = { '\0' };

static int StringResizeCallback(ImGuiInputTextCallbackData* data)
{
	if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
	{
		auto my_str = static_cast<csl::ut::String*>(data->UserData);
		my_str->reserve(data->BufSize);
		data->Buf = my_str->c_str();
	}
	return 0;
}

void InputText(const char* label, csl::ut::String* str, ImGuiInputTextFlags flags) {
	ImGui::InputText(label, str->c_str() == nullptr ? dummy : str->c_str(), str->size(), flags | ImGuiInputTextFlags_CallbackResize, StringResizeCallback, str);
}

// This is a completely nonstandard hack around the fact that these things don't have resizeable buffers
// (instead they are always allocated to the exact string length) but it shouldn't cause issues.
class ResizeableVariableString : public csl::ut::VariableString {
public:
	void resize(size_t newSize) {
		size_t allocatorAddr = reinterpret_cast<size_t>(m_pAllocator);
		csl::fnd::IAllocator* allocator = m_pAllocator == nullptr ? hh::fnd::MemoryRouter::GetModuleAllocator() : reinterpret_cast<csl::fnd::IAllocator*>(allocatorAddr & ~1);

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

void InputObjectId(const char* label, hh::game::ObjectId* id) {
	auto* objWorld = hh::game::GameManager::GetInstance()->GetService<hh::game::ObjectWorld>();

	if (objWorld == nullptr) {
		ImGui::Text("%s: %016zx%016zx", label, id->groupId, id->objectId);
	}
	else {
		char unkNameBuf[50];
		sprintf_s(unkNameBuf, 50, "<%016zx%016zx>", id->groupId, id->objectId);

		const char* name = unkNameBuf;

		for (auto* chunk : objWorld->GetWorldChunks()) {
			int idx = chunk->GetObjectIndexById(*id);

			if (idx != -1)
				name = chunk->GetWorldObjectStatusByIndex(idx).objectData->name;
		}

		if (ImGui::BeginCombo(label, name)) {
			for (auto* chunk : objWorld->GetWorldChunks()) {
				for (auto* layers : chunk->GetLayers()) {
					for (auto* obj : layers->GetResource()->GetObjects()) {
						if (ImGui::Selectable(obj->name))
							*id = obj->id;
						if (*id == obj->id)
							ImGui::SetItemDefaultFocus();
					}
				}
			}
			ImGui::EndCombo();
		}
	}
}
