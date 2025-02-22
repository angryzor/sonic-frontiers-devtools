#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    const char* shadowFilterNames[] = {
        "SHADOW_FILTER_POINT",
        "SHADOW_FILTER_PCF",
        "SHADOW_FILTER_PCSS",
        "SHADOW_FILTER_ESM",
        "SHADOW_FILTER_MSM",
        "SHADOW_FILTER_VSM_POINT",
        "SHADOW_FILTER_VSM_LINEAR",
        "SHADOW_FILTER_VSM_ANISO_2",
        "SHADOW_FILTER_VSM_ANISO_4",
        "SHADOW_FILTER_VSM_ANISO_8",
        "SHADOW_FILTER_VSM_ANISO_16",
        "SHADOW_FILTER_COUNT",
        "SHADOW_FILTER_VSM_FIRST",
        "SHADOW_FILTER_VSM_LAST"
    };
    
    const char* shadowRangeTypeNames[] = {
        "SHADOW_RANGE_TYPE_CAMERA_LOOKAT",
        "SHADOW_RANGE_TYPE_POSITION_MANUAL",
        "SHADOW_RANGE_TYPE_FULL_MANUAL",
        "SHADOW_RANGE_TYPE_COUNT",
        "SHADOW_RANGE_TYPE_DEFAULT"
    };
    
    const char* fitProjectionNames[] = {
        "FIT_PROJECTION_TO_CASCADES",
        "FIT_PROJECTION_TO_SCENE",
        "FIT_PROJECTION_TO_ROTATE_CASCADES"
    };
    
    const char* fitNearFarNames[] = {
        "FIT_NEARFAR_ZERO_ONE",
        "FIT_NEARFAR_AABB",
        "FIT_NEARFAR_SCENE_AABB"
    };
    
    const char* partitionTypeNames[] = {
        "PARTITION_PSSM",
        "PARTITION_MANUAL"
    };

    template<>
#ifdef DEVTOOLS_TARGET_SDK_rangers
    void RenderElementInspector<1035>(hh::dv::DvElementBase* element) {
#elif DEVTOOLS_TARGET_SDK_miller
    void RenderElementInspector<1037>(hh::dv::DvElementBase* element) {
#endif
        auto* elem = reinterpret_cast<app::dv::DvElementShadowMapParam*>(element);
        auto* data = elem->GetData();
		int curShadowFilter = static_cast<int>(data->shadowFilter);
        if (ImGui::Combo("Shadow Filter", &curShadowFilter, shadowFilterNames, 14))
            data->shadowFilter = static_cast<app::dv::DvElementShadowMapParam::Data::ShadowFilter>(curShadowFilter);
        int curShadowRangeType = static_cast<int>(data->shadowRangeType);
        if (ImGui::Combo("Shadow Range Type", &curShadowRangeType, shadowRangeTypeNames, 5))
            data->shadowRangeType = static_cast<app::dv::DvElementShadowMapParam::Data::ShadowRangeType>(curShadowRangeType);
        int curFitProjection = static_cast<int>(data->fitProjection);
        if (ImGui::Combo("Fit Projection", &curFitProjection, fitProjectionNames, 5))
            data->fitProjection = static_cast<app::dv::DvElementShadowMapParam::Data::FitProjection>(curFitProjection);
        int curFitNearFar = static_cast<int>(data->fitNearFar);
        if (ImGui::Combo("Fit Near Far", &curFitNearFar, fitNearFarNames, 3))
            data->fitNearFar = static_cast<app::dv::DvElementShadowMapParam::Data::FitNearFar>(curFitNearFar);
        int curPartitionType = static_cast<int>(data->partitionType);
        if (ImGui::Combo("Partition Type", &curPartitionType, partitionTypeNames, 3))
            data->partitionType = static_cast<app::dv::DvElementShadowMapParam::Data::PartitionType>(curPartitionType);
        Editor("Scene Range", data->sceneRange);
        Editor("Scene Center", data->sceneCenter);
        Editor("Manual Light Position", data->manualLightPos);
        Editor("PSSM Lambda", data->pssmLambda);
        Editor("Cascade Offset", data->cascadeOffset);
        Editor("Cascade Level", data->cascadeLevel);
        Editor("Cascade Splits", data->cascadeSplits);
        Editor("Cascade Bias", data->cascadeBias);
        Editor("Bias", data->bias);
        Editor("Offset", data->offset);
        Editor("Normal Bias", data->normalBias);
        Editor("Blur Quality", data->blurQuality);
        Editor("Blur Size", data->blurSize);
        Editor("Fadeout Distance", data->fadeoutDistance);
        Editor("Cascade Transition Fade Distance", data->cascadeTransitionFadeDistance);
        CheckboxFlags("Enable Back Face Shadow", data->flags, app::dv::DvElementShadowMapParam::Data::Flags::ENABLE_BACK_FACE_SHADOW);
        CheckboxFlags("Enable Shadow Camera", data->flags, app::dv::DvElementShadowMapParam::Data::Flags::ENABLE_SHADOW_CAMERA);
        CheckboxFlags("Enable Draw Scene AABB", data->flags, app::dv::DvElementShadowMapParam::Data::Flags::ENABLE_DRAW_SCENE_AABB);
        CheckboxFlags("Enable Draw Shadow Frustrum", data->flags, app::dv::DvElementShadowMapParam::Data::Flags::ENABLE_DRAW_SHADOW_FRUSTRUM);
        CheckboxFlags("Enable Draw Cascade", data->flags, app::dv::DvElementShadowMapParam::Data::Flags::ENABLE_DRAW_CASCADE);
        CheckboxFlags("Enable Draw Camera", data->flags, app::dv::DvElementShadowMapParam::Data::Flags::ENABLE_DRAW_CAMERA_FRUSTRUM);
        CheckboxFlags("Enable Pause Camera", data->flags, app::dv::DvElementShadowMapParam::Data::Flags::ENABLE_PAUSE_CAMERA);
    }
}
