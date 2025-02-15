#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    const char* soundIdNames[] = {
        "BGM",
        "SFX",
        "Voice"
    };

    template<>
    void RenderElementInspector<1016>(hh::dv::DvElementBase* element) {
        auto* elem = reinterpret_cast<app::dv::DvElementSound*>(element);
        auto* data = elem->GetData();
		Editor("Cue Name", data->cueName);
        int curSoundId = static_cast<int>(data->soundId);
		if (ImGui::Combo("Sound ID", &curSoundId, soundIdNames, 3))
            data->soundId = static_cast<app::dv::DvElementSound::Data::SoundID>(curSoundId);
    }
}
