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
	edited |= BitFieldEditor("Blend mode 1", ImGuiDataType_U64, reinterpret_cast<uint64_t*>(&mask.flags), 28ull, 4ull);
	edited |= BitFieldEditor("Blend mode 2", ImGuiDataType_U64, reinterpret_cast<uint64_t*>(&mask.flags), 23ull, 4ull);
	edited |= BitFieldEditor("Blend op", ImGuiDataType_U64, reinterpret_cast<uint64_t*>(&mask.flags), 19ull, 3ull);
	edited |= BitFieldEditor("Alpha blend mode 1", ImGuiDataType_U64, reinterpret_cast<uint64_t*>(&mask.flags), 14ull, 4ull);
	edited |= BitFieldEditor("Alpha blend mode 2", ImGuiDataType_U64, reinterpret_cast<uint64_t*>(&mask.flags), 9ull, 4ull);
	edited |= BitFieldEditor("Alpha blend op", ImGuiDataType_U64, reinterpret_cast<uint64_t*>(&mask.flags), 5ull, 3ull);
	edited |= ImGui::CheckboxFlags("Enable blending", reinterpret_cast<uint64_t*>(&mask.flags), 1ull << 46ull);
	edited |= ImGui::CheckboxFlags("Cull front", reinterpret_cast<uint64_t*>(&mask.flags), 1ull << 41ull);
	edited |= ImGui::CheckboxFlags("Cull back", reinterpret_cast<uint64_t*>(&mask.flags), 1ull << 40ull);
	edited |= ImGui::CheckboxFlags("Cull unknown", reinterpret_cast<uint64_t*>(&mask.flags), 1ull << 39ull);
	ImGui::Text("Mask: %zx", mask.flags);

	return edited;
}
