#pragma once
#include "Timeline.h"

namespace ui::operation_modes::modes::dvscene_editor {
    using namespace hh::dv;
    using namespace app::dv;
    using ElementID = DvNodeElement::ElementID;

    constexpr std::pair<ElementID, std::pair<size_t, size_t>> RenderTimelineElements[] = {
        {ElementID::CAMERA_PARAMS,                      {offsetof(DvElementCameraParams::Data, curveData),                      sizeof(DvElementCameraParams::Data::curveData)}},
        {ElementID::CAMERA_SHAKE_LOOP,                  {offsetof(DvElementCameraShakeLoop::Data, curveData),                   sizeof(DvElementCameraShakeLoop::Data::curveData)}},
        {ElementID::EFFECT,                             {offsetof(DvElementEffect::Data, curveData),                            sizeof(DvElementEffect::Data::curveData)}},
        {ElementID::PATH_INTERPOLATION,                 {offsetof(DvElementPathInterpolation::Data, curveData),                 sizeof(DvElementPathInterpolation::Data::curveData)}},
        {ElementID::CAMERA_OFFSET,                      {offsetof(DvElementCameraOffset::Data, curveData),                      sizeof(DvElementCameraOffset::Data::curveData)}},
        {ElementID::MODEL_FADE,                         {offsetof(DvElementModelFade::Data, curveData),                         sizeof(DvElementModelFade::Data::curveData)}},
        {ElementID::CAMERA_HEDGEHOG,                    {offsetof(DvElementCameraHedgehog::Data, curveData),                    sizeof(DvElementCameraHedgehog::Data::curveData)}},
        {ElementID::SPOTLIGHT,                          {offsetof(DvElementSpotlight::Data, curveData),                         sizeof(DvElementSpotlight::Data::curveData)}},
#ifdef DEVTOOLS_TARGET_SDK_miller
        {ElementID::POINT_LIGHT,                        {offsetof(DvElementPointLight::Data, curveData),                        sizeof(DvElementPointLight::Data::curveData)}},
        {ElementID::MATERIAL_PARAM,                     {offsetof(DvElementMaterialParam::Data, curveData),                     sizeof(DvElementMaterialParam::Data::curveData)}},
#endif
        {ElementID::BLOOM_PARAM,                        {offsetof(DvElementBloomParam::Data, curveData),                        sizeof(DvElementBloomParam::Data::curveData)}},
        {ElementID::DOF_PARAM,                          {offsetof(DvElementDOFParam::Data, curveData),                          sizeof(DvElementDOFParam::Data::curveData)}},
        {ElementID::COLOR_CONTRAST,                     {offsetof(DvElementColorContrast::Data, curveData),                     sizeof(DvElementColorContrast::Data::curveData)}},
#ifdef DEVTOOLS_TARGET_SDK_rangers
        {ElementID::CAMERA_CONTROL_PARAM,               {offsetof(DvElementCameraControlParam::Data, curveData),                sizeof(DvElementCameraControlParam::Data::curveData)}},
#endif
        {ElementID::GODRAY_PARAM,                       {offsetof(DvElementGodrayParam::Data, curveData),                       sizeof(DvElementGodrayParam::Data::curveData)}},
        {ElementID::CHROMATIC_ABERRATION_FILTER_PARAM,  {offsetof(DvElementChromaticAberrationFilterParam::Data, curveData),    sizeof(DvElementChromaticAberrationFilterParam::Data::curveData)}},
        {ElementID::VIGNETTE_PARAM,                     {offsetof(DvElementVignetteParam::Data, curveData),                     sizeof(DvElementVignetteParam::Data::curveData)}},
        {ElementID::FADE,                               {offsetof(DvElementFade::Data, curveData),                              sizeof(DvElementFade::Data::curveData)}},
        {ElementID::LETTER_BOX,                         {offsetof(DvElementLetterBox::Data, curveData),                         sizeof(DvElementLetterBox::Data::curveData)}},
        {ElementID::TIME,                               {offsetof(DvElementTime::Data, curveData),                              sizeof(DvElementTime::Data::curveData)}},
        {ElementID::SUN,                                {offsetof(DvElementSun::Data, curveData),                               sizeof(DvElementSun::Data::curveData)}},
        {ElementID::LOOK_AT_IK,                         {offsetof(DvElementLookAtIK::Data, curveData),                          sizeof(DvElementLookAtIK::Data::curveData)}},
        {ElementID::CAMERA_BLUR_PARAM,                  {offsetof(DvElementCameraBlurParam::Data, curveData),                   sizeof(DvElementCameraBlurParam::Data::curveData)}},
        {ElementID::FACIAL_ANIMATION,                   {offsetof(DvElementFacialAnimation::Data, curveData),                   sizeof(DvElementFacialAnimation::Data::curveData)}},
        {ElementID::AURA,                               {offsetof(DvElementAura::Data, curveData),                              sizeof(DvElementAura::Data::curveData)}},
        {ElementID::CYBER_SPACE_NOISE,                  {offsetof(DvElementCyberSpaceNoise::Data, curveData),                   sizeof(DvElementCyberSpaceNoise::Data::curveData)}},
        {ElementID::CROSS_FADE,                         {offsetof(DvElementCrossFade::Data, curveData),                         sizeof(DvElementCrossFade::Data::curveData)}},
        {ElementID::WEATHER,                            {offsetof(DvElementWeather::Data, curveData),                           sizeof(DvElementWeather::Data::curveData)}},
        {ElementID::VARIABLE_POINT_LIGHT,               {offsetof(DvElementVariablePointLight::Data, curveData),                sizeof(DvElementVariablePointLight::Data::curveData)}},
#ifdef DEVTOOLS_TARGET_SDK_rangers
        {ElementID::THE_END_CABLE_OBJECT,               {offsetof(DvElementTheEndCableObject::Data, curveData),                 sizeof(DvElementTheEndCableObject::Data::curveData)}},
#elif DEVTOOLS_TARGET_SDK_miller
        {ElementID::FOG,                                {offsetof(DvElementFog::Data, curveData),                               sizeof(DvElementFog::Data::curveData)}},
        {ElementID::DOF,                                {offsetof(DvElementDOF::Data, curveData),                               sizeof(DvElementDOF::Data::curveData)}},
#endif
    };

    constexpr std::pair<size_t, size_t> GetElementTimelineRender(ElementID type) {
        for (const auto& [key, func] : RenderTimelineElements) {
            if (key == type) return func;
        }
        return {};
    }
}
