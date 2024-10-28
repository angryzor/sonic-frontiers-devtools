#include <ui/game-services/game-service-inspectors/FxParamManager.h>
#include <ui/common/editors/Reflection.h>

template<typename Rfl> static void RenderFxParamManagerInterpolatorInspector(const char* name, app::gfx::FxParamManager::Interpolator<Rfl>& interpolator) {
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
	ImGui::SeparatorText("SceneParameters");
	ImGui::Text("Current: %d", service.currentSceneParameters);
	ImGui::Text("SceneParameters list:");

	for (size_t i = 0; i < 2; i++) {
		if (auto* sceneParameters = service.sceneParameters[i]) {
			if (ImGui::TreeNode(sceneParameters, "%d", i)) {
				Editor("Scene data", *sceneParameters->GetSceneData());
				ImGui::TreePop();
			}
		}
	}

	ImGui::SeparatorText("Interpolated parameters");
	Editor("NeedleFxParameter", service.parameters);
	Editor("NeedleFxSceneConfig", service.sceneConfig);

	ImGui::SeparatorText("Interpolators");

	if (ImGui::TreeNode("NeedleFXParameter")) {
		RenderFxParamManagerInterpolatorInspector("FxBloomParameter", *service.paramInterpolators.bloomInterpolator);
		RenderFxParamManagerInterpolatorInspector("FxDOFParameter", *service.paramInterpolators.dofInterpolator);
		RenderFxParamManagerInterpolatorInspector("FxColorContrastParameter", *service.paramInterpolators.colorContrastInterpolator);
		RenderFxParamManagerInterpolatorInspector("FxToneMapParameter", *service.paramInterpolators.tonemapInterpolator);
		RenderFxParamManagerInterpolatorInspector("FxCameraControlParameter", *service.paramInterpolators.cameraControlInterpolator);
		RenderFxParamManagerInterpolatorInspector("FxShadowMapParameter", *service.paramInterpolators.shadowmapInterpolator);
		RenderFxParamManagerInterpolatorInspector("FxShadowHeightMapParameter", *service.paramInterpolators.shadowHeightMapInterpolator);
		RenderFxParamManagerInterpolatorInspector("FxVolumetricShadowParameter", *service.paramInterpolators.volShadowInterpolator);
		RenderFxParamManagerInterpolatorInspector("FxScreenBlurParameter", *service.paramInterpolators.blurInterpolator);
		RenderFxParamManagerInterpolatorInspector("FxSSAOParameter", *service.paramInterpolators.ssaoInterpolator);
		RenderFxParamManagerInterpolatorInspector("FxSHLightFieldParameter", *service.paramInterpolators.shlightfieldInterpolator);
		RenderFxParamManagerInterpolatorInspector("FxLightScatteringParameter", *service.paramInterpolators.lightscatteringInterpolator);
		RenderFxParamManagerInterpolatorInspector("FxFogParameter", *service.paramInterpolators.fogInterpolator);
		RenderFxParamManagerInterpolatorInspector("FxRLRParameter", *service.paramInterpolators.rlrInterpolator);
		RenderFxParamManagerInterpolatorInspector("FxSSGIParameter", *service.paramInterpolators.ssgiInterpolator);
		RenderFxParamManagerInterpolatorInspector("FxPlanarReflectionParameter", *service.paramInterpolators.planarReflectionInterpolator);
		RenderFxParamManagerInterpolatorInspector("FxOcclusionCapsuleParameter", *service.paramInterpolators.occlusionCapsuleInterpolator);
		RenderFxParamManagerInterpolatorInspector("FxGodrayParameter", *service.paramInterpolators.godrayInterpolator);
		RenderFxParamManagerInterpolatorInspector("FxScreenSpaceGodrayParameter", *service.paramInterpolators.ssGodrayInterpolator);
		RenderFxParamManagerInterpolatorInspector("FxHeatHazeParameter", *service.paramInterpolators.heatHazeInterpolator);
		RenderFxParamManagerInterpolatorInspector("FxSceneEnvironmentParameter", *service.paramInterpolators.sceneEnvInterpolator);
		RenderFxParamManagerInterpolatorInspector("FxRenderOption", *service.paramInterpolators.renderOptionInterpolator);
		RenderFxParamManagerInterpolatorInspector("FxSGGIParameter", *service.paramInterpolators.sggiInterpolator);
		RenderFxParamManagerInterpolatorInspector("FxTAAParameter", *service.paramInterpolators.taaInterpolator);
		RenderFxParamManagerInterpolatorInspector("FxEffectParameter", *service.paramInterpolators.effectInterpolator);
		RenderFxParamManagerInterpolatorInspector("FxAtmosphereParameter", *service.paramInterpolators.atmosphereInterpolator);
		RenderFxParamManagerInterpolatorInspector("FxDensityParameter", *service.paramInterpolators.densityInterpolator);
		RenderFxParamManagerInterpolatorInspector("FxWindComputeParameter", *service.paramInterpolators.windInterpolator);
		RenderFxParamManagerInterpolatorInspector("FxGpuEnvironmentParameter", *service.paramInterpolators.gpuEnvironmentInterpolator);
		RenderFxParamManagerInterpolatorInspector("FxInteractiveWaveParameter", *service.paramInterpolators.interactiveWaveInterpolator);
		RenderFxParamManagerInterpolatorInspector("FxChromaticAberrationParameter", *service.paramInterpolators.chromaticAberrationInterpolator);
		RenderFxParamManagerInterpolatorInspector("FxVignetteParameter", *service.paramInterpolators.vignetteInterpolator);
		RenderFxParamManagerInterpolatorInspector("FxTerrainMaterialBlendingParameter", *service.paramInterpolators.terrainBlendInterpolator);
		RenderFxParamManagerInterpolatorInspector("FxWeatherParameter", *service.paramInterpolators.weatherInterpolator);
		RenderFxParamManagerInterpolatorInspector("FxColorAccessibilityFilterParameter", *service.paramInterpolators.colorAccessibilityInterpolator);
		RenderFxParamManagerInterpolatorInspector("FxCyberNoiseEffectParameter", *service.paramInterpolators.cyberNoiseInterpolator);
		RenderFxParamManagerInterpolatorInspector("FxCyberSpaceStartNoiseParameter", *service.paramInterpolators.cyberStartNoiseInterpolator);
		RenderFxParamManagerInterpolatorInspector("FxCyberNPCSSEffectRenderParameter", *service.paramInterpolators.cyberNPCSSInterpolator);
		RenderFxParamManagerInterpolatorInspector("FxDentParameter", *service.paramInterpolators.dentInterpolator);
		RenderFxParamManagerInterpolatorInspector("FxFieldScanEffectRenderParameter", *service.paramInterpolators.fieldScanInterpolator);
		RenderFxParamManagerInterpolatorInspector("FxSeparableSSSParameter", *service.paramInterpolators.ssssInterpolator);
		RenderFxParamManagerInterpolatorInspector("FxTimeStopParameter", *service.paramInterpolators.timeStopInterpolator);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("NeedleFXSceneConfig")) {
		RenderFxParamManagerInterpolatorInspector("FxRenderTargetSetting", *service.sceneConfigInterpolators.fxRenderTargetSettingInterpolator);
		RenderFxParamManagerInterpolatorInspector("FxAntiAliasing", *service.sceneConfigInterpolators.fxAntiAliasingInterpolator);
		RenderFxParamManagerInterpolatorInspector("FxLODParameter", *service.sceneConfigInterpolators.fxLODParameterInterpolator);
		RenderFxParamManagerInterpolatorInspector("FxDetailParameter", *service.sceneConfigInterpolators.fxDetailParameterInterpolator);
		RenderFxParamManagerInterpolatorInspector("FxDynamicResolutionParameter", *service.sceneConfigInterpolators.fxDynamicResolutionParameterInterpolator);
		RenderFxParamManagerInterpolatorInspector("StageCommonTimeProgressParameter", *service.sceneConfigInterpolators.stageCommonTimeProgressParameterInterpolator);
		RenderFxParamManagerInterpolatorInspector("FxModelParameter", *service.sceneConfigInterpolators.fxModelParameterInterpolator);
		RenderFxParamManagerInterpolatorInspector("PerformanceSetting", *service.sceneConfigInterpolators.performanceSettingInterpolator);
		ImGui::TreePop();
	}

	//ImGui::SeparatorText("Extensions");
	//for (auto* extension : service.extensions) {
	//	ImGui::Text("%zx", extension->GetRuntimeTypeInfo());
	//}
}
