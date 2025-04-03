#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    bool RenderElementInspector<hh::dv::DvNodeElement::ElementID::FOG>(char* element) {
        bool changed = false;
        auto* data = reinterpret_cast<app::dv::DvElementFog::Data*>(element);
		changed |= CheckboxFlags("Distance Fog Enabled", data->flags, app::dv::DvElementFog::Data::Flags::DISTANCE_FOG_ENABLED);
		changed |= CheckboxFlags("Height Fog Enabled", data->flags, app::dv::DvElementFog::Data::Flags::HEIGHT_FOG_ENABLED);
		changed |= CheckboxFlags("Curve Enabled", data->flags, app::dv::DvElementFog::Data::Flags::CURVE_ENABLED);
        changed |= Editor("Parameters", data->params);
        if(data->flags.test(app::dv::DvElementFog::Data::Flags::CURVE_ENABLED))
            changed |= Editor("Finish Parameters", data->finishParams);
        return changed;
    }
}
