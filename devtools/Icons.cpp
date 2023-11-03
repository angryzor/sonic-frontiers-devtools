#include "Pch.h"
#include "icons.h"
#include "Desktop.h"

void RenderIcon(IconId id, Desktop& desktop) {
    auto& uv = iconUVs[static_cast<size_t>(id)];

    ImGui::Image(desktop.iconView, ImVec2(uv.w, uv.h), ImVec2(uv.x / atlasWidth, uv.y / atlasHeight), ImVec2((uv.x + uv.w) / atlasWidth, (uv.y + uv.h) / atlasHeight));
}
