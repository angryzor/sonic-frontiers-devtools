#include "GameServiceInspector.h"
#include <imgui_internal.h>
#include <ui/Desktop.h>
#include <ui/resources/editors/ResObjectWorldEditor.h>
#include <ui/common/viewers/Basic.h>
#include <ui/common/editors/Basic.h>
#include <ui/common/editors/ObjectData.h>
#include <ui/common/editors/LevelInfo.h>
#include <ui/common/editors/GraphicsContext.h>
#include <ui/common/editors/Reflection.h>
#include <ui/common/inputs/Basic.h>

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
	else if (service.pStaticClass == app::level::StageInfo::GetClass()) {
		RenderStageInfoInspector(static_cast<app::level::StageInfo&>(service));
	}
	else if (service.pStaticClass == app::level::LevelInfo::GetClass()) {
		RenderLevelInfoInspector(static_cast<app::level::LevelInfo&>(service));
	}
	else if (service.pStaticClass == app::game::GameModeResourceManager::GetClass()) {
		RenderGameModeResourceManagerInspector(static_cast<app::game::GameModeResourceManager&>(service));
	}
	else if (service.pStaticClass == app::trr::TerrainManager::GetClass()) {
		RenderTerrainManagerInspector(static_cast<app::trr::TerrainManager&>(service));
	}
	else if (service.pStaticClass == hh::game::CameraManager::GetClass()) {
		RenderCameraManagerInspector(static_cast<hh::game::CameraManager&>(service));
	}
	else if (service.pStaticClass == app::camera::CameraService::GetClass()) {
		RenderCameraServiceInspector(static_cast<app::camera::CameraService&>(service));
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

			if (ImGui::Checkbox("Editor mode", &editorMode))
				chunk->SetEditorStatus(editorMode);

			if (ImGui::CollapsingHeader("Layers")) {
				for (auto* layer : chunk->GetLayers()) {
					if (ImGui::TreeNode(layer, layer->GetName())) {
						bool enabled = layer->IsEnable();

						if (ImGui::Checkbox("Enabled", &enabled))
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
						CheckboxFlags("Enabled", status.flags, WorldObjectStatus::Flag::ENABLED);
						CheckboxFlags("Is alive", status.flags, WorldObjectStatus::Flag::IS_ALIVE);
						CheckboxFlags("Started", status.flags, WorldObjectStatus::Flag::STARTED);
						CheckboxFlags("No restart", status.flags, WorldObjectStatus::Flag::NO_RESTART);

						Editor("Spawn priority", status.spawnPriority);

						ImGui::SeparatorText("Object Data");
						Editor("Object data", *status.objectData);

						ImGui::TreePop();
					}
				}
			}
			ImGui::TreePop();
		}
	}
}

void GameServiceInspector::RenderStageInfoInspector(app::level::StageInfo& service)
{
	ImGui::SeparatorText("Stages");
	for (auto& stage : service.stages) {
		if (ImGui::TreeNodeEx(stage, ImGuiTreeNodeFlags_None, "%s", stage->name.c_str())) {
			Editor("Stage data", *stage);
			ImGui::TreePop();
		}
	}
}

