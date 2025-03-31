#include "Tag.h"

bool OverlayTag(const char* label, bool selected, ImVec2 pos, ImVec4 color) {
	auto& style = ImGui::GetStyle();
	auto* dl = ImGui::GetWindowDrawList();
	auto primaryTextSize = ImGui::CalcTextSize(label);

	dl->AddRectFilled(pos - primaryTextSize / 2 - style.FramePadding, pos + primaryTextSize / 2 + style.FramePadding, 0x60000000);

	ImGui::SetCursorPos(pos - primaryTextSize / 2);
	ImGui::PushStyleColor(ImGuiCol_Text, color);
	ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.5f, 0.5f));

	bool result = ImGui::Selectable(label, false, ImGuiSelectableFlags_AllowOverlap, primaryTextSize);

	ImGui::PopStyleVar();
	ImGui::PopStyleColor();

	if (ImGui::IsItemHovered())
		ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);

	return result;
}
