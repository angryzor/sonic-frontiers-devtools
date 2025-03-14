#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
#ifdef DEVTOOLS_TARGET_SDK_miller
    const char* gradationModeNames[] = {
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
#endif

    template<>
#ifdef DEVTOOLS_TARGET_SDK_rangers
    bool RenderElementInspector<1009>(char* element) {
#elif DEVTOOLS_TARGET_SDK_miller
    bool RenderElementInspector<1011>(char* element) {
#endif
        bool changed = false;
        auto* data = reinterpret_cast<app::dv::DvElementVignetteParam::Data*>(element);
#ifdef DEVTOOLS_TARGET_SDK_rangers
        changed |= CheckboxFlags("Enabled", data->flags, app::dv::DvElementVignetteParam::Data::Flags::ENABLED);
        if(data->flags.test(app::dv::DvElementVignetteParam::Data::Flags::ENABLED)){
            changed |= CheckboxFlags("Curve Enabled", data->flags, app::dv::DvElementVignetteParam::Data::Flags::CURVE_ENABLED);
            changed |= Editor("Unk1", data->unk1);
            changed |= Editor("Center", data->center);
            changed |= Editor("Direction", data->direction);
            changed |= Editor("Vignette Param", data->param0);
            if(data->flags.test(app::dv::DvElementVignetteParam::Data::Flags::CURVE_ENABLED))
                changed |= Editor("Finish Vignette Param", data->param1);
            changed |= Editor("MinMax Param", data->mmParam0);
            if(data->flags.test(app::dv::DvElementVignetteParam::Data::Flags::CURVE_ENABLED))
                changed |= Editor("Finish MinMax Param", data->mmParam1);
            changed |= Editor("Unk2", data->unk2);
            changed |= Editor("Penumbra Scale 0", data->penumbraScale0);
            changed |= Editor("Penumbra Scale 1", data->penumbraScale1);
            changed |= Editor("Unk3", data->unk3);
        }
#elif DEVTOOLS_TARGET_SDK_miller
        changed |= CheckboxFlags("Curve Enabled", data->flags, app::dv::DvElementVignetteParam::Data::Flags::CURVE_ENABLED);
        int curGradType = static_cast<int>(data->gradationType);
		if (changed |= ImGui::Combo("Gradation Type", &curGradType, gradationModeNames, 2))
			data->gradationType = static_cast<app::dv::DvElementVignetteParam::Data::GradationMode>(curGradType);
        int curBlendMode = static_cast<int>(data->blendMode);
        if (changed |= ImGui::Combo("Blend Mode", &curBlendMode, blendModeNames, 5))
            data->blendMode = static_cast<app::dv::DvElementVignetteParam::Data::BlendMode>(curBlendMode);
        changed |= Editor("Parameters", data->params);
        if(data->flags.test(app::dv::DvElementVignetteParam::Data::Flags::CURVE_ENABLED))
            changed |= Editor("Finish Parameters", data->finishParams);
#endif
        return changed;
    }
}
