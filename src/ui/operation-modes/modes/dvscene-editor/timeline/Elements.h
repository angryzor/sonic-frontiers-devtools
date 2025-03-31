#pragma once
#include "Timeline.h"

namespace ui::operation_modes::modes::dvscene_editor {
    constexpr std::pair<hh::dv::DvNodeElement::ElementID, std::pair<size_t, size_t>> RenderTimelineElements[] = {
        {hh::dv::DvNodeElement::ElementID::CAMERA_PARAMS, {offsetof(hh::dv::DvElementCameraParams::Data, curveData), sizeof(hh::dv::DvElementCameraParams::Data::curveData)}},
        {hh::dv::DvNodeElement::ElementID::CAMERA_SHAKE_LOOP, {offsetof(hh::dv::DvElementCameraShakeLoop::Data, curveData), sizeof(hh::dv::DvElementCameraShakeLoop::Data::curveData)}},
        {hh::dv::DvNodeElement::ElementID::EFFECT, {offsetof(hh::dv::DvElementEffect::Data, curveData), sizeof(hh::dv::DvElementEffect::Data::curveData)}},
        {hh::dv::DvNodeElement::ElementID::PATH_INTERPOLATION, {offsetof(hh::dv::DvElementPathInterpolation::Data, curveData), sizeof(hh::dv::DvElementPathInterpolation::Data::curveData)}},
        {hh::dv::DvNodeElement::ElementID::CAMERA_OFFSET, {offsetof(hh::dv::DvElementCameraOffset::Data, curveData), sizeof(hh::dv::DvElementCameraOffset::Data::curveData)}},
        {hh::dv::DvNodeElement::ElementID::MODEL_FADE, {offsetof(hh::dv::DvElementModelFade::Data, curveData), sizeof(hh::dv::DvElementModelFade::Data::curveData)}},
        {hh::dv::DvNodeElement::ElementID::CAMERA_HEDGEHOG, {offsetof(hh::dv::DvElementCameraHedgehog::Data, curveData), sizeof(hh::dv::DvElementCameraHedgehog::Data::curveData)}},
        {hh::dv::DvNodeElement::ElementID::SPOTLIGHT, {offsetof(hh::dv::DvElementSpotlight::Data, curveData), sizeof(hh::dv::DvElementSpotlight::Data::curveData)}},
#ifdef DEVTOOLS_TARGET_SDK_miller
        {hh::dv::DvNodeElement::ElementID::POINT_LIGHT, {offsetof(hh::dv::DvElementPointLight::Data, curveData), sizeof(hh::dv::DvElementPointLight::Data::curveData)}},
        {hh::dv::DvNodeElement::ElementID::MATERIAL_PARAM, {offsetof(hh::dv::DvElementMaterialParam::Data, curveData), sizeof(hh::dv::DvElementMaterialParam::Data::curveData)}},
#endif
        {hh::dv::DvNodeElement::ElementID::BLOOM_PARAM, {offsetof(app::dv::DvElementBloomParam::Data, curveData), sizeof(app::dv::DvElementBloomParam::Data::curveData)}},
        {hh::dv::DvNodeElement::ElementID::DOF_PARAM, {offsetof(app::dv::DvElementDOFParam::Data, curveData), sizeof(app::dv::DvElementDOFParam::Data::curveData)}},
        {hh::dv::DvNodeElement::ElementID::COLOR_CONTRAST, {offsetof(app::dv::DvElementColorContrast::Data, curveData), sizeof(app::dv::DvElementColorContrast::Data::curveData)}},
#ifdef DEVTOOLS_TARGET_SDK_rangers
        {hh::dv::DvNodeElement::ElementID::CAMERA_CONTROL_PARAM, {offsetof(app::dv::DvElementCameraControlParam::Data, curveData), sizeof(app::dv::DvElementCameraControlParam::Data::curveData)}},
#endif
        {hh::dv::DvNodeElement::ElementID::GODRAY_PARAM, {offsetof(app::dv::DvElementGodrayParam::Data, curveData), sizeof(app::dv::DvElementGodrayParam::Data::curveData)}},
        {hh::dv::DvNodeElement::ElementID::CHROMATIC_ABERRATION_FILTER_PARAM, {offsetof(app::dv::DvElementChromaticAberrationFilterParam::Data, curveData), sizeof(app::dv::DvElementChromaticAberrationFilterParam::Data::curveData)}},
        {hh::dv::DvNodeElement::ElementID::VIGNETTE_PARAM, {offsetof(app::dv::DvElementVignetteParam::Data, curveData), sizeof(app::dv::DvElementVignetteParam::Data::curveData)}},
        {hh::dv::DvNodeElement::ElementID::FADE, {offsetof(app::dv::DvElementFade::Data, curveData), sizeof(app::dv::DvElementFade::Data::curveData)}},
        {hh::dv::DvNodeElement::ElementID::LETTER_BOX, {offsetof(app::dv::DvElementLetterBox::Data, curveData), sizeof(app::dv::DvElementLetterBox::Data::curveData)}},
        {hh::dv::DvNodeElement::ElementID::TIME, {offsetof(app::dv::DvElementTime::Data, curveData), sizeof(app::dv::DvElementTime::Data::curveData)}},
        {hh::dv::DvNodeElement::ElementID::SUN, {offsetof(app::dv::DvElementSun::Data, curveData), sizeof(app::dv::DvElementSun::Data::curveData)}},
        {hh::dv::DvNodeElement::ElementID::LOOK_AT_IK, {offsetof(app::dv::DvElementLookAtIK::Data, curveData), sizeof(app::dv::DvElementLookAtIK::Data::curveData)}},
        {hh::dv::DvNodeElement::ElementID::CAMERA_BLUR_PARAM, {offsetof(app::dv::DvElementCameraBlurParam::Data, curveData), sizeof(app::dv::DvElementCameraBlurParam::Data::curveData)}},
        {hh::dv::DvNodeElement::ElementID::FACIAL_ANIMATION, {offsetof(app::dv::DvElementFacialAnimation::Data, curveData), sizeof(app::dv::DvElementFacialAnimation::Data::curveData)}},
        {hh::dv::DvNodeElement::ElementID::AURA, {offsetof(app::dv::DvElementAura::Data, curveData), sizeof(app::dv::DvElementAura::Data::curveData)}},
        {hh::dv::DvNodeElement::ElementID::CYBER_SPACE_NOISE, {offsetof(app::dv::DvElementCyberSpaceNoise::Data, curveData), sizeof(app::dv::DvElementCyberSpaceNoise::Data::curveData)}},
        {hh::dv::DvNodeElement::ElementID::CROSS_FADE, {offsetof(app::dv::DvElementCrossFade::Data, curveData), sizeof(app::dv::DvElementCrossFade::Data::curveData)}},
        {hh::dv::DvNodeElement::ElementID::WEATHER, {offsetof(app::dv::DvElementWeather::Data, curveData), sizeof(app::dv::DvElementWeather::Data::curveData)}},
        {hh::dv::DvNodeElement::ElementID::VARIABLE_POINT_LIGHT, {offsetof(app::dv::DvElementVariablePointLight::Data, curveData), sizeof(app::dv::DvElementVariablePointLight::Data::curveData)}},
#ifdef DEVTOOLS_TARGET_SDK_rangers
        {hh::dv::DvNodeElement::ElementID::THE_END_CABLE_OBJECT, {offsetof(app::dv::DvElementTheEndCableObject::Data, curveData), sizeof(app::dv::DvElementTheEndCableObject::Data::curveData)}},
#elif DEVTOOLS_TARGET_SDK_miller
        {hh::dv::DvNodeElement::ElementID::FOG, {offsetof(app::dv::DvElementFog::Data, curveData), sizeof(app::dv::DvElementFog::Data::curveData)}},
        {hh::dv::DvNodeElement::ElementID::DOF, {offsetof(app::dv::DvElementDOF::Data, curveData), sizeof(app::dv::DvElementDOF::Data::curveData)}},
#endif
    };

    constexpr std::pair<size_t, size_t> GetElementTimelineRender(hh::dv::DvNodeElement::ElementID type) {
        for (const auto& [key, func] : RenderTimelineElements) {
            if (key == type) return func;
        }
        return {};
    }
}
