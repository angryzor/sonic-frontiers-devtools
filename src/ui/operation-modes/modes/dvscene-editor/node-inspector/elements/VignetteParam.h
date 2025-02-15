#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    void RenderElementInspector<1009>(hh::dv::DvElementBase* element) {
        auto* elem = reinterpret_cast<app::dv::DvElementVignetteParam*>(element);
        auto* data = elem->GetData();
		CheckboxFlags("Unk0", data->flags, app::dv::DvElementVignetteParam::Data::Flags::UNK0);
		CheckboxFlags("Curve Enabled", data->flags, app::dv::DvElementVignetteParam::Data::Flags::CURVE_ENABLED);
        Editor("Unk1", data->unk1);
        Editor("Center", data->center);
        Editor("Direction", data->direction);
        Editor("Vignette Param", data->param0);
        Editor("Finish Vignette Param", data->param1);
        Editor("MinMax Param", data->mmParam0);
        Editor("Finish MinMax Param", data->mmParam1);
        Editor("Unk2", data->unk2);
        Editor("Penumbra Scale 0", data->penumbraScale0);
        Editor("Penumbra Scale 1", data->penumbraScale1);
        Editor("Unk3", data->unk3);
    }
}
