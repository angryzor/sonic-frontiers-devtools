#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    const char* ssaoTypeNames[] = {
        "SSAO",
        "HBAO",
        "HBAO_SSS",
        "SSS"
    };

    const char* renderTargetSizeNames[] = {
        "RTSIZE_SAME_AS_FRAMEBUFFER",
        "RTSIZE_ONE_SECOND",
        "RTSIZE_ONE_FORTH"
    };

    const char* blurTypeNames[] = {
        "None",
        "Gauss2x2",
        "Gauss3x3",
        "Gauss4x4",
        "Gauss5x5",
        "Gauss6x6",
        "Gauss7x7",
        "Gauss8x8",
        "Gauss9x9",
        "Bilateral",
        "NoisySample"
    };

    template<>
    bool RenderElementInspector<1005>(char* element) {
        bool changed = false;
        auto* data = reinterpret_cast<app::dv::DvElementSSAOParam::Data*>(element);
        changed |= CheckboxFlags("Enabled", data->flags, app::dv::DvElementSSAOParam::Data::Flags::ENABLED);
        if(data->flags.test(app::dv::DvElementSSAOParam::Data::Flags::ENABLED)){
            changed |= CheckboxFlags("Use Render Target Size", data->flags, app::dv::DvElementSSAOParam::Data::Flags::USE_RENDER_TARGET_SIZE);
            int curSSAOType = static_cast<int>(data->postType);
            if (changed |= ImGui::Combo("SSAO Type", &curSSAOType, ssaoTypeNames, 4))
                data->postType = static_cast<app::dv::DvElementSSAOParam::Data::SSAOType>(curSSAOType);
            int curRenderTargetSize = static_cast<int>(data->renderTargetSize);
            if (changed |= ImGui::Combo("Render Target Size", &curRenderTargetSize, renderTargetSizeNames, 3))
                data->renderTargetSize = static_cast<app::dv::DvElementSSAOParam::Data::RenderTargetSize>(curRenderTargetSize);
            int curBlurType = static_cast<int>(data->blurStep);
            if (changed |= ImGui::Combo("Blur Type", &curBlurType, blurTypeNames, 11))
                data->blurStep = static_cast<app::dv::DvElementSSAOParam::Data::BlurType>(curBlurType);
            changed |= Editor("Intensity", data->intensity);
            changed |= Editor("Radius", data->radius);
            changed |= Editor("Fadeout Distance", data->fadeoutDistance);
            changed |= Editor("Fadeout Radius", data->fadeoutRadius);
            changed |= Editor("Power", data->power);
            changed |= Editor("Bias", data->bias);
            changed |= Editor("Occlusion Distance", data->occlusionDistance);
            changed |= Editor("Direct Lighting Influence", data->directLightingInfluence);
            changed |= Editor("Bilateral Threshold", data->bilateralThreshold);
        }
        return changed;
    }
}
