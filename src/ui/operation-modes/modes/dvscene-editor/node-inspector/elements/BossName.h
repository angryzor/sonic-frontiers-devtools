#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
#ifdef DEVTOOLS_TARGET_SDK_rangers
    const char* bossNames[] = {
        "GIANT",
        "DRAGON",
        "KNIGHT",
        "RIFLE",
        "THEEND",
        "RIFLEBEAST"
    };
#elif DEVTOOLS_TARGET_SDK_miller
    const char* bossNames[] = {
        "BIOLIZARD",
        "METAL_OVERLORD",
        "MEPHILES",
        "DEVIL_DOOM",
        "PERFECT_BLACK_DOOM"
    };
#endif

    template<>
#ifdef DEVTOOLS_TARGET_SDK_rangers
    bool RenderElementInspector<1014>(char* element) {
#elif DEVTOOLS_TARGET_SDK_miller
    bool RenderElementInspector<1016>(char* element) {
#endif
        bool changed = false;
        auto* data = reinterpret_cast<app::dv::DvElementBossName::Data*>(element);
		int curBossId = static_cast<int>(data->bossId);
#ifdef DEVTOOLS_TARGET_SDK_rangers
        if (changed |= ImGui::Combo("Boss Name", &curBossId, bossNames, 6))
#elif DEVTOOLS_TARGET_SDK_miller
        if (changed |= ImGui::Combo("Boss Name", &curBossId, bossNames, 5))
#endif
            data->bossId = static_cast<app::dv::DvElementBossName::Data::BossID>(curBossId);
        return changed;
    }
}
