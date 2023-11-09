#include "Pch.h"
#include "GameServiceInspector.h"

using namespace hh::game;

void GameServiceInspector::Render() {
	auto* gameManager = GameManager::GetInstance();

	const ImGuiWindowFlags windowFlags
		= ImGuiWindowFlags_MenuBar;

	ImGui::SetNextWindowPos(ImVec2(ImGui::GetMainViewport()->WorkSize.x, ImGui::GetMainViewport()->WorkSize.y - 30), ImGuiCond_Once, ImVec2(1, 0));
	ImGui::SetNextWindowSize(ImVec2(800, ImGui::GetMainViewport()->WorkSize.y - 100), ImGuiCond_Once);
	ImGui::SetNextWindowCollapsed(true, ImGuiCond_Once);
	if (ImGui::Begin("GameService inspector", NULL, windowFlags)) {
		for (auto* service : gameManager->GetServices()) {
			if (ImGui::CollapsingHeader(service->pStaticClass->pName)) {
				RenderServiceInspector(*service);
			}
		}
	}
	ImGui::End();
}

void GameServiceInspector::RenderServiceInspector(hh::game::GameService& service) {
	if (service.pStaticClass == app::gfx::FxParamManager::GetClass()) {
		RenderFxParamManagerInspector(static_cast<app::gfx::FxParamManager&>(service));
	}
	else {
		RenderUnknownServiceInspector(service);
	}
}

//namespace app::rfl {
//	struct NeedleFxParameter {
//		static const hh::fnd::RflClass rflClass;
//	};
//	struct FxBloomParameter {
//		static const hh::fnd::RflClass rflClass;
//	};
//	struct FxDOFParameter {
//		static const hh::fnd::RflClass rflClass;
//	};
//	struct FxColorContrastParameter {
//		static const hh::fnd::RflClass rflClass;
//	};
//	struct FxCameraControlParameter {
//		static const hh::fnd::RflClass rflClass;
//	};
//	struct alignas(16) FxShadowMapParameter {
//		static const hh::fnd::RflClass rflClass;
//	};
//	struct alignas(16) FxRenderOption {
//		static const hh::fnd::RflClass rflClass;
//	};
//	struct alignas(16) FxAtmosphereParameter {
//		static const hh::fnd::RflClass rflClass;
//	};
//	struct FxDensityParameter {
//		static const hh::fnd::RflClass rflClass;
//	};
//	struct FxChromaticAberrationParameter {
//		static const hh::fnd::RflClass rflClass;
//	};
//	struct FxColorAccessibilityFilterParameter {
//		static const hh::fnd::RflClass rflClass;
//	};
//	struct FxCyberSpaceStartNoiseParameter {
//		static const hh::fnd::RflClass rflClass;
//	};
//	struct FxAntiAliasing {
//		static const hh::fnd::RflClass rflClass;
//	};
//}

