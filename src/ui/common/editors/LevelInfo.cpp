#include "LevelInfo.h"
#include "Basic.h"
#include <ui/common/inputs/Basic.h>

static const char* chunkTypeNames[] = { "INITIAL", "AFTER" };
static const char* cyberModeNames[] = { "UNKNOWN", "LOW_GRAVITY", "TIME_EXTEND", "SPEED_SCALE", "NITRO", "MAX_SPEED_CHALLENGE" };

template<typename T>
bool PerRankEditor(const char* label, T (&ranks)[4]) {
	bool edited{};

	if (ImGui::TreeNode(label)) {
		edited |= Editor("S rank", ranks[0]);
		edited |= Editor("A rank", ranks[1]);
		edited |= Editor("B rank", ranks[2]);
		edited |= Editor("C rank", ranks[3]);
		ImGui::TreePop();
	}

	return edited;
}

bool Editor(const char* label, app::level::StageData& data) {
	bool edited{};

	ImGui::PushID(&data);

	edited |= Editor("Name", data.name);
	edited |= Editor("Stage", data.stage);
	edited |= Editor("Cyber name", data.cyberName);
	edited |= Editor("SceneParam name", data.sceneParamName);
	edited |= Editor("SceneParam stage", data.sceneParamStage);
	edited |= Editor("Gedit resource name", data.geditResourceName);

	int missingIdx{ -1 };
	edited |= DragScalar("World index", data.worldIndex, &missingIdx);
	edited |= DragScalar("Stage index", data.stageIndex, &missingIdx);
	edited |= DragScalar("Cyber stage index", data.cyberStageIndex, &missingIdx);
	edited |= Editor("Default SceneParam index", data.defaultSceneParamIndex);

	if (ImGui::TreeNode("Attributes")) {
		edited |= CheckboxFlags("Unknown", data.attributeFlags, static_cast<app::level::StageData::AttributeFlags>(0));
		edited |= CheckboxFlags("Cyber", data.attributeFlags, app::level::StageData::AttributeFlags::CYBER);
		edited |= CheckboxFlags("Minigame", data.attributeFlags, app::level::StageData::AttributeFlags::MINIGAME);
		edited |= CheckboxFlags("Hacking", data.attributeFlags, app::level::StageData::AttributeFlags::HACKING);
		edited |= CheckboxFlags("Last boss", data.attributeFlags, app::level::StageData::AttributeFlags::LAST_BOSS);
		edited |= CheckboxFlags("Master trial", data.attributeFlags, app::level::StageData::AttributeFlags::MASTER_TRIAL);
		edited |= CheckboxFlags("Tutorial", data.attributeFlags, app::level::StageData::AttributeFlags::TUTORIAL);
		edited |= CheckboxFlags("Navmesh", data.attributeFlags, app::level::StageData::AttributeFlags::NAVMESH);
		edited |= CheckboxFlags("Height field", data.attributeFlags, app::level::StageData::AttributeFlags::HEIGHT_FIELD);
		edited |= CheckboxFlags("Point cloud", data.attributeFlags, app::level::StageData::AttributeFlags::POINT_CLOUD);
		edited |= CheckboxFlags("Autosave", data.attributeFlags, app::level::StageData::AttributeFlags::AUTOSAVE);
		edited |= CheckboxFlags("Diving", data.attributeFlags, app::level::StageData::AttributeFlags::DIVING);
		edited |= CheckboxFlags("Side step", data.attributeFlags, app::level::StageData::AttributeFlags::SIDE_STEP);
		edited |= CheckboxFlags("Athletic", data.attributeFlags, app::level::StageData::AttributeFlags::ATHLETIC);
		edited |= CheckboxFlags("Boarding", data.attributeFlags, app::level::StageData::AttributeFlags::BOARDING);
		edited |= CheckboxFlags("Drift", data.attributeFlags, app::level::StageData::AttributeFlags::DRIFT);
		edited |= CheckboxFlags("Side view", data.attributeFlags, app::level::StageData::AttributeFlags::SIDE_VIEW);
		edited |= CheckboxFlags("Lava", data.attributeFlags, app::level::StageData::AttributeFlags::LAVA);
		edited |= CheckboxFlags("Sonic", data.attributeFlags, app::level::StageData::AttributeFlags::SONIC);
		edited |= CheckboxFlags("Tails", data.attributeFlags, app::level::StageData::AttributeFlags::TAILS);
		edited |= CheckboxFlags("Amy", data.attributeFlags, app::level::StageData::AttributeFlags::AMY);
		edited |= CheckboxFlags("Knuckles", data.attributeFlags, app::level::StageData::AttributeFlags::KNUCKLES);
		edited |= CheckboxFlags("Battle rush", data.attributeFlags, app::level::StageData::AttributeFlags::BATTLE_RUSH);
		edited |= CheckboxFlags("Boss rush", data.attributeFlags, app::level::StageData::AttributeFlags::BOSS_RUSH);
		edited |= CheckboxFlags("Extra", data.attributeFlags, app::level::StageData::AttributeFlags::EXTRA);
		edited |= CheckboxFlags("Delete fall dead collision", data.attributeFlags, app::level::StageData::AttributeFlags::DELETE_FALL_DEAD_COLLISION);
		edited |= CheckboxFlags("Delete air wall collision", data.attributeFlags, app::level::StageData::AttributeFlags::DELETE_AIR_WALL_COLLISION);
		edited |= CheckboxFlags("Delete grind rail", data.attributeFlags, app::level::StageData::AttributeFlags::DELETE_GRIND_RAIL);
		edited |= CheckboxFlags("Change new collision", data.attributeFlags, app::level::StageData::AttributeFlags::CHANGE_NEW_COLLISION);
		edited |= CheckboxFlags("Restrict debris", data.attributeFlags, app::level::StageData::AttributeFlags::RESTRICT_DEBRIS);
		edited |= CheckboxFlags("Special parry effect", data.attributeFlags, app::level::StageData::AttributeFlags::SPECIAL_PARRY_EFFECT);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Mission flags")) {
		edited |= CheckboxFlags("Unknown", data.missionFlags, static_cast<app::level::StageData::MissionFlags>(0));
		edited |= CheckboxFlags("Goal", data.missionFlags, app::level::StageData::MissionFlags::GOAL);
		edited |= CheckboxFlags("Rank", data.missionFlags, app::level::StageData::MissionFlags::RANK);
		edited |= CheckboxFlags("Ring", data.missionFlags, app::level::StageData::MissionFlags::RING);
		edited |= CheckboxFlags("Red ring", data.missionFlags, app::level::StageData::MissionFlags::RED_RING);
		edited |= CheckboxFlags("Number ring", data.missionFlags, app::level::StageData::MissionFlags::NUMBER_RING);
		edited |= CheckboxFlags("Silver moon ring", data.missionFlags, app::level::StageData::MissionFlags::SILVER_MOON_RING);
		edited |= CheckboxFlags("Hide goal", data.missionFlags, app::level::StageData::MissionFlags::HIDE_GOAL);
		edited |= CheckboxFlags("Shadow tails", data.missionFlags, app::level::StageData::MissionFlags::SHADOW_TAILS);
		edited |= CheckboxFlags("Mine", data.missionFlags, app::level::StageData::MissionFlags::MINE);
		edited |= CheckboxFlags("Animal", data.missionFlags, app::level::StageData::MissionFlags::ANIMAL);
		ImGui::TreePop();
	}

	edited |= ComboEnum("Chunk type", data.chunkType, chunkTypeNames);
	edited |= Editor("Time limit", data.timeLimit);
	edited |= Editor("Death plane height", data.deathPlaneHeight);
	edited |= Editor("Noise time", data.noiseTime);
	//ImGui::Text("Unk6: %d", data.unk6);
	edited |= PerRankEditor("Red Ring mission thresholds", data.redRingMissionThreshold);

	//ImGui::Text("Unk7: %d", data.unk7);
	edited |= Editor("Ring mission threshold", data.ringMissionThreshold);
	edited |= ComboEnum("Cyber mode", data.cyberMode, cyberModeNames);

	//csl::ut::MoveArray<uint8_t> staticSectors;
	//csl::ut::MoveArray<uint8_t> dynamicSectors;

	if (ImGui::TreeNode("Rank times")) {
		edited |= PerRankEditor("Normal", data.rankTimes);
		edited |= PerRankEditor("Very hard", data.rankTimesVeryHard);
		edited |= PerRankEditor("Challenge", data.rankTimesChallenge);
		edited |= PerRankEditor("Challenge all", data.rankTimesChallengeAll);
		ImGui::TreePop();
	}

	ImGui::PopID();

	return edited;
}
