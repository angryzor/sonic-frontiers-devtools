#include "GameServiceInspector.h"
#include <imgui_internal.h>
#include <ui/Desktop.h>
#include <ui/resources/editors/ResObjectWorldEditor.h>
#include <ui/common/ObjectDataEditor.h>
#include <ui/common/SimpleWidgets.h>

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

						status.flags.m_dummy = flags;

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

void GameServiceInspector::RenderStageInfoInspector(app::level::StageInfo& service)
{
	ImGui::SeparatorText("Stages");
	for (auto& stage : service.stages) {
		if (ImGui::TreeNodeEx(stage, ImGuiTreeNodeFlags_None, "%s", stage->name.c_str())) {
			RenderStageDataInspector(*stage);
			ImGui::TreePop();
		}
	}
}

void GameServiceInspector::RenderLevelInfoInspector(app::level::LevelInfo& service)
{
	auto* stageData = service.GetStageData();
	ImGui::SeparatorText("Stage data");
	if (stageData)
		RenderStageDataInspector(*stageData);
	else
		ImGui::Text("No stage data loaded.");
}

static const char* chunkTypeNames[] = { "INITIAL", "AFTER" };
static const char* cyberModeNames[] = { "UNKNOWN", "LOW_GRAVITY", "TIME_EXTEND", "SPEED_SCALE", "NITRO", "MAX_SPEED_CHALLENGE" };

