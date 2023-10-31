#include "Pch.h"
#include "icons.h"
#include "Desktop.h"

void RenderIcon(IconId id, Desktop& desktop) {
    const IconUV& uv = iconUVs[static_cast<size_t>(id)];
    ImGui::Image(desktop.iconView, ImVec2(uv.w, uv.h), ImVec2(uv.x, uv.y), ImVec2(uv.x + uv.w, uv.y + uv.h));
}
