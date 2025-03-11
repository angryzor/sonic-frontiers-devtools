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
        auto* converseDataColl = hh::fw::Application::GetInstance()->GetModule<hh::text::TextAppModule>()->textLanguageDataCollection->textLanguageData[1]->converseDataCollection;
        csl::ut::MoveArray<const char*> tempNames{ element->GetAllocator() };
        int selected = 0;
        int y = 0;
        for (auto* x : converseDataColl->converseDatas) {
            if (strlen(x->data.key) < 16) {
                tempNames.push_back(x->data.key);
                if (strcmp(x->data.key, data->converseName) == 0)
                    selected = y;
                y++;
            }
        }
#ifdef DEVTOOLS_TARGET_SDK_miller
        Editor("Caption Enabled", data->captionEnabled);
        Editor("Sound Enabled", data->soundEnabled);
#endif
        if (SearchableCombo("Converse Name", &selected, tempNames.begin(), tempNames.size(), 16))
            strcpy(data->converseName, tempNames[selected]);
        tempNames.clear();
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
