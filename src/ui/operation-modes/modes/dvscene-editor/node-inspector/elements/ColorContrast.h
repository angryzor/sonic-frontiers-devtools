#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    void RenderElementInspector<1002>(hh::dv::DvElementBase* element) {
        auto* elem = reinterpret_cast<app::dv::DvElementColorContrast*>(element);
        auto* data = elem->GetData();
        CheckboxFlags("Enabled", data->flags, app::dv::DvElementColorContrast::Data::Flags::ENABLED);
        CheckboxFlags("Curve Enabled", data->flags, app::dv::DvElementColorContrast::Data::Flags::CURVE_ENABLED);
        CheckboxFlags("Use HLS Correction", data->flags, app::dv::DvElementColorContrast::Data::Flags::USE_HLS_CORRECTION);
        Editor("Unk0", data->unk0);
        Editor("Dynamic Range", data->dynamicRange);
        Editor("Crush Shadows", data->crushShadows);
        Editor("Crush Highlights", data->crushHighlights);
        Editor("HLS Hue Offset", data->hlsHueOffset);
        Editor("HLS Lightness Offset", data->hlsLightnessOffset);
        Editor("HLS Saturation Offset", data->hlsSaturationOffset);
    }
}
