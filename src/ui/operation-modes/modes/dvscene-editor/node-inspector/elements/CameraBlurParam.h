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
    void RenderElementInspector<1020>(hh::dv::DvElementBase* element) {
#elif DEVTOOLS_TARGET_SDK_miller
    void RenderElementInspector<1022>(hh::dv::DvElementBase* element) {
#endif
        auto* elem = reinterpret_cast<app::dv::DvElementCameraBlurParam*>(element);
        auto* data = elem->GetData();
		CheckboxFlags("Enabled", data->flags, app::dv::DvElementCameraBlurParam::Data::Flags::ENABLED);
		CheckboxFlags("Single Direction OPT", data->flags, app::dv::DvElementCameraBlurParam::Data::Flags::SINGLE_DIRECTION_OPT);
		CheckboxFlags("Curve Enabled", data->flags, app::dv::DvElementCameraBlurParam::Data::Flags::CURVE_ENABLED);
        Editor("Sample Amount", data->sampleAmount);
#ifdef DEVTOOLS_TARGET_SDK_rangers
        Editor("Blur Amount", data->blurAmount);
        Editor("Finish Blur Amount", data->finishBlurAmount);
#elif DEVTOOLS_TARGET_SDK_miller
        Editor("Parameters", data->params);
        Editor("Finish Parameters", data->finishParams);
        int curBlurType = static_cast<int>(data->blurType);
        if (ImGui::Combo("Blur Type", &curBlurType, camBlurTypeNames, 3))
			data->blurType = static_cast<app::dv::DvElementCameraBlurParam::Data::BlurType>(curBlurType);
#endif
    }
}