void GameServiceInspector::RenderStageDataInspector(app::level::StageData& data)
{
	ImGui::PushID(&data);

	InputText("Name", &data.name);
	InputText("Stage", &data.stage);
	InputText("Cyber name", &data.cyberName);
	InputText("SceneParam name", &data.sceneParamName);
	InputText("SceneParam stage", &data.sceneParamStage);
	InputText("Gedit resource name", &data.geditResourceName);

	ImGui::DragInt("World index", &data.worldIndex, 1.0f, -1);
	ImGui::DragInt("Stage index", &data.stageIndex, 1.0f, -1);
	ImGui::DragInt("Cyber stage index", &data.cyberStageIndex, 1.0f, -1);
	ImGui::DragScalar("Default SceneParam index", ImGuiDataType_U32, &data.defaultSceneParamIndex);

	if (ImGui::TreeNode("Attributes")) {
		CheckboxFlags("Unknown", &data.attributeFlags, static_cast<app::level::StageData::AttributeFlags>(0));
		CheckboxFlags("Cyber", &data.attributeFlags, app::level::StageData::AttributeFlags::CYBER);
		CheckboxFlags("Minigame", &data.attributeFlags, app::level::StageData::AttributeFlags::MINIGAME);
		CheckboxFlags("Hacking", &data.attributeFlags, app::level::StageData::AttributeFlags::HACKING);
		CheckboxFlags("Last boss", &data.attributeFlags, app::level::StageData::AttributeFlags::LAST_BOSS);
		CheckboxFlags("Master trial", &data.attributeFlags, app::level::StageData::AttributeFlags::MASTER_TRIAL);
		CheckboxFlags("Tutorial", &data.attributeFlags, app::level::StageData::AttributeFlags::TUTORIAL);
		CheckboxFlags("Navmesh", &data.attributeFlags, app::level::StageData::AttributeFlags::NAVMESH);
		CheckboxFlags("Height field", &data.attributeFlags, app::level::StageData::AttributeFlags::HEIGHT_FIELD);
		CheckboxFlags("Point cloud", &data.attributeFlags, app::level::StageData::AttributeFlags::POINT_CLOUD);
		CheckboxFlags("Autosave", &data.attributeFlags, app::level::StageData::AttributeFlags::AUTOSAVE);
		CheckboxFlags("Diving", &data.attributeFlags, app::level::StageData::AttributeFlags::DIVING);
		CheckboxFlags("Side step", &data.attributeFlags, app::level::StageData::AttributeFlags::SIDE_STEP);
		CheckboxFlags("Athletic", &data.attributeFlags, app::level::StageData::AttributeFlags::ATHLETIC);
		CheckboxFlags("Boarding", &data.attributeFlags, app::level::StageData::AttributeFlags::BOARDING);
		CheckboxFlags("Drift", &data.attributeFlags, app::level::StageData::AttributeFlags::DRIFT);
		CheckboxFlags("Side view", &data.attributeFlags, app::level::StageData::AttributeFlags::SIDE_VIEW);
		CheckboxFlags("Lava", &data.attributeFlags, app::level::StageData::AttributeFlags::LAVA);
		CheckboxFlags("Sonic", &data.attributeFlags, app::level::StageData::AttributeFlags::SONIC);
		CheckboxFlags("Tails", &data.attributeFlags, app::level::StageData::AttributeFlags::TAILS);
		CheckboxFlags("Amy", &data.attributeFlags, app::level::StageData::AttributeFlags::AMY);
		CheckboxFlags("Knuckles", &data.attributeFlags, app::level::StageData::AttributeFlags::KNUCKLES);
		CheckboxFlags("Battle rush", &data.attributeFlags, app::level::StageData::AttributeFlags::BATTLE_RUSH);
		CheckboxFlags("Boss rush", &data.attributeFlags, app::level::StageData::AttributeFlags::BOSS_RUSH);
		CheckboxFlags("Extra", &data.attributeFlags, app::level::StageData::AttributeFlags::EXTRA);
		CheckboxFlags("Delete fall dead collision", &data.attributeFlags, app::level::StageData::AttributeFlags::DELETE_FALL_DEAD_COLLISION);
		CheckboxFlags("Delete air wall collision", &data.attributeFlags, app::level::StageData::AttributeFlags::DELETE_AIR_WALL_COLLISION);
		CheckboxFlags("Delete grind rail", &data.attributeFlags, app::level::StageData::AttributeFlags::DELETE_GRIND_RAIL);
		CheckboxFlags("Change new collision", &data.attributeFlags, app::level::StageData::AttributeFlags::CHANGE_NEW_COLLISION);
		CheckboxFlags("Restrict debris", &data.attributeFlags, app::level::StageData::AttributeFlags::RESTRICT_DEBRIS);
		CheckboxFlags("Special parry effect", &data.attributeFlags, app::level::StageData::AttributeFlags::SPECIAL_PARRY_EFFECT);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Mission flags")) {
		CheckboxFlags("Unknown", &data.missionFlags, static_cast<app::level::StageData::MissionFlags>(0));
		CheckboxFlags("Goal", &data.missionFlags, app::level::StageData::MissionFlags::GOAL);
		CheckboxFlags("Rank", &data.missionFlags, app::level::StageData::MissionFlags::RANK);
		CheckboxFlags("Ring", &data.missionFlags, app::level::StageData::MissionFlags::RING);
		CheckboxFlags("Red ring", &data.missionFlags, app::level::StageData::MissionFlags::RED_RING);
		CheckboxFlags("Number ring", &data.missionFlags, app::level::StageData::MissionFlags::NUMBER_RING);
		CheckboxFlags("Silver moon ring", &data.missionFlags, app::level::StageData::MissionFlags::SILVER_MOON_RING);
		CheckboxFlags("Hide goal", &data.missionFlags, app::level::StageData::MissionFlags::HIDE_GOAL);
		CheckboxFlags("Shadow tails", &data.missionFlags, app::level::StageData::MissionFlags::SHADOW_TAILS);
		CheckboxFlags("Mine", &data.missionFlags, app::level::StageData::MissionFlags::MINE);
		CheckboxFlags("Animal", &data.missionFlags, app::level::StageData::MissionFlags::ANIMAL);
		ImGui::TreePop();
	}

	ComboEnum("Chunk type", &data.chunkType, chunkTypeNames);
	ImGui::DragFloat("Time limit", &data.timeLimit, 0.01f);
	ImGui::DragFloat("Death plane height", &data.deathPlaneHeight, 0.001f);
	ImGui::DragFloat("Noise time", &data.noiseTime, 0.01f);
	//ImGui::Text("Unk6: %d", data.unk6);
	if (ImGui::TreeNode("Red Ring mission thresholds")) {
		ImGui::DragScalar("S rank", ImGuiDataType_U8, &data.redRingMissionThreshold[0]);
		ImGui::DragScalar("A rank", ImGuiDataType_U8, &data.redRingMissionThreshold[1]);
		ImGui::DragScalar("B rank", ImGuiDataType_U8, &data.redRingMissionThreshold[2]);
		ImGui::DragScalar("C rank", ImGuiDataType_U8, &data.redRingMissionThreshold[3]);
		ImGui::TreePop();
	}
	//ImGui::Text("Unk7: %d", data.unk7);
	ImGui::InputScalar("Ring mission threshold: %d", ImGuiDataType_U32, &data.ringMissionThreshold);
	ComboEnum("Cyber mode", &data.cyberMode, cyberModeNames);

	//csl::ut::MoveArray<uint8_t> staticSectors;
	//csl::ut::MoveArray<uint8_t> dynamicSectors;

	if (ImGui::TreeNode("Rank times")) {
		if (ImGui::TreeNode("Normal")) {
			ImGui::DragScalar("S rank", ImGuiDataType_U32, &data.rankTimes[0]);
			ImGui::DragScalar("A rank", ImGuiDataType_U32, &data.rankTimes[1]);
			ImGui::DragScalar("B rank", ImGuiDataType_U32, &data.rankTimes[2]);
			ImGui::DragScalar("C rank", ImGuiDataType_U32, &data.rankTimes[3]);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Very hard")) {
			ImGui::DragScalar("S rank", ImGuiDataType_U32, &data.rankTimesVeryHard[0]);
			ImGui::DragScalar("A rank", ImGuiDataType_U32, &data.rankTimesVeryHard[1]);
			ImGui::DragScalar("B rank", ImGuiDataType_U32, &data.rankTimesVeryHard[2]);
			ImGui::DragScalar("C rank", ImGuiDataType_U32, &data.rankTimesVeryHard[3]);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Challenge")) {
			ImGui::DragScalar("S rank", ImGuiDataType_U32, &data.rankTimesChallenge[0]);
			ImGui::DragScalar("A rank", ImGuiDataType_U32, &data.rankTimesChallenge[1]);
			ImGui::DragScalar("B rank", ImGuiDataType_U32, &data.rankTimesChallenge[2]);
			ImGui::DragScalar("C rank", ImGuiDataType_U32, &data.rankTimesChallenge[3]);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Challenge all")) {
			ImGui::DragScalar("S rank", ImGuiDataType_U32, &data.rankTimesChallengeAll[0]);
			ImGui::DragScalar("A rank", ImGuiDataType_U32, &data.rankTimesChallengeAll[1]);
			ImGui::DragScalar("B rank", ImGuiDataType_U32, &data.rankTimesChallengeAll[2]);
			ImGui::DragScalar("C rank", ImGuiDataType_U32, &data.rankTimesChallengeAll[3]);
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}

	ImGui::PopID();
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
	ImGui::Text("Unk11: %d", service.currentTerrain);
	ImGui::Text("Unk10 count: %d", service.unk10.size());
	//ImGui::SeparatorText("Unk13s:");
	//if (service.unk13.)
	//for (auto i = service.unk13.begin(); i != service.unk13.end(); i++) {
	//	ImGui::Text("%s", i.key());
	//}
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
