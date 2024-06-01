#include <ui/game-services/game-service-inspectors/FxParamManager.h>

template<typename Rfl> static void RenderFxParamManagerInterpolatorInspector(const char* name, app::gfx::FxParamManager::InterpolatorT<Rfl>& interpolator) {
    if (ImGui::TreeNode(name)) {
        ImGui::Text("Enabled group bits: %x", interpolator.interpolationGroupEnabledBits);

        unsigned short i{ 0 };
        for (auto& interpolationJob : interpolator.interpolationJobs) {
            char nodename[200];
            snprintf(nodename, sizeof(nodename), "Job %d", i++);

            if (ImGui::TreeNode(nodename)) {
                ImGui::Text("Active on interpolation groups %x", interpolationJob.interpolationGroupMemberBits);
                ImGui::Text("Owner hash: %zx", interpolationJob.ownerId);
                ImGui::Text("Priority: %d", interpolationJob.priority);
                Editor("Unk6", interpolationJob.unk6);
                ImGui::Text("Tween duration: %f", 1 / interpolationJob.tweenPositionIncrementPerSecond);
                ImGui::SliderFloat("Current time", &interpolationJob.currentTweenPosition, 0.0f, 1.0f);
                ImGui::SeparatorText("Target parameters");
                Editor("Target parameters", interpolationJob.parameters);
                ImGui::TreePop();
            }
        }
        ImGui::TreePop();
    }
}

void RenderGameServiceInspector(app::gfx::FxParamManager& service) {
	ImGui::SeparatorText("Interpolated parameters");
	Editor("NeedleFxParameter", service.parameters);

	ImGui::SeparatorText("Interpolators");

	if (ImGui::TreeNode("NeedleFXParameter")) {
        RenderFxParamManagerInterpolatorInspector("FxBloomParameter", *service.paramInterpolators.bloomInterpolator);
        RenderFxParamManagerInterpolatorInspector("FxDOFParameter", *service.paramInterpolators.dofInterpolator);
        //RenderFxParamManagerInterpolatorInspector:Enum<hh::NeedleFxParameter::ToneMapType, signed char>>("ToneMapType", *service.paramInterpolators.toneMapTypeInterpolator);
        //RenderFxParamManagerInterpolatorInspector:Enum<hh::NeedleFxParameter::ExposureMode, signed char>>("ExposureMode", *service.paramInterpolators.exposureModeInterpolator);
        RenderFxParamManagerInterpolatorInspector("FxColorContrastParameter", *service.paramInterpolators.colorContrastInterpolator);
        RenderFxParamManagerInterpolatorInspector("FxToneMapParameter", *service.paramInterpolators.toneMapInterpolator);
        RenderFxParamManagerInterpolatorInspector("FxExposureParameter", *service.paramInterpolators.exposureInterpolator);
        RenderFxParamManagerInterpolatorInspector("FxShadowMapParameter", *service.paramInterpolators.shadowMapInterpolator);
        RenderFxParamManagerInterpolatorInspector("FxScreenBlurParameter", *service.paramInterpolators.blurInterpolator);
        RenderFxParamManagerInterpolatorInspector("FxSSAOParameter", *service.paramInterpolators.ssaoInterpolator);
        RenderFxParamManagerInterpolatorInspector("FxLightFieldParameter", *service.paramInterpolators.lightFieldInterpolator);
        RenderFxParamManagerInterpolatorInspector("FxSHLightFieldParameter", *service.paramInterpolators.shLightFieldInterpolator);
        RenderFxParamManagerInterpolatorInspector("FxLightScatteringParameter", *service.paramInterpolators.lightScatteringInterpolator);
        RenderFxParamManagerInterpolatorInspector("FxRLRParameter", *service.paramInterpolators.rlrInterpolator);
        RenderFxParamManagerInterpolatorInspector("FxOcclusionCapsuleParameter", *service.paramInterpolators.occlusionCapsuleInterpolator);
        RenderFxParamManagerInterpolatorInspector("FxGodrayParameter", *service.paramInterpolators.godrayInterpolator);
        RenderFxParamManagerInterpolatorInspector("FxScreenSpaceGodrayParameter", *service.paramInterpolators.ssGodrayInterpolator);
        RenderFxParamManagerInterpolatorInspector("FxHeatHazeParameter", *service.paramInterpolators.heatHazeInterpolator);
        RenderFxParamManagerInterpolatorInspector("FxSceneEnvironmentParameter", *service.paramInterpolators.sceneEnvironmentInterpolator);
        RenderFxParamManagerInterpolatorInspector("FxRenderOption", *service.paramInterpolators.renderOptionInterpolator);
        RenderFxParamManagerInterpolatorInspector("FxSGGIParameter", *service.paramInterpolators.sggiInterpolator);
        RenderFxParamManagerInterpolatorInspector("FxTAAParameter", *service.paramInterpolators.taaInterpolator);
        RenderFxParamManagerInterpolatorInspector("FxEffectParameter", *service.paramInterpolators.effectInterpolator);
		ImGui::TreePop();
	}
}
