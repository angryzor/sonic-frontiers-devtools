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
#endif

    template<>
    void RenderElementInspector<1014>(hh::dv::DvElementBase* element) {
        auto* elem = reinterpret_cast<app::dv::DvElementBossName*>(element);
        auto* data = elem->GetData();
		int curBossId = static_cast<int>(data->bossId);
		if (ImGui::Combo("Boss Name", &curBossId, bossNames, 6))
			data->bossId = static_cast<app::dv::DvElementBossName::Data::BossID>(curBossId);
    }
}
