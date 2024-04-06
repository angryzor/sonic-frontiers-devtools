#include "Pch.h"
#include "GameServiceInspector.h"
#include "resource-editors/ResObjectWorldEditor.h"
#include "Desktop.h"
#include "imgui/imgui_internal.h"
#include "common/ObjectDataEditor.h"

using namespace hh::game;

GameServiceInspector::GameServiceInspector(csl::fnd::IAllocator* allocator) : StandaloneWindow{ allocator } {
	SetTitle("GameService inspector");
}

void GameServiceInspector::RenderContents() {
	auto* gameManager = GameManager::GetInstance();

	for (auto* service : gameManager->GetServices()) {
		ImGui::PushID(service);
		if (ImGui::CollapsingHeader(service->pStaticClass->pName))
			RenderServiceInspector(*service);
		ImGui::PopID();
	}
}

void GameServiceInspector::RenderServiceInspector(hh::game::GameService& service) {
	if (service.pStaticClass == app::gfx::FxParamManager::GetClass()) {
		RenderFxParamManagerInspector(static_cast<app::gfx::FxParamManager&>(service));
	}
	else if (service.pStaticClass == hh::game::ObjectWorld::GetClass()) {
		RenderObjectWorldInspector(static_cast<hh::game::ObjectWorld&>(service));
	}
	else {
		RenderUnknownServiceInspector(service);
	}
}

