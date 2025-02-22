#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
#ifdef DEVTOOLS_TARGET_SDK_rangers
    void RenderElementInspector<1006>(hh::dv::DvElementBase* element) {
#elif DEVTOOLS_TARGET_SDK_miller
    void RenderElementInspector<1008>(hh::dv::DvElementBase* element) {
#endif
        auto* elem = reinterpret_cast<app::dv::DvElementAtmosphereGodrayParam*>(element);
        auto* data = elem->GetData();
		Editor("Enabled", data->enabled);
        Editor("Density", data->density);
        Editor("Decay", data->decay);
        Editor("Weight", data->weight);
    }
}
