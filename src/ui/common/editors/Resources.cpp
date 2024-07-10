#include "Resources.h"
#include <ui/resources/editors/ResFxColFile2Editor.h>

void Editor(const char* label, app::gfx::ResFxColFile2& obj) {
	if (ImGui::Button("Edit"))
		ResFxColFile2Editor::Create(Desktop::instance->GetAllocator(), &obj);
	ImGui::Text("%s: ResFxColFile2 - %s", label, obj.GetName());
}
