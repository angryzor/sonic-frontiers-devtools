#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
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
        "KOREAN",
#ifdef DEVTOOLS_TARGET_SDK_miller
        "THAI"
#endif
    };

    template<>
#ifdef DEVTOOLS_TARGET_SDK_rangers
    void RenderElementInspector<1015>(hh::dv::DvElementBase* element) {
#elif DEVTOOLS_TARGET_SDK_miller
    void RenderElementInspector<1017>(hh::dv::DvElementBase* element) {
#endif
        auto* elem = reinterpret_cast<app::dv::DvElementCaption*>(element);
        auto* data = elem->GetData();
#ifdef DEVTOOLS_TARGET_SDK_miller
        Editor("Caption Enabled", data->captionEnabled);
        Editor("Sound Enabled", data->soundEnabled);
#endif
		Editor("Converse Name", data->converseName);
#ifdef DEVTOOLS_TARGET_SDK_miller
        Editor("Sound Name", data->soundName);
#endif
        int curLangId = static_cast<int>(data->languageId);
#ifdef DEVTOOLS_TARGET_SDK_rangers
        if (ImGui::Combo("Language", &curLangId, langNames, 12))
#elif DEVTOOLS_TARGET_SDK_miller
        if (ImGui::Combo("Language", &curLangId, langNames, 13))
#endif
			data->languageId = static_cast<app::dv::DvElementCaption::Data::LanguageID>(curLangId);
    }
}
