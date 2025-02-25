#pragma once
#include "Timeline.h"

namespace ui::operation_modes::modes::dvscene_editor {
    constexpr std::pair<int, std::pair<size_t, size_t>> RenderTimelineElements[] = {
        {1, {offsetof(hh::dv::DvElementCameraParams::Data, curveData), 256}},
        {7, {offsetof(hh::dv::DvElementCameraShakeLoop::Data, curveData), 64}},
        {8, {offsetof(hh::dv::DvElementEffect::Data, curveData), 128}},
        {10, {offsetof(hh::dv::DvElementPathInterpolation::Data, curveData), 128}},
        {17, {offsetof(hh::dv::DvElementCameraOffset::Data, curveData), 256}},
        {18, {offsetof(hh::dv::DvElementModelFade::Data, curveData), 128}},
        {20, {offsetof(hh::dv::DvElementCameraHedgehog::Data, curveData), 32}},
        {24, {offsetof(hh::dv::DvElementSpotlight::Data, curveData), 64}},
#ifdef DEVTOOLS_TARGET_SDK_miller
        {22, {offsetof(hh::dv::DvElementPointLight::Data, curveData), 128}},
        {27, {offsetof(hh::dv::DvElementMaterialParam::Data, curveData), 32}},
#endif
        {1000, {offsetof(app::dv::DvElementBloomParam::Data, curveData), 32}},
        {1001, {offsetof(app::dv::DvElementDOFParam::Data, curveData), 32}},
        {1002, {offsetof(app::dv::DvElementColorContrast::Data, curveData), 32}},
        {1003, {offsetof(app::dv::DvElementCameraControlParam::Data, curveData), 32}},
#ifdef DEVTOOLS_TARGET_SDK_rangers
        {1005, {offsetof(app::dv::DvElementGodrayParam::Data, curveData), 32}},
        {1008, {offsetof(app::dv::DvElementChromaticAberrationFilterParam::Data, curveData), 32}},
        {1009, {offsetof(app::dv::DvElementVignetteParam::Data, curveData), 32}},
        {1010, {offsetof(app::dv::DvElementFade::Data, curveData), 32}},
        {1011, {offsetof(app::dv::DvElementLetterBox::Data, curveData), 32}},
        {1017, {offsetof(app::dv::DvElementTime::Data, curveData), 32}},
        {1018, {offsetof(app::dv::DvElementSun::Data, curveData), 32}},
        {1019, {offsetof(app::dv::DvElementLookAtIK::Data, curveData), 64}},
        {1020, {offsetof(app::dv::DvElementCameraBlurParam::Data, curveData), 32}},
        {1025, {offsetof(app::dv::DvElementFacialAnimation::Data, curveData), 32}},
        {1027, {offsetof(app::dv::DvElementAura::Data, curveData), 32}},
        {1029, {offsetof(app::dv::DvElementCyberSpaceNoise::Data, curveData), 32}},
        {1033, {offsetof(app::dv::DvElementCrossFade::Data, curveData), 32}},
        {1034, {offsetof(app::dv::DvElementWeather::Data, curveData), 32}},
        {1036, {offsetof(app::dv::DvElementVariablePointLight::Data, curveData), 128}},
        {1042, {offsetof(app::dv::DvElementTheEndCableObject::Data, curveData), 1024}}
#elif DEVTOOLS_TARGET_SDK_miller
        {1007, {offsetof(app::dv::DvElementGodrayParam::Data, curveData), 32}},
        {1010, {offsetof(app::dv::DvElementChromaticAberrationFilterParam::Data, curveData), 32}},
        {1011, {offsetof(app::dv::DvElementVignetteParam::Data, curveData), 32}},
        {1012, {offsetof(app::dv::DvElementFade::Data, curveData), 32}},
        {1013, {offsetof(app::dv::DvElementLetterBox::Data, curveData), 32}},
        {1019, {offsetof(app::dv::DvElementTime::Data, curveData), 32}},
        {1020, {offsetof(app::dv::DvElementSun::Data, curveData), 32}},
        {1021, {offsetof(app::dv::DvElementLookAtIK::Data, curveData), 64}},
        {1022, {offsetof(app::dv::DvElementCameraBlurParam::Data, curveData), 32}},
        {1027, {offsetof(app::dv::DvElementFacialAnimation::Data, curveData), 32}},
        {1029, {offsetof(app::dv::DvElementAura::Data, curveData), 32}},
        {1031, {offsetof(app::dv::DvElementCyberSpaceNoise::Data, curveData), 32}},
        {1035, {offsetof(app::dv::DvElementCrossFade::Data, curveData), 32}},
        {1036, {offsetof(app::dv::DvElementWeather::Data, curveData), 32}},
        {1038, {offsetof(app::dv::DvElementVariablePointLight::Data, curveData), 128}},
        {1046, {offsetof(app::dv::DvElementFog::Data, curveData), 32}},
        {1047, {offsetof(app::dv::DvElementDOF::Data, curveData), 32}},
#endif
    };

    constexpr std::pair<size_t, size_t> GetElementTimelineRender(int type) {
        for (const auto& [key, func] : RenderTimelineElements) {
            if (key == type) return func;
        }
        return {};
    }
}
