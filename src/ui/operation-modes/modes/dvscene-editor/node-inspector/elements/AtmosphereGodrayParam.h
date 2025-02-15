#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    void RenderElementInspector<1006>(hh::dv::DvElementBase* element) {
        auto* elem = reinterpret_cast<app::dv::DvElementAtmosphereGodrayParam*>(element);
        auto* data = elem->GetData();
		Editor("Enabled", data->enabled);
        Editor("Density", data->density);
        Editor("Decay", data->decay);
        Editor("Weight", data->weight);
    }
}
