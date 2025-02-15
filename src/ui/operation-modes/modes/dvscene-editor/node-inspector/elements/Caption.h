#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
#ifdef DEVTOOLS_TARGET_SDK_rangers
    const char* langNames[] = {
        "ENGLISH",
        "FRENCH",
        "ITALIAN",
        "GERMAN",
        "SPANISH",
        "POLISH",
        "PORTUGUESE",
        "RUSSIAN",
        "JAPANESE",
        "CHINESE",
        "CHINESE_SIMPLIFIED",
        "KOREAN"
    };

    template<>
    void RenderElementInspector<1015>(hh::dv::DvElementBase* element) {
        auto* elem = reinterpret_cast<app::dv::DvElementCaption*>(element);
        auto* data = elem->GetData();
		Editor("Converse Name", data->converseName);
        int curLangId = static_cast<int>(data->languageId);
		if (ImGui::Combo("Language", &curLangId, langNames, 12))
			data->languageId = static_cast<app::dv::DvElementCaption::Data::LanguageID>(curLangId);
    }
#endif
}
