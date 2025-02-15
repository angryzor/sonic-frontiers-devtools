#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    void RenderElementInspector<1020>(hh::dv::DvElementBase* element) {
        auto* elem = reinterpret_cast<app::dv::DvElementCameraBlurParam*>(element);
        auto* data = elem->GetData();
		CheckboxFlags("Enabled", data->flags, app::dv::DvElementCameraBlurParam::Data::Flags::ENABLED);
		CheckboxFlags("Single Direction OPT", data->flags, app::dv::DvElementCameraBlurParam::Data::Flags::SINGLE_DIRECTION_OPT);
		CheckboxFlags("Curve Enabled", data->flags, app::dv::DvElementCameraBlurParam::Data::Flags::CURVE_ENABLED);
        Editor("Sample Amount", data->sampleAmount);
        Editor("Blur Amount", data->blurAmount);
        Editor("Finish Blur Amount", data->finishBlurAmount);
    }
}
