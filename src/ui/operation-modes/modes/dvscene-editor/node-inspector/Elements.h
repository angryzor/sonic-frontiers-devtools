#pragma once
#include "NodeInspector.h"
#include <ui/common/viewers/Basic.h>
#include <ui/common/editors/Basic.h>
#include <ui/common/editors/DvScene.h>

#include "elements/CameraParams.h"
#include "elements/DrawOff.h"
#include "elements/PathOffset.h"
#include "elements/CameraShake.h"
#include "elements/CameraShakeLoop.h"
#include "elements/Effect.h"
#include "elements/PathInterpolation.h"
#include "elements/CameraNearFar.h"
#include "elements/UVAnim.h"
#include "elements/VisibilityAnim.h"
#include "elements/MaterialAnim.h"
#include "elements/MultipleAnim.h"
#include "elements/CameraOffset.h"
//#include "elements/DebugMotion.h"
#include "elements/CameraHedgehog.h"
#include "elements/CameraInGame.h"
#include "elements/VertexAnimationTexture.h"
#include "elements/Spotlight.h"
#include "elements/ControllerVibration.h"
//#include "elements/SpotlightModel.h"
#ifdef DEVTOOLS_TARGET_SDK_miller
#include "elements/PointLight.h"
#include "elements/TexturePatternAnim.h"
#include "elements/MaterialParam.h"
#endif
#include "elements/BloomParam.h"
#include "elements/DOFParam.h"
#include "elements/ColorContrast.h"
#include "elements/CameraControlParam.h"
#include "elements/ShadowResolution.h"
#ifdef DEVTOOLS_TARGET_SDK_miller
#include "elements/SSAOParam.h"
#include "elements/OcclusionCapsuleParam.h"
#endif
#include "elements/GodrayParam.h"
#include "elements/AtmosphereGodrayParam.h"
//#include "elements/AtmosphereHeightFogParam.h"
#include "elements/ChromaticAberrationFilterParam.h"
#include "elements/VignetteParam.h"
#include "elements/Fade.h"
#include "elements/BossName.h"
#include "elements/Caption.h"
#include "elements/Sound.h"
#include "elements/Time.h"
#include "elements/Sun.h"
#include "elements/LookAtIK.h"
#include "elements/CameraBlurParam.h"
#include "elements/GeneralTrigger.h"
//#include "elements/FootIk.h"
#include "elements/DitherParam.h"
#include "elements/QTE.h"
#include "elements/FacialAnimation.h"
#include "elements/OverrideASM.h"
#include "elements/Aura.h"
#include "elements/ChangeTimeScale.h"
#include "elements/LipAnimation.h"
#include "elements/CrossFade.h"
#include "elements/Weather.h"
#include "elements/ShadowMapParam.h"
#include "elements/VariablePointLight.h"
#include "elements/DensitySectorPoint.h"
#ifdef DEVTOOLS_TARGET_SDK_rangers
#include "elements/VAT.h"
#include "elements/QTEAccel.h"
#include "elements/TheEndCableObject.h"
#endif
#ifdef DEVTOOLS_TARGET_SDK_miller
#include "elements/BulletTime.h"
#include "elements/TimeStop.h"
#include "elements/ObjectTimeStop.h"
#include "elements/ShadowShape.h"
#include "elements/Falloff.h"
#include "elements/Fog.h"
#include "elements/DOF.h"
#endif


namespace ui::operation_modes::modes::dvscene_editor {
    using ElementFuncType = bool(*)(char*);
    using ElementID = hh::dv::DvNodeElement::ElementID;

