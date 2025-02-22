#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    void RenderElementInspector<1046>(hh::dv::DvElementBase* element) {
        auto* elem = reinterpret_cast<app::dv::DvElementFog*>(element);
        auto* data = elem->GetData();
		CheckboxFlags("Distance Fog Enabled", data->flags, app::dv::DvElementFog::Data::Flags::DISTANCE_FOG_ENABLED);
		CheckboxFlags("Height Fog Enabled", data->flags, app::dv::DvElementFog::Data::Flags::HEIGHT_FOG_ENABLED);
		CheckboxFlags("Curve Enabled", data->flags, app::dv::DvElementFog::Data::Flags::CURVE_ENABLED);
        Editor("Parameters", data->params);
        Editor("Finish Parameters", data->finishParams);
    }
}
