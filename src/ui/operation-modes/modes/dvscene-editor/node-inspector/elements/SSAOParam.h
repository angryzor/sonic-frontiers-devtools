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
    void RenderElementInspector<1005>(hh::dv::DvElementBase* element) {
        auto* elem = reinterpret_cast<app::dv::DvElementSSAOParam*>(element);
        auto* data = elem->GetData();
        
        int curSSAOType = static_cast<int>(data->postType);
        if (ImGui::Combo("SSAO Type", &curSSAOType, ssaoTypeNames, 4))
            data->postType = static_cast<app::dv::DvElementSSAOParam::Data::SSAOType>(curSSAOType);
        
        int curRenderTargetSize = static_cast<int>(data->renderTargetSize);
        if (ImGui::Combo("Render Target Size", &curRenderTargetSize, renderTargetSizeNames, 3))
            data->renderTargetSize = static_cast<app::dv::DvElementSSAOParam::Data::RenderTargetSize>(curRenderTargetSize);
        
        int curBlurType = static_cast<int>(data->blurStep);
        if (ImGui::Combo("Blur Type", &curBlurType, blurTypeNames, 11))
            data->blurStep = static_cast<app::dv::DvElementSSAOParam::Data::BlurType>(curBlurType);
        
        Editor("Intensity", data->intensity);
        Editor("Radius", data->radius);
        Editor("Fadeout Distance", data->fadeoutDistance);
        Editor("Fadeout Radius", data->fadeoutRadius);
        Editor("Power", data->power);
        Editor("Bias", data->bias);
        Editor("Occlusion Distance", data->occlusionDistance);
        Editor("Direct Lighting Influence", data->directLightingInfluence);
        Editor("Bilateral Threshold", data->bilateralThreshold);
        
        CheckboxFlags("Enabled", data->flags, app::dv::DvElementSSAOParam::Data::Flags::ENABLED);
        CheckboxFlags("Use Render Target Size", data->flags, app::dv::DvElementSSAOParam::Data::Flags::USE_RENDER_TARGET_SIZE);
    }
}
