#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    const char* gradationTypeNames[] = {
        "GRADATION_MODE_CIRCLE",
        "GRADATION_MODE_LINE"
    };

    const char* blendModeNames[] = {
        "BLEND_MODE_ALPHA_BLEND",
        "BLEND_MODE_ADD",
        "BLEND_MODE_MUL",
        "BLEND_MODE_SCREEN",
        "BLEND_MODE_OVERLAY"
    };

    template<>
#ifdef DEVTOOLS_TARGET_SDK_rangers
    bool RenderElementInspector<1009>(char* element) {
#elif DEVTOOLS_TARGET_SDK_miller
    bool RenderElementInspector<1011>(char* element) {
#endif
        bool changed = false;
        auto* data = reinterpret_cast<app::dv::DvElementVignetteParam::Data*>(element);
#ifdef DEVTOOLS_TARGET_SDK_rangers
        changed |= CheckboxFlags("Depth Enabled", data->flags, app::dv::DvElementVignetteParam::Data::Flags::DEPTH_ENABLED);
#endif
        changed |= CheckboxFlags("Curve Enabled", data->flags, app::dv::DvElementVignetteParam::Data::Flags::CURVE_ENABLED);
        int curGradType = static_cast<int>(data->gradationType);
        if (changed |= ImGui::Combo("Gradation Type", &curGradType, gradationTypeNames, 2))
            data->gradationType = static_cast<app::dv::DvElementVignetteParam::Data::GradationType>(curGradType);
        int curBlendMode = static_cast<int>(data->blendMode);
        if (changed |= ImGui::Combo("Blend Mode", &curBlendMode, blendModeNames, 5))
            data->blendMode = static_cast<app::dv::DvElementVignetteParam::Data::BlendMode>(curBlendMode);
        changed |= Editor("Vignette Parameters", data->vignetteParams);
#ifdef DEVTOOLS_TARGET_SDK_rangers
        if(data->flags.test(app::dv::DvElementVignetteParam::Data::Flags::DEPTH_ENABLED))
            changed |= Editor("Depth Parameters", data->depthParams);
#endif
        if (data->flags.test(app::dv::DvElementVignetteParam::Data::Flags::CURVE_ENABLED))
            changed |= Editor("Finish Vignette Parameters", data->finishVignetteParams);
#ifdef DEVTOOLS_TARGET_SDK_rangers
        if (data->flags.test(app::dv::DvElementVignetteParam::Data::Flags::CURVE_ENABLED) && data->flags.test(app::dv::DvElementVignetteParam::Data::Flags::DEPTH_ENABLED))
            changed |= Editor("Finish Depth Parameters", data->finishDepthParams);
#endif
        return changed;
    }
}
