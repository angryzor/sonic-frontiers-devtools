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
    void RenderElementInspector<1014>(hh::dv::DvElementBase* element) {
#elif DEVTOOLS_TARGET_SDK_miller
    void RenderElementInspector<1016>(hh::dv::DvElementBase* element) {
#endif
        auto* elem = reinterpret_cast<app::dv::DvElementBossName*>(element);
        auto* data = elem->GetData();
		int curBossId = static_cast<int>(data->bossId);
#ifdef DEVTOOLS_TARGET_SDK_rangers
        if (ImGui::Combo("Boss Name", &curBossId, bossNames, 6))
#elif DEVTOOLS_TARGET_SDK_miller
        if (ImGui::Combo("Boss Name", &curBossId, bossNames, 5))
#endif
			data->bossId = static_cast<app::dv::DvElementBossName::Data::BossID>(curBossId);
    }
}
