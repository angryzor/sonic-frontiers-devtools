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

    constexpr std::pair<int, ElementFuncType> RenderElementInspectors[] = {
        {1, RenderElementInspector<1>},
        {3, RenderElementInspector<3>},
        {5, RenderElementInspector<5>},
        {6, RenderElementInspector<6>},
        {7, RenderElementInspector<7>},
        {8, RenderElementInspector<8>},
        {10, RenderElementInspector<10>},
        {12, RenderElementInspector<12>},
        {13, RenderElementInspector<13>},
        {14, RenderElementInspector<14>},
        {15, RenderElementInspector<15>},
        {16, RenderElementInspector<16>},
        {17, RenderElementInspector<17>},
        {19, RenderElementInspector<16>},
        {20, RenderElementInspector<20>},
        {21, RenderElementInspector<21>},
        {23, RenderElementInspector<23>},
        {24, RenderElementInspector<24>},
        {25, RenderElementInspector<25>},
#ifdef DEVTOOLS_TARGET_SDK_miller
        {22, RenderElementInspector<22>},
        {26, RenderElementInspector<26>},
        {27, RenderElementInspector<27>},
#endif
        {1000, RenderElementInspector<1000>},
        {1001, RenderElementInspector<1001>},
        {1002, RenderElementInspector<1002>},
        {1003, RenderElementInspector<1003>},
        {1004, RenderElementInspector<1004>},
        {1005, RenderElementInspector<1005>},
        {1006, RenderElementInspector<1006>},
        {1008, RenderElementInspector<1008>},
#ifdef DEVTOOLS_TARGET_SDK_rangers
        {1009, RenderElementInspector<1009>},
        {1014, RenderElementInspector<1014>},
        {1015, RenderElementInspector<1015>},
        {1024, RenderElementInspector<1024>},
        {1033, RenderElementInspector<1033>},
        {1034, RenderElementInspector<1034>},
#endif
        {1010, RenderElementInspector<1010>},
#ifdef DEVTOOLS_TARGET_SDK_miller
        {1011, RenderElementInspector<1011>},
        {1012, RenderElementInspector<1012>},
#endif
        {1016, RenderElementInspector<1016>},
        {1017, RenderElementInspector<1017>},
        {1018, RenderElementInspector<1018>},
        {1019, RenderElementInspector<1019>},
        {1020, RenderElementInspector<1020>},
        {1021, RenderElementInspector<1021>},
#ifdef DEVTOOLS_TARGET_SDK_miller
        {1022, RenderElementInspector<1022>},
#endif
        {1023, RenderElementInspector<1023>},
        {1025, RenderElementInspector<1025>},
        {1026, RenderElementInspector<1026>},
        {1027, RenderElementInspector<1027>},
        {1028, RenderElementInspector<1028>},
        {1030, RenderElementInspector<1030>},
        {1035, RenderElementInspector<1035>},
        {1036, RenderElementInspector<1036>},
        {1038, RenderElementInspector<1038>},
        {1040, RenderElementInspector<1040>},
        {1041, RenderElementInspector<1041>},
        {1042, RenderElementInspector<1042>},
#ifdef DEVTOOLS_TARGET_SDK_miller
        {1043, RenderElementInspector<1043>},
        {1044, RenderElementInspector<1044>},
        {1045, RenderElementInspector<1045>},
        {1046, RenderElementInspector<1046>},
        {1047, RenderElementInspector<1047>}
#endif
    };

    constexpr ElementFuncType GetElementInspectorRender(int type) {
        for (const auto& [key, func] : RenderElementInspectors) {
            if (key == type) return func;
        }
        return nullptr;
    }
}