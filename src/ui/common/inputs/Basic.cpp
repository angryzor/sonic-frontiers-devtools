#include "Basic.h"

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

bool InputText(const char* label, csl::ut::String& str, ImGuiInputTextFlags flags) {
	return ImGui::InputText(label, str.c_str() == nullptr ? dummy : str.c_str(), str.size(), flags | ImGuiInputTextFlags_CallbackResize, StringResizeCallback, &str);
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

bool InputText(const char* label, csl::ut::VariableString& str, ImGuiInputTextFlags flags) {
	auto& rstr = static_cast<ResizeableVariableString&>(str);

	bool edited = ImGui::InputText(label, rstr.c_str(), rstr.size() + 1, flags | ImGuiInputTextFlags_CallbackResize, VariableStringResizeCallback, &rstr);

	if (ImGui::BeginDragDropTarget()) {
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Resource")) {
			auto* resource = *static_cast<hh::fnd::ManagedResource**>(payload->Data);

			if (!str.GetAllocator())
				str.Set(resource->GetName(), -1, hh::fnd::MemoryRouter::GetModuleAllocator());
			else
				str.Set(resource->GetName());
		}
		ImGui::EndDragDropTarget();
	}

	return edited;
}
