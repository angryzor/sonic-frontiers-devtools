#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
#ifdef DEVTOOLS_TARGET_SDK_miller
    const char* camBlurTypeNames[] = {
        "BLURTYPE_CAMERA",
        "BLURTYPE_PREV_SURFACE",
        "BLURTYPE_RADIAL",
    };
#endif

    template<>
#ifdef DEVTOOLS_TARGET_SDK_rangers
    bool RenderElementInspector<1020>(char* element) {
#elif DEVTOOLS_TARGET_SDK_miller
    bool RenderElementInspector<1022>(char* element) {
#endif
        bool changed = false;
        auto* data = reinterpret_cast<app::dv::DvElementCameraBlurParam::Data*>(element);
		changed |= CheckboxFlags("Enabled", data->flags, app::dv::DvElementCameraBlurParam::Data::Flags::ENABLED);
        if(data->flags.test(app::dv::DvElementCameraBlurParam::Data::Flags::ENABLED)){
            changed |= CheckboxFlags("Single Direction OPT", data->flags, app::dv::DvElementCameraBlurParam::Data::Flags::SINGLE_DIRECTION_OPT);
            changed |= CheckboxFlags("Curve Enabled", data->flags, app::dv::DvElementCameraBlurParam::Data::Flags::CURVE_ENABLED);
            changed |= Editor("Sample Amount", data->sampleAmount);
#ifdef DEVTOOLS_TARGET_SDK_rangers
            changed |= Editor("Blur Amount", data->blurAmount);
            if(data->flags.test(app::dv::DvElementCameraBlurParam::Data::Flags::CURVE_ENABLED))
                changed |= Editor("Finish Blur Amount", data->finishBlurAmount);
#elif DEVTOOLS_TARGET_SDK_miller
            changed |= Editor("Parameters", data->params);
            if(data->flags.test(app::dv::DvElementCameraBlurParam::Data::Flags::CURVE_ENABLED))
                changed |= Editor("Finish Parameters", data->finishParams);
            changed |= ComboEnum("Blur Type", data->blurType, camBlurTypeNames);
#endif
        }
        return changed;
    }
}
