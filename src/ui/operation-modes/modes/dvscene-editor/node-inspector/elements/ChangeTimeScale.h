#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    void RenderElementInspector<1028>(hh::dv::DvElementBase* element) {
        auto* elem = reinterpret_cast<app::dv::DvElementChangeTimeScale*>(element);
        auto* data = elem->GetData();
        Editor("Enabled", data->enabled);
        Editor("Time Scale", data->timeScale);
        Editor("Unk1", data->unk1);
        Editor("Unk2", data->unk2);
    }
}
