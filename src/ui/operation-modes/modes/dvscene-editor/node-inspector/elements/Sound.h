#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    const char* soundIdNames[] = {
        "BGM",
        "SFX",
        "Voice"
    };

    template<>
#ifdef DEVTOOLS_TARGET_SDK_rangers
    bool RenderElementInspector<1016>(char* element) {
#elif DEVTOOLS_TARGET_SDK_miller
    bool RenderElementInspector<1018>(char* element) {
#endif
        bool changed = false;
        auto* data = reinterpret_cast<app::dv::DvElementSound::Data*>(element);
		changed |= Editor("Cue Name", data->cueName);
        int curSoundId = static_cast<int>(data->soundId);
		if (changed |= ImGui::Combo("Sound ID", &curSoundId, soundIdNames, 3))
            data->soundId = static_cast<app::dv::DvElementSound::Data::SoundID>(curSoundId);
        return changed;
    }
}