    constexpr std::pair<ElementID, ElementFuncType> RenderElementInspectors[] = {
        {ElementID::CAMERA_PARAMS,                      RenderElementInspector<ElementID::CAMERA_PARAMS>},
        {ElementID::DRAW_OFF,                           RenderElementInspector<ElementID::DRAW_OFF>},
        {ElementID::PATH_OFFSET,                        RenderElementInspector<ElementID::PATH_OFFSET>},
        {ElementID::CAMERA_SHAKE,                       RenderElementInspector<ElementID::CAMERA_SHAKE>},
        {ElementID::CAMERA_SHAKE_LOOP,                  RenderElementInspector<ElementID::CAMERA_SHAKE_LOOP>},
        {ElementID::EFFECT,                             RenderElementInspector<ElementID::EFFECT>},
        {ElementID::PATH_INTERPOLATION,                 RenderElementInspector<ElementID::PATH_INTERPOLATION>},
        {ElementID::CAMERA_NEAR_FAR,                    RenderElementInspector<ElementID::CAMERA_NEAR_FAR>},
        {ElementID::UV_ANIM,                            RenderElementInspector<ElementID::UV_ANIM>},
        {ElementID::VISIBILITY_ANIM,                    RenderElementInspector<ElementID::VISIBILITY_ANIM>},
        {ElementID::MATERIAL_ANIM,                      RenderElementInspector<ElementID::MATERIAL_ANIM>},
        {ElementID::MULTIPLE_ANIM,                      RenderElementInspector<ElementID::MULTIPLE_ANIM>},
        {ElementID::CAMERA_OFFSET,                      RenderElementInspector<ElementID::CAMERA_OFFSET>},
        {ElementID::DEBUG_MOTION,                       RenderElementInspector<ElementID::MULTIPLE_ANIM>},
        {ElementID::CAMERA_HEDGEHOG,                    RenderElementInspector<ElementID::CAMERA_HEDGEHOG>},
        {ElementID::CAMERA_IN_GAME,                     RenderElementInspector<ElementID::CAMERA_IN_GAME>},
        {ElementID::VERTEX_ANIMATION_TEXTURE,           RenderElementInspector<ElementID::VERTEX_ANIMATION_TEXTURE>},
        {ElementID::SPOTLIGHT,                          RenderElementInspector<ElementID::SPOTLIGHT>},
        {ElementID::CONTROLLER_VIBRATION,               RenderElementInspector<ElementID::CONTROLLER_VIBRATION>},
#ifdef DEVTOOLS_TARGET_SDK_miller
        {ElementID::POINT_LIGHT,                        RenderElementInspector<ElementID::POINT_LIGHT>},
        {ElementID::TEXTURE_PATTERN_ANIM,               RenderElementInspector<ElementID::TEXTURE_PATTERN_ANIM>},
        {ElementID::MATERIAL_PARAM,                     RenderElementInspector<ElementID::MATERIAL_PARAM>},
#endif
        {ElementID::BLOOM_PARAM,                        RenderElementInspector<ElementID::BLOOM_PARAM>},
        {ElementID::DOF_PARAM,                          RenderElementInspector<ElementID::DOF_PARAM>},
        {ElementID::COLOR_CONTRAST,                     RenderElementInspector<ElementID::COLOR_CONTRAST>},
#ifdef DEVTOOLS_TARGET_SDK_rangers
        {ElementID::CAMERA_CONTROL_PARAM,               RenderElementInspector<ElementID::CAMERA_CONTROL_PARAM>},
#elif DEVTOOLS_TARGET_SDK_miller
        {ElementID::CAMERA_CONTROL,                     RenderElementInspector<ElementID::CAMERA_CONTROL>},
#endif
        {ElementID::SHADOW_RESOLUTION,                  RenderElementInspector<ElementID::SHADOW_RESOLUTION>},
#ifdef DEVTOOLS_TARGET_SDK_miller
        {ElementID::SSAO_PARAM,                         RenderElementInspector<ElementID::SSAO_PARAM>},
        {ElementID::OCCLUSION_CAPSULE_PARAM,            RenderElementInspector<ElementID::OCCLUSION_CAPSULE_PARAM>},
#endif
        {ElementID::GODRAY_PARAM,                       RenderElementInspector<ElementID::GODRAY_PARAM>},
        {ElementID::ATMOSPHERE_GODRAY_PARAM,            RenderElementInspector<ElementID::ATMOSPHERE_GODRAY_PARAM>},
        {ElementID::CHROMATIC_ABERRATION_FILTER_PARAM,  RenderElementInspector<ElementID::CHROMATIC_ABERRATION_FILTER_PARAM>},
        {ElementID::FADE,                               RenderElementInspector<ElementID::FADE>},
        {ElementID::VIGNETTE_PARAM,                     RenderElementInspector<ElementID::VIGNETTE_PARAM>},
        {ElementID::BOSS_NAME,                          RenderElementInspector<ElementID::BOSS_NAME>},
        {ElementID::CAPTION,                            RenderElementInspector<ElementID::CAPTION>},
        {ElementID::SOUND,                              RenderElementInspector<ElementID::SOUND>},
        {ElementID::TIME,                               RenderElementInspector<ElementID::TIME>},
        {ElementID::SUN,                                RenderElementInspector<ElementID::SUN>},
        {ElementID::LOOK_AT_IK,                         RenderElementInspector<ElementID::LOOK_AT_IK>},
        {ElementID::CAMERA_BLUR_PARAM,                  RenderElementInspector<ElementID::CAMERA_BLUR_PARAM>},
        {ElementID::GENERAL_TRIGGER,                    RenderElementInspector<ElementID::GENERAL_TRIGGER>},
        {ElementID::DITHER_PARAM,                       RenderElementInspector<ElementID::DITHER_PARAM>},
        {ElementID::QTE,                                RenderElementInspector<ElementID::QTE>},
        {ElementID::FACIAL_ANIMATION,                   RenderElementInspector<ElementID::FACIAL_ANIMATION>},
        {ElementID::OVERRIDE_ASM,                       RenderElementInspector<ElementID::OVERRIDE_ASM>},
        {ElementID::AURA,                               RenderElementInspector<ElementID::AURA>},
        {ElementID::CHANGE_TIME_SCALE,                  RenderElementInspector<ElementID::CHANGE_TIME_SCALE>},
        {ElementID::LIP_ANIMATION,                      RenderElementInspector<ElementID::LIP_ANIMATION>},
        {ElementID::CROSS_FADE,                         RenderElementInspector<ElementID::CROSS_FADE>},
        {ElementID::WEATHER,                            RenderElementInspector<ElementID::WEATHER>},
        {ElementID::SHADOW_MAP_PARAM,                   RenderElementInspector<ElementID::SHADOW_MAP_PARAM>},
        {ElementID::VARIABLE_POINT_LIGHT,               RenderElementInspector<ElementID::VARIABLE_POINT_LIGHT>},
        {ElementID::DENSITY_SECTOR_POINT,               RenderElementInspector<ElementID::DENSITY_SECTOR_POINT>},
#ifdef DEVTOOLS_TARGET_SDK_rangers
        {ElementID::VAT,                                RenderElementInspector<ElementID::VAT>},
        {ElementID::QTE_ACCEL,                          RenderElementInspector<ElementID::QTE_ACCEL>},
        {ElementID::THE_END_CABLE_OBJECT,               RenderElementInspector<ElementID::THE_END_CABLE_OBJECT>},
#elif DEVTOOLS_TARGET_SDK_miller
        {ElementID::BULLET_TIME,                        RenderElementInspector<ElementID::BULLET_TIME>},
        {ElementID::TIME_STOP,                          RenderElementInspector<ElementID::TIME_STOP>},
        {ElementID::OBJECT_TIME_STOP,                   RenderElementInspector<ElementID::OBJECT_TIME_STOP>},
        {ElementID::SHADOW_SHAPE,                       RenderElementInspector<ElementID::SHADOW_SHAPE>},
        {ElementID::FALLOFF,                            RenderElementInspector<ElementID::FALLOFF>},
        {ElementID::FOG,                                RenderElementInspector<ElementID::FOG>},
        {ElementID::DOF,                                RenderElementInspector<ElementID::DOF>},
#endif
    };

    constexpr ElementFuncType GetElementInspectorRender(ElementID type) {
        for (const auto& [key, func] : RenderElementInspectors) {
            if (key == type) return func;
        }
        return nullptr;
    }
}