void GameServiceInspector::RenderFxParamManagerInspector(app::gfx::FxParamManager& service) {
	ImGui::SeparatorText("NeedleFXParameter");
	ReflectionEditor::Render(service.parameters);

	ImGui::SeparatorText("NeedleFXSceneConfig");
	ReflectionEditor::Render(service.sceneConfig);

	ImGui::SeparatorText("NeedleFXParameter interpolators");
	RenderFxParamManagerInterpolatorInspector<app::rfl::FxBloomParameter>("FxBloomParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxBloomParameter>*>(service.interpolators[0]));
	RenderFxParamManagerInterpolatorInspector<app::rfl::FxDOFParameter>("FxDOFParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxDOFParameter>*>(service.interpolators[1]));
	RenderFxParamManagerInterpolatorInspector<app::rfl::FxColorContrastParameter>("FxColorContrastParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxColorContrastParameter>*>(service.interpolators[2]));
	RenderFxParamManagerInterpolatorInspector<app::rfl::FxToneMapParameter>("FxToneMapParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxToneMapParameter>*>(service.interpolators[3]));
	RenderFxParamManagerInterpolatorInspector<app::rfl::FxCameraControlParameter>("FxCameraControlParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxCameraControlParameter>*>(service.interpolators[4]));
	RenderFxParamManagerInterpolatorInspector<app::rfl::FxShadowMapParameter>("FxShadowMapParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxShadowMapParameter>*>(service.interpolators[5]));
	RenderFxParamManagerInterpolatorInspector<app::rfl::FxShadowHeightMapParameter>("FxShadowHeightMapParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxShadowHeightMapParameter>*>(service.interpolators[6]));
	RenderFxParamManagerInterpolatorInspector<app::rfl::FxVolumetricShadowParameter>("FxVolumetricShadowParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxVolumetricShadowParameter>*>(service.interpolators[7]));
	RenderFxParamManagerInterpolatorInspector<app::rfl::FxScreenBlurParameter>("FxScreenBlurParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxScreenBlurParameter>*>(service.interpolators[8]));
	RenderFxParamManagerInterpolatorInspector<app::rfl::FxSSAOParameter>("FxSSAOParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxSSAOParameter>*>(service.interpolators[9]));
	RenderFxParamManagerInterpolatorInspector<app::rfl::FxSHLightFieldParameter>("FxSHLightFieldParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxSHLightFieldParameter>*>(service.interpolators[10]));
	RenderFxParamManagerInterpolatorInspector<app::rfl::FxLightScatteringParameter>("FxLightScatteringParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxLightScatteringParameter>*>(service.interpolators[11]));
	RenderFxParamManagerInterpolatorInspector<app::rfl::FxRLRParameter>("FxRLRParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxRLRParameter>*>(service.interpolators[12]));
	RenderFxParamManagerInterpolatorInspector<app::rfl::FxSSGIParameter>("FxSSGIParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxSSGIParameter>*>(service.interpolators[13]));
	RenderFxParamManagerInterpolatorInspector<app::rfl::FxPlanarReflectionParameter>("FxPlanarReflectionParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxPlanarReflectionParameter>*>(service.interpolators[14]));
	RenderFxParamManagerInterpolatorInspector<app::rfl::FxOcclusionCapsuleParameter>("FxOcclusionCapsuleParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxOcclusionCapsuleParameter>*>(service.interpolators[15]));
	RenderFxParamManagerInterpolatorInspector<app::rfl::FxGodrayParameter>("FxGodrayParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxGodrayParameter>*>(service.interpolators[16]));
	RenderFxParamManagerInterpolatorInspector<app::rfl::FxScreenSpaceGodrayParameter>("FxScreenSpaceGodrayParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxScreenSpaceGodrayParameter>*>(service.interpolators[17]));
	RenderFxParamManagerInterpolatorInspector<app::rfl::FxHeatHazeParameter>("FxHeatHazeParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxHeatHazeParameter>*>(service.interpolators[18]));
	RenderFxParamManagerInterpolatorInspector<app::rfl::FxSceneEnvironmentParameter>("FxSceneEnvironmentParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxSceneEnvironmentParameter>*>(service.interpolators[19]));
	RenderFxParamManagerInterpolatorInspector<app::rfl::FxRenderOption>("FxRenderOption", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxRenderOption>*>(service.interpolators[20]));
	RenderFxParamManagerInterpolatorInspector<app::rfl::FxSGGIParameter>("FxSGGIParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxSGGIParameter>*>(service.interpolators[21]));
	RenderFxParamManagerInterpolatorInspector<app::rfl::FxTAAParameter>("FxTAAParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxTAAParameter>*>(service.interpolators[22]));
	RenderFxParamManagerInterpolatorInspector<app::rfl::FxEffectParameter>("FxEffectParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxEffectParameter>*>(service.interpolators[23]));
	RenderFxParamManagerInterpolatorInspector<app::rfl::FxAtmosphereParameter>("FxAtmosphereParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxAtmosphereParameter>*>(service.interpolators[24]));
	RenderFxParamManagerInterpolatorInspector<app::rfl::FxDensityParameter>("FxDensityParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxDensityParameter>*>(service.interpolators[25]));
	RenderFxParamManagerInterpolatorInspector<app::rfl::FxWindComputeParameter>("FxWindComputeParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxWindComputeParameter>*>(service.interpolators[26]));
	RenderFxParamManagerInterpolatorInspector<app::rfl::FxGpuEnvironmentParameter>("FxGpuEnvironmentParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxGpuEnvironmentParameter>*>(service.interpolators[27]));
	RenderFxParamManagerInterpolatorInspector<app::rfl::FxInteractiveWaveParameter>("FxInteractiveWaveParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxInteractiveWaveParameter>*>(service.interpolators[28]));
	RenderFxParamManagerInterpolatorInspector<app::rfl::FxChromaticAberrationParameter>("FxChromaticAberrationParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxChromaticAberrationParameter>*>(service.interpolators[29]));
	RenderFxParamManagerInterpolatorInspector<app::rfl::FxVignetteParameter>("FxVignetteParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxVignetteParameter>*>(service.interpolators[30]));
	RenderFxParamManagerInterpolatorInspector<app::rfl::FxTerrainMaterialBlendingParameter>("FxTerrainMaterialBlendingParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxTerrainMaterialBlendingParameter>*>(service.interpolators[31]));
	RenderFxParamManagerInterpolatorInspector<app::rfl::FxWeatherParameter>("FxWeatherParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxWeatherParameter>*>(service.interpolators[32]));
	RenderFxParamManagerInterpolatorInspector<app::rfl::FxColorAccessibilityFilterParameter>("FxColorAccessibilityFilterParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxColorAccessibilityFilterParameter>*>(service.interpolators[33]));
	RenderFxParamManagerInterpolatorInspector<app::rfl::FxCyberNoiseEffectParameter>("FxCyberNoiseEffectParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxCyberNoiseEffectParameter>*>(service.interpolators[34]));
	RenderFxParamManagerInterpolatorInspector<app::rfl::FxCyberSpaceStartNoiseParameter>("FxCyberSpaceStartNoiseParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxCyberSpaceStartNoiseParameter>*>(service.interpolators[35]));
	RenderFxParamManagerInterpolatorInspector<app::rfl::FxCyberNPCSSEffectRenderParameter>("FxCyberNPCSSEffectRenderParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxCyberNPCSSEffectRenderParameter>*>(service.interpolators[36]));
	RenderFxParamManagerInterpolatorInspector<app::rfl::FxDentParameter>("FxDentParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxDentParameter>*>(service.interpolators[37]));
	RenderFxParamManagerInterpolatorInspector<app::rfl::FxFieldScanEffectRenderParameter>("FxFieldScanEffectRenderParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxFieldScanEffectRenderParameter>*>(service.interpolators[38]));
	RenderFxParamManagerInterpolatorInspector<app::rfl::FxSeparableSSSParameter>("FxSeparableSSSParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxSeparableSSSParameter>*>(service.interpolators[39]));

	ImGui::SeparatorText("NeedleFXSceneConfig interpolators");
	RenderFxParamManagerInterpolatorInspector<app::rfl::FxRenderTargetSetting>("FxRenderTargetSetting", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxRenderTargetSetting>*>(service.unk103.fxRenderTargetSettingInterpolator));
	RenderFxParamManagerInterpolatorInspector<app::rfl::FxAntiAliasing>("FxAntiAliasing", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxAntiAliasing>*>(service.unk103.fxAntiAliasingInterpolator));
	RenderFxParamManagerInterpolatorInspector<app::rfl::StageCommonAtmosphereParameter>("StageCommonAtmosphereParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::StageCommonAtmosphereParameter>*>(service.unk103.stageCommonAtmosphereParameterInterpolator));
	RenderFxParamManagerInterpolatorInspector<app::rfl::FxLODParameter>("FxLODParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxLODParameter>*>(service.unk103.fxLODParameterInterpolator));
	RenderFxParamManagerInterpolatorInspector<app::rfl::FxDetailParameter>("FxDetailParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxDetailParameter>*>(service.unk103.fxDetailParameterInterpolator));
	RenderFxParamManagerInterpolatorInspector<app::rfl::FxDynamicResolutionParameter>("FxDynamicResolutionParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxDynamicResolutionParameter>*>(service.unk103.fxDynamicResolutionParameterInterpolator));
	RenderFxParamManagerInterpolatorInspector<app::rfl::StageCommonTimeProgressParameter>("StageCommonTimeProgressParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::StageCommonTimeProgressParameter>*>(service.unk103.stageCommonTimeProgressParameterInterpolator));
}

void GameServiceInspector::RenderUnknownServiceInspector(hh::game::GameService& service) {
	ImGui::Text("Inspector for this service not yet implemented");
}