void GameServiceInspector::RenderLevelInfoInspector(app::level::LevelInfo& service)
{
	auto* stageData = service.GetStageData();
	ImGui::SeparatorText("Stage data");
	if (stageData)
		Editor("Stage data", *stageData);
	else
		ImGui::Text("No stage data loaded.");

	if (ImGui::TreeNode("PlayerInformation")) {
		for (size_t i = 0; i < 2; i++) {
			auto* playerInfo = service.GetPlayerInformation(i);
			char nodeName[50];
			snprintf(nodeName, sizeof(nodeName), "Player %zd", i);

			if (ImGui::TreeNode(nodeName)) {
				Editor(nodeName, *playerInfo);
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}
}

void GameServiceInspector::RenderGameModeResourceManagerInspector(app::game::GameModeResourceManager& service)
{
	for (auto& module : service.modules) {
		if (ImGui::TreeNodeEx(module, ImGuiTreeNodeFlags_DefaultOpen, "%x", module->GetNameHash())) {
			ImGui::Text("Module flags: %x", module->flags);
			if (ImGui::TreeNode("Resources")) {
				for (auto& resource : module->resourceCollection->resources) {
					ImGui::Text("%s", resource.name);
				}
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}
	}
}

void GameServiceInspector::RenderTerrainManagerInspector(app::trr::TerrainManager& service)
{
	Viewer("Unk11", service.currentTerrain);
	Viewer("Unk10 count", service.unk10.size());
	//ImGui::SeparatorText("Unk13s:");
	//if (service.unk13.)
	//for (auto i = service.unk13.begin(); i != service.unk13.end(); i++) {
	//	ImGui::Text("%s", i.key());
	//}
}

void GameServiceInspector::RenderCameraManagerInspector(hh::game::CameraManager& service)
{
	ImGui::SeparatorText("Components");
	for (auto* component : service.GetComponents()) {
		if (ImGui::TreeNode(component, component->name)) {
			ImGui::Text("Viewport ID: %d", component->viewportId);
			Editor("Viewport data", component->viewportData);
			ImGui::TreePop();
		}
	}

	ImGui::SeparatorText("Camera stacks");
	for (int i = 0; i < service.GetCameraCount(); i++) {
		ImGui::PushID(i);
		if (ImGui::TreeNode("Camera stack", "Camera stack %d", i)) {
			csl::ut::MoveArray<CameraComponent*> components{ hh::fnd::MemoryRouter::GetTempAllocator() };
			service.GetComponents(i, components);

			if (components.size() > 0) {
				ImGui::SeparatorText("Components");

				for (auto* component : components) {
					if (ImGui::TreeNode(component, component->name)) {
						ImGui::Text("Viewport ID: %d", component->viewportId);
						Editor("Viewport data", component->viewportData);
						ImGui::TreePop();
					}
				}
			}

			ImGui::TreePop();
		}
		ImGui::PopID();
	}
}

void GameServiceInspector::RenderCameraServiceInspector(app::camera::CameraService& service)
{
	for (auto& bridgeUnit : service.cameraBridges) {
		if (bridgeUnit.cameraBridge.cameraFrameId != -1 && bridgeUnit.cameraComponent != nullptr && ImGui::TreeNodeEx(&bridgeUnit, ImGuiTreeNodeFlags_None, "%d", bridgeUnit.cameraBridge.cameraFrameId)) {
			ImGui::Text("Camera frame ID: %d - %s", bridgeUnit.cameraBridge.cameraFrameId, bridgeUnit.cameraBridge.cameraFrame->name.c_str());
			Editor("CameraFrame", bridgeUnit.cameraBridge.cameraFrame);
			if (ImGui::TreeNodeEx("Component", ImGuiTreeNodeFlags_None, "%s", bridgeUnit.cameraComponent->name.c_str())) {
				ImGui::Text("Viewport ID: %d", bridgeUnit.cameraComponent->viewportId);
				Editor("Viewport data", bridgeUnit.cameraComponent->viewportData);
				ImGui::TreePop();
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
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxBloomParameter>("FxBloomParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxBloomParameter>*>(service.paramInterpolators.bloomInterpolator));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxDOFParameter>("FxDOFParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxDOFParameter>*>(service.paramInterpolators.dofInterpolator));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxColorContrastParameter>("FxColorContrastParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxColorContrastParameter>*>(service.paramInterpolators.colorContrastInterpolator));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxToneMapParameter>("FxToneMapParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxToneMapParameter>*>(service.paramInterpolators.tonemapInterpolator));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxCameraControlParameter>("FxCameraControlParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxCameraControlParameter>*>(service.paramInterpolators.cameraControlInterpolator));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxShadowMapParameter>("FxShadowMapParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxShadowMapParameter>*>(service.paramInterpolators.shadowmapInterpolator));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxShadowHeightMapParameter>("FxShadowHeightMapParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxShadowHeightMapParameter>*>(service.paramInterpolators.shadowHeightMapInterpolator));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxVolumetricShadowParameter>("FxVolumetricShadowParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxVolumetricShadowParameter>*>(service.paramInterpolators.volShadowInterpolator));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxScreenBlurParameter>("FxScreenBlurParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxScreenBlurParameter>*>(service.paramInterpolators.blurInterpolator));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxSSAOParameter>("FxSSAOParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxSSAOParameter>*>(service.paramInterpolators.ssaoInterpolator));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxSHLightFieldParameter>("FxSHLightFieldParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxSHLightFieldParameter>*>(service.paramInterpolators.shlightfieldInterpolator));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxLightScatteringParameter>("FxLightScatteringParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxLightScatteringParameter>*>(service.paramInterpolators.lightscatteringInterpolator));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxRLRParameter>("FxRLRParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxRLRParameter>*>(service.paramInterpolators.rlrInterpolator));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxSSGIParameter>("FxSSGIParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxSSGIParameter>*>(service.paramInterpolators.ssgiInterpolator));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxPlanarReflectionParameter>("FxPlanarReflectionParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxPlanarReflectionParameter>*>(service.paramInterpolators.planarReflectionInterpolator));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxOcclusionCapsuleParameter>("FxOcclusionCapsuleParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxOcclusionCapsuleParameter>*>(service.paramInterpolators.occlusionCapsuleInterpolator));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxGodrayParameter>("FxGodrayParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxGodrayParameter>*>(service.paramInterpolators.godrayInterpolator));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxScreenSpaceGodrayParameter>("FxScreenSpaceGodrayParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxScreenSpaceGodrayParameter>*>(service.paramInterpolators.ssGodrayInterpolator));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxHeatHazeParameter>("FxHeatHazeParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxHeatHazeParameter>*>(service.paramInterpolators.heatHazeInterpolator));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxSceneEnvironmentParameter>("FxSceneEnvironmentParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxSceneEnvironmentParameter>*>(service.paramInterpolators.sceneEnvInterpolator));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxRenderOption>("FxRenderOption", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxRenderOption>*>(service.paramInterpolators.renderOptionInterpolator));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxSGGIParameter>("FxSGGIParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxSGGIParameter>*>(service.paramInterpolators.sggiInterpolator));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxTAAParameter>("FxTAAParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxTAAParameter>*>(service.paramInterpolators.taaInterpolator));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxEffectParameter>("FxEffectParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxEffectParameter>*>(service.paramInterpolators.effectInterpolator));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxAtmosphereParameter>("FxAtmosphereParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxAtmosphereParameter>*>(service.paramInterpolators.atmosphereInterpolator));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxDensityParameter>("FxDensityParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxDensityParameter>*>(service.paramInterpolators.densityInterpolator));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxWindComputeParameter>("FxWindComputeParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxWindComputeParameter>*>(service.paramInterpolators.windInterpolator));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxGpuEnvironmentParameter>("FxGpuEnvironmentParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxGpuEnvironmentParameter>*>(service.paramInterpolators.gpuEnvironmentInterpolator));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxInteractiveWaveParameter>("FxInteractiveWaveParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxInteractiveWaveParameter>*>(service.paramInterpolators.interactiveWaveInterpolator));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxChromaticAberrationParameter>("FxChromaticAberrationParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxChromaticAberrationParameter>*>(service.paramInterpolators.chromaticAberrationInterpolator));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxVignetteParameter>("FxVignetteParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxVignetteParameter>*>(service.paramInterpolators.vignetteInterpolator));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxTerrainMaterialBlendingParameter>("FxTerrainMaterialBlendingParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxTerrainMaterialBlendingParameter>*>(service.paramInterpolators.terrainBlendInterpolator));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxWeatherParameter>("FxWeatherParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxWeatherParameter>*>(service.paramInterpolators.weatherInterpolator));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxColorAccessibilityFilterParameter>("FxColorAccessibilityFilterParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxColorAccessibilityFilterParameter>*>(service.paramInterpolators.colorAccessibilityInterpolator));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxCyberNoiseEffectParameter>("FxCyberNoiseEffectParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxCyberNoiseEffectParameter>*>(service.paramInterpolators.cyberNoiseInterpolator));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxCyberSpaceStartNoiseParameter>("FxCyberSpaceStartNoiseParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxCyberSpaceStartNoiseParameter>*>(service.paramInterpolators.cyberStartNoiseInterpolator));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxCyberNPCSSEffectRenderParameter>("FxCyberNPCSSEffectRenderParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxCyberNPCSSEffectRenderParameter>*>(service.paramInterpolators.cyberNPCSSInterpolator));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxDentParameter>("FxDentParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxDentParameter>*>(service.paramInterpolators.dentInterpolator));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxFieldScanEffectRenderParameter>("FxFieldScanEffectRenderParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxFieldScanEffectRenderParameter>*>(service.paramInterpolators.fieldScanInterpolator));
		RenderFxParamManagerInterpolatorInspector<app::rfl::FxSeparableSSSParameter>("FxSeparableSSSParameter", *static_cast<app::gfx::FxParamManager::Interpolator<app::rfl::FxSeparableSSSParameter>*>(service.paramInterpolators.ssssInterpolator));
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
