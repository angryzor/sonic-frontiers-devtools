#include "Needle.h"
#include <imgui_internal.h>

template<typename T>
bool BitFieldEditor(const char* label, ImGuiDataType dtype, T* field, T startBit, T size) {
	bool edited{};
	T minVal = 0;
	T maxVal = ((1 << size) - 1);
	T orig = (*field >> startBit) & maxVal;
	T v = orig;

	if (edited = ImGui::DragScalar(label, dtype, &v, 1.0f, &minVal, &maxVal))
		*field = (*field & ~(maxVal << startBit)) | (v << startBit);

	return edited;
}

bool Editor(const char* label, hh::needle::RsFlagMask& mask)
{
	bool edited{};

	ImGui::SeparatorText(label);
	ImGui::Text("Mask: %zx", mask.flags);
	edited |= BitFieldEditor("Blend mode 1", ImGuiDataType_U64, reinterpret_cast<uint64_t*>(&mask.flags), 28ui64, 4ui64);
	edited |= BitFieldEditor("Blend mode 2", ImGuiDataType_U64, reinterpret_cast<uint64_t*>(&mask.flags), 23ui64, 4ui64);
	edited |= BitFieldEditor("Blend op", ImGuiDataType_U64, reinterpret_cast<uint64_t*>(&mask.flags), 19ui64, 3ui64);
	edited |= BitFieldEditor("Alpha blend mode 1", ImGuiDataType_U64, reinterpret_cast<uint64_t*>(&mask.flags), 14ui64, 4ui64);
	edited |= BitFieldEditor("Alpha blend mode 2", ImGuiDataType_U64, reinterpret_cast<uint64_t*>(&mask.flags), 9ui64, 4ui64);
	edited |= BitFieldEditor("Alpha blend op", ImGuiDataType_U64, reinterpret_cast<uint64_t*>(&mask.flags), 5ui64, 3ui64);
	edited |= ImGui::CheckboxFlags("Enable blending", reinterpret_cast<uint64_t*>(&mask.flags), 1ui64 << 46ui64);
	edited |= ImGui::CheckboxFlags("Cull front", reinterpret_cast<uint64_t*>(&mask.flags), 1ui64 << 41ui64);
	edited |= ImGui::CheckboxFlags("Cull back", reinterpret_cast<uint64_t*>(&mask.flags), 1ui64 << 40ui64);
	edited |= ImGui::CheckboxFlags("Cull unknown", reinterpret_cast<uint64_t*>(&mask.flags), 1ui64 << 39ui64);
	ImGui::Text("Mask: %zx", mask.flags);

	return edited;
}
