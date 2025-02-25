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
    void RenderElementInspector<1009>(hh::dv::DvElementBase* element) {
#elif DEVTOOLS_TARGET_SDK_miller
    void RenderElementInspector<1011>(hh::dv::DvElementBase* element) {
#endif
        auto* elem = reinterpret_cast<app::dv::DvElementVignetteParam*>(element);
        auto* data = elem->GetData();
#ifdef DEVTOOLS_TARGET_SDK_rangers
        CheckboxFlags("Enabled", data->flags, app::dv::DvElementVignetteParam::Data::Flags::ENABLED);
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
#elif DEVTOOLS_TARGET_SDK_miller
        CheckboxFlags("Curve Enabled", data->flags, app::dv::DvElementVignetteParam::Data::Flags::CURVE_ENABLED);
        int curGradType = static_cast<int>(data->gradationType);
		if (ImGui::Combo("Gradation Type", &curGradType, gradationModeNames, 2))
			data->gradationType = static_cast<app::dv::DvElementVignetteParam::Data::GradationMode>(curGradType);
        int curBlendMode = static_cast<int>(data->blendMode);
        if (ImGui::Combo("Blend Mode", &curBlendMode, blendModeNames, 5))
            data->blendMode = static_cast<app::dv::DvElementVignetteParam::Data::BlendMode>(curBlendMode);
        Editor("Parameters", data->params);
        Editor("Finish Parameters", data->finishParams);
#endif
    }
}
