#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
#ifdef DEVTOOLS_TARGET_SDK_rangers
    void RenderElementInspector<1028>(hh::dv::DvElementBase* element) {
#elif DEVTOOLS_TARGET_SDK_miller
    void RenderElementInspector<1030>(hh::dv::DvElementBase* element) {
#endif
        auto* elem = reinterpret_cast<app::dv::DvElementChangeTimeScale*>(element);
        auto* data = elem->GetData();
        Editor("Enabled", data->enabled);
        Editor("Time Scale", data->timeScale);
        Editor("Unk1", data->unk1);
        Editor("Unk2", data->unk2);
    }
}
