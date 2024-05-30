#include "FxParamManager.h"

void RenderGameServiceInspector(app::gfx::FxParamManager& service) {
	ImGui::SeparatorText("Interpolated parameters");
	Editor("NeedleFxParameter", service.parameters);

	ImGui::SeparatorText("Interpolators");

	if (ImGui::TreeNode("NeedleFXParameter")) {
        RenderFxParamManagerInterpolatorInspector<hh::FxBloomParameter>("FxBloomParameter", *service.paramInterpolators.bloomInterpolator);
        RenderFxParamManagerInterpolatorInspector<hh::FxDOFParameter>("FxDOFParameter", *service.paramInterpolators.dofInterpolator);
        //RenderFxParamManagerInterpolatorInspector<csl::ut::Enum<hh::NeedleFxParameter::ToneMapType, signed char>>("ToneMapType", *service.paramInterpolators.toneMapTypeInterpolator);
        //RenderFxParamManagerInterpolatorInspector<csl::ut::Enum<hh::NeedleFxParameter::ExposureMode, signed char>>("ExposureMode", *service.paramInterpolators.exposureModeInterpolator);
        RenderFxParamManagerInterpolatorInspector<hh::FxColorContrastParameter>("FxColorContrastParameter", *service.paramInterpolators.colorContrastInterpolator);
        RenderFxParamManagerInterpolatorInspector<hh::FxToneMapParameter>("FxToneMapParameter", *service.paramInterpolators.toneMapInterpolator);
        RenderFxParamManagerInterpolatorInspector<hh::FxExposureParameter>("FxExposureParameter", *service.paramInterpolators.exposureInterpolator);
        RenderFxParamManagerInterpolatorInspector<hh::FxShadowMapParameter>("FxShadowMapParameter", *service.paramInterpolators.shadowMapInterpolator);
        RenderFxParamManagerInterpolatorInspector<hh::FxScreenBlurParameter>("FxScreenBlurParameter", *service.paramInterpolators.blurInterpolator);
        RenderFxParamManagerInterpolatorInspector<hh::FxSSAOParameter>("FxSSAOParameter", *service.paramInterpolators.ssaoInterpolator);
        RenderFxParamManagerInterpolatorInspector<hh::FxLightFieldParameter>("FxLightFieldParameter", *service.paramInterpolators.lightFieldInterpolator);
        RenderFxParamManagerInterpolatorInspector<hh::FxSHLightFieldParameter>("FxSHLightFieldParameter", *service.paramInterpolators.shLightFieldInterpolator);
        RenderFxParamManagerInterpolatorInspector<hh::FxLightScatteringParameter>("FxLightScatteringParameter", *service.paramInterpolators.lightScatteringInterpolator);
        RenderFxParamManagerInterpolatorInspector<hh::FxRLRParameter>("FxRLRParameter", *service.paramInterpolators.rlrInterpolator);
        RenderFxParamManagerInterpolatorInspector<hh::FxOcclusionCapsuleParameter>("FxOcclusionCapsuleParameter", *service.paramInterpolators.occlusionCapsuleInterpolator);
        RenderFxParamManagerInterpolatorInspector<hh::FxGodrayParameter>("FxGodrayParameter", *service.paramInterpolators.godrayInterpolator);
        RenderFxParamManagerInterpolatorInspector<hh::FxScreenSpaceGodrayParameter>("FxScreenSpaceGodrayParameter", *service.paramInterpolators.ssGodrayInterpolator);
        RenderFxParamManagerInterpolatorInspector<hh::FxHeatHazeParameter>("FxHeatHazeParameter", *service.paramInterpolators.heatHazeInterpolator);
        RenderFxParamManagerInterpolatorInspector<hh::FxSceneEnvironmentParameter>("FxSceneEnvironmentParameter", *service.paramInterpolators.sceneEnvironmentInterpolator);
        RenderFxParamManagerInterpolatorInspector<hh::FxRenderOption>("FxRenderOption", *service.paramInterpolators.renderOptionInterpolator);
        RenderFxParamManagerInterpolatorInspector<hh::FxSGGIParameter>("FxSGGIParameter", *service.paramInterpolators.sggiInterpolator);
        RenderFxParamManagerInterpolatorInspector<hh::FxTAAParameter>("FxTAAParameter", *service.paramInterpolators.taaInterpolator);
        RenderFxParamManagerInterpolatorInspector<hh::FxEffectParameter>("FxEffectParameter", *service.paramInterpolators.effectInterpolator);
		ImGui::TreePop();
	}
}