void GameServiceInspector::RenderObjectWorldInspector(hh::game::ObjectWorld& objWorld) {
	int i = 0;
	for (auto* chunk : objWorld.GetWorldChunks()) {
		if (ImGui::TreeNode(chunk, "Chunk %d", i++)) {
			bool editorMode = chunk->IsStatusEditor();

			ImGui::Checkbox("Editor mode", &editorMode);

			chunk->SetEditorStatus(editorMode);

			if (ImGui::CollapsingHeader("Layers")) {
				for (auto* layer : chunk->GetLayers()) {
					if (ImGui::TreeNode(layer, layer->GetName())) {
						bool enabled = layer->IsEnable();

						ImGui::Checkbox("Enabled", &enabled);

						chunk->SetLayerEnabled(layer->GetName(), enabled);

						if (ImGui::Button("Edit resource"))
							ResObjectWorldEditor::Create(Desktop::instance->GetAllocator(), layer->GetResource());

						ImGui::TreePop();
					}
				}
			}

			if (ImGui::CollapsingHeader("Object statuses")) {
				for (auto& status : chunk->GetObjectStatuses()) {
					if (ImGui::TreeNode(&status, status.objectData->name)) {
						auto flags = static_cast<unsigned int>(status.flags.m_dummy);

						ImGui::CheckboxFlags("Enabled", &flags, 1 << static_cast<uint8_t>(WorldObjectStatus::Flag::ENABLED));
						ImGui::CheckboxFlags("Is alive", &flags, 1 << static_cast<uint8_t>(WorldObjectStatus::Flag::IS_ALIVE));
						ImGui::CheckboxFlags("Started", &flags, 1 << static_cast<uint8_t>(WorldObjectStatus::Flag::STARTED));
						ImGui::CheckboxFlags("No restart", &flags, 1 << static_cast<uint8_t>(WorldObjectStatus::Flag::NO_RESTART));

						status.flags.m_dummy = static_cast<WorldObjectStatus::Flag>(flags);

						ImGui::InputInt("Spawn priority", &status.spawnPriority);

						ImGui::SeparatorText("Object Data");
						ObjectDataEditor::Render(status.objectData);

						ImGui::TreePop();
					}
				}
			}
			ImGui::TreePop();
		}
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
	ImGui::SeparatorText("SceneParameters");
	ImGui::Text("Current: %d", service.currentSceneParameters);
	ImGui::Text("SceneParameters list:");

	for (size_t i = 0; i < 2; i++) {
		if (auto* sceneParameters = service.sceneParameters[i]) {
			if (ImGui::TreeNode(sceneParameters, "%d", i)) {
				ReflectionEditor::Render(*sceneParameters->GetSceneData());
				ImGui::TreePop();
			}
		}
	}

	ImGui::SeparatorText("Interpolated parameters");
	ReflectionEditor::Render(service.parameters);
	ReflectionEditor::Render(service.sceneConfig);

	ImGui::SeparatorText("Interpolators");

	if (ImGui::TreeNode("NeedleFXParameter")) {
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxBloomParameter>("FxBloomParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxBloomParameter>*>(service.paramInterpolators[0]));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxDOFParameter>("FxDOFParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxDOFParameter>*>(service.paramInterpolators[1]));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxColorContrastParameter>("FxColorContrastParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxColorContrastParameter>*>(service.paramInterpolators[2]));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxToneMapParameter>("FxToneMapParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxToneMapParameter>*>(service.paramInterpolators[3]));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxCameraControlParameter>("FxCameraControlParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxCameraControlParameter>*>(service.paramInterpolators[4]));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxShadowMapParameter>("FxShadowMapParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxShadowMapParameter>*>(service.paramInterpolators[5]));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxShadowHeightMapParameter>("FxShadowHeightMapParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxShadowHeightMapParameter>*>(service.paramInterpolators[6]));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxVolumetricShadowParameter>("FxVolumetricShadowParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxVolumetricShadowParameter>*>(service.paramInterpolators[7]));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxScreenBlurParameter>("FxScreenBlurParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxScreenBlurParameter>*>(service.paramInterpolators[8]));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxSSAOParameter>("FxSSAOParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxSSAOParameter>*>(service.paramInterpolators[9]));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxSHLightFieldParameter>("FxSHLightFieldParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxSHLightFieldParameter>*>(service.paramInterpolators[10]));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxLightScatteringParameter>("FxLightScatteringParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxLightScatteringParameter>*>(service.paramInterpolators[11]));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxRLRParameter>("FxRLRParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxRLRParameter>*>(service.paramInterpolators[12]));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxSSGIParameter>("FxSSGIParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxSSGIParameter>*>(service.paramInterpolators[13]));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxPlanarReflectionParameter>("FxPlanarReflectionParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxPlanarReflectionParameter>*>(service.paramInterpolators[14]));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxOcclusionCapsuleParameter>("FxOcclusionCapsuleParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxOcclusionCapsuleParameter>*>(service.paramInterpolators[15]));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxGodrayParameter>("FxGodrayParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxGodrayParameter>*>(service.paramInterpolators[16]));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxScreenSpaceGodrayParameter>("FxScreenSpaceGodrayParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxScreenSpaceGodrayParameter>*>(service.paramInterpolators[17]));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxHeatHazeParameter>("FxHeatHazeParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxHeatHazeParameter>*>(service.paramInterpolators[18]));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxSceneEnvironmentParameter>("FxSceneEnvironmentParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxSceneEnvironmentParameter>*>(service.paramInterpolators[19]));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxRenderOption>("FxRenderOption", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxRenderOption>*>(service.paramInterpolators[20]));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxSGGIParameter>("FxSGGIParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxSGGIParameter>*>(service.paramInterpolators[21]));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxTAAParameter>("FxTAAParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxTAAParameter>*>(service.paramInterpolators[22]));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxEffectParameter>("FxEffectParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxEffectParameter>*>(service.paramInterpolators[23]));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxAtmosphereParameter>("FxAtmosphereParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxAtmosphereParameter>*>(service.paramInterpolators[24]));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxDensityParameter>("FxDensityParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxDensityParameter>*>(service.paramInterpolators[25]));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxWindComputeParameter>("FxWindComputeParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxWindComputeParameter>*>(service.paramInterpolators[26]));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxGpuEnvironmentParameter>("FxGpuEnvironmentParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxGpuEnvironmentParameter>*>(service.paramInterpolators[27]));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxInteractiveWaveParameter>("FxInteractiveWaveParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxInteractiveWaveParameter>*>(service.paramInterpolators[28]));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxChromaticAberrationParameter>("FxChromaticAberrationParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxChromaticAberrationParameter>*>(service.paramInterpolators[29]));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxVignetteParameter>("FxVignetteParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxVignetteParameter>*>(service.paramInterpolators[30]));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxTerrainMaterialBlendingParameter>("FxTerrainMaterialBlendingParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxTerrainMaterialBlendingParameter>*>(service.paramInterpolators[31]));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxWeatherParameter>("FxWeatherParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxWeatherParameter>*>(service.paramInterpolators[32]));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxColorAccessibilityFilterParameter>("FxColorAccessibilityFilterParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxColorAccessibilityFilterParameter>*>(service.paramInterpolators[33]));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxCyberNoiseEffectParameter>("FxCyberNoiseEffectParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxCyberNoiseEffectParameter>*>(service.paramInterpolators[34]));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxCyberSpaceStartNoiseParameter>("FxCyberSpaceStartNoiseParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxCyberSpaceStartNoiseParameter>*>(service.paramInterpolators[35]));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxCyberNPCSSEffectRenderParameter>("FxCyberNPCSSEffectRenderParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxCyberNPCSSEffectRenderParameter>*>(service.paramInterpolators[36]));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxDentParameter>("FxDentParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxDentParameter>*>(service.paramInterpolators[37]));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxFieldScanEffectRenderParameter>("FxFieldScanEffectRenderParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxFieldScanEffectRenderParameter>*>(service.paramInterpolators[38]));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxSeparableSSSParameter>("FxSeparableSSSParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxSeparableSSSParameter>*>(service.paramInterpolators[39]));
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("NeedleFXSceneConfig")) {
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxRenderTargetSetting>("FxRenderTargetSetting", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxRenderTargetSetting>*>(service.sceneConfigInterpolators.fxRenderTargetSettingInterpolator));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxAntiAliasing>("FxAntiAliasing", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxAntiAliasing>*>(service.sceneConfigInterpolators.fxAntiAliasingInterpolator));
		RenderFxParamManagerInterpolatorInspector<app::rfl::StageCommonAtmosphereParameter>("StageCommonAtmosphereParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::StageCommonAtmosphereParameter>*>(service.sceneConfigInterpolators.stageCommonAtmosphereParameterInterpolator));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxLODParameter>("FxLODParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxLODParameter>*>(service.sceneConfigInterpolators.fxLODParameterInterpolator));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxDetailParameter>("FxDetailParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxDetailParameter>*>(service.sceneConfigInterpolators.fxDetailParameterInterpolator));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxDynamicResolutionParameter>("FxDynamicResolutionParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxDynamicResolutionParameter>*>(service.sceneConfigInterpolators.fxDynamicResolutionParameterInterpolator));
		RenderFxParamManagerInterpolatorInspector<app::rfl::StageCommonTimeProgressParameter>("StageCommonTimeProgressParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::StageCommonTimeProgressParameter>*>(service.sceneConfigInterpolators.stageCommonTimeProgressParameterInterpolator));
		ImGui::TreePop();
	}

	//ImGui::SeparatorText("Extensions");
	//for (auto* extension : service.extensions) {
	//	ImGui::Text("%zx", extension->GetRuntimeTypeInfo());
	//}
}

void GameServiceInspector::RenderUnknownServiceInspector(hh::game::GameService& service) {
	ImGui::Text("Inspector for this service not yet implemented");
}
