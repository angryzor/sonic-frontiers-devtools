#include <ui/game-services/game-service-inspectors/LevelInfo.h>
#include <ui/common/editors/Basic.h>
#include <ui/common/inputs/Basic.h>

static const char* chunkTypeNames[] = { "INITIAL", "AFTER" };
static const char* clearTypeNames[] = { "GOAL", "TIME" };

template<typename T>
bool PerRankEditor(const char* label, T(&ranks)[4]) {
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
	edited |= Editor("Cyber name", data.path);
	edited |= Editor("SceneParam name", data.sceneParamName);
	edited |= Editor("SceneParam stage", data.sceneParamStage);
	edited |= Editor("Gedit resource name", data.geditResourceName);
	edited |= Editor("BGM", data.bgm);

	int missingIdx{ -1 };
	edited |= DragScalar("Stage index", data.stageIndex, &missingIdx);
	edited |= Editor("Default SceneParam index", data.defaultSceneParamIndex);

	if (ImGui::TreeNode("Attributes")) {
		edited |= CheckboxFlags("Test stage", data.attributeFlags, app::level::StageData::AttributeFlags::TEST_STAGE);
		edited |= CheckboxFlags("Navmesh", data.attributeFlags, app::level::StageData::AttributeFlags::NAVMESH);
		edited |= CheckboxFlags("Height field", data.attributeFlags, app::level::StageData::AttributeFlags::HEIGHTFIELD);
		edited |= CheckboxFlags("Point cloud", data.attributeFlags, app::level::StageData::AttributeFlags::POINTCLOUD);
		edited |= CheckboxFlags("Autosave", data.attributeFlags, app::level::StageData::AttributeFlags::AUTOSAVE);
		edited |= CheckboxFlags("Diving", data.attributeFlags, app::level::StageData::AttributeFlags::DIVING);
		edited |= CheckboxFlags("Side step", data.attributeFlags, app::level::StageData::AttributeFlags::SIDE_STEP);
		edited |= CheckboxFlags("Athletic", data.attributeFlags, app::level::StageData::AttributeFlags::ATHLETIC);
		edited |= CheckboxFlags("Boarding", data.attributeFlags, app::level::StageData::AttributeFlags::BOARDING);
		edited |= CheckboxFlags("Drift", data.attributeFlags, app::level::StageData::AttributeFlags::DRIFT);
		edited |= CheckboxFlags("Side view", data.attributeFlags, app::level::StageData::AttributeFlags::SIDE_VIEW);
		edited |= CheckboxFlags("Lava", data.attributeFlags, app::level::StageData::AttributeFlags::LAVA);
		edited |= CheckboxFlags("Boss", data.attributeFlags, app::level::StageData::AttributeFlags::BOSS);
		edited |= CheckboxFlags("White Space", data.attributeFlags, app::level::StageData::AttributeFlags::WHITE_SPACE);
		edited |= CheckboxFlags("IBL capture with SVCol", data.attributeFlags, app::level::StageData::AttributeFlags::IBL_CAPTURE_WITH_SV_COL);
		edited |= CheckboxFlags("Use speed line follow effect", data.attributeFlags, app::level::StageData::AttributeFlags::USE_SPEED_LINE_FOLLOW_EFFECT);
		ImGui::TreePop();
	}

	edited |= ComboEnum("Chunk type", data.chunkType, chunkTypeNames);
	edited |= Editor("Time limit", data.timeLimit);
	edited |= Editor("Death plane height", data.deathPlaneHeight);
	edited |= Editor("Noise time", data.noiseTime);

	edited |= ComboEnum("Clear type", data.clearType, clearTypeNames);

	//csl::ut::MoveArray<uint8_t> staticSectors;
	//csl::ut::MoveArray<uint8_t> dynamicSectors;

	edited |= Editor("Unk1", data.unk1);
	edited |= PerRankEditor("Rank times", data.rank);

	ImGui::PopID();

	return edited;
}

bool Editor(const char* label, app::level::PlayerInformation& playerInfo)
{
	bool edited{};
	ImGui::PushID(label);

	//edited |= Editor("playerObject", playerInfo.playerObject);
	edited |= Editor("Position", playerInfo.position);
	edited |= Editor("Rotation", playerInfo.rotation);
	edited |= Editor("dword80", playerInfo.dword80);
	edited |= Editor("vector470", playerInfo.vector470);
	edited |= Editor("vector490", playerInfo.vector490);
	edited |= Editor("vector4B0", playerInfo.vector4B0);
	edited |= Editor("vector4D0", playerInfo.vector4D0);
	edited |= Editor("vector4F0", playerInfo.vector4F0);
	edited |= Editor("vector4110", playerInfo.vector4110);
	edited |= Editor("vector4130", playerInfo.vector4130);
	edited |= Editor("vector4150", playerInfo.vector4150);
	edited |= Editor("vector4170", playerInfo.vector4170);
	edited |= Editor("vector4190", playerInfo.vector4190);
	edited |= Editor("vector41B0", playerInfo.vector41B0);
	edited |= Editor("vector41D0", playerInfo.vector41D0);
	edited |= Editor("vector41F0", playerInfo.vector41F0);
	edited |= Editor("vector4210", playerInfo.vector4210);
	edited |= Editor("vector4230", playerInfo.vector4230);
	edited |= Editor("byte250", playerInfo.byte250);
	edited |= Editor("vector2254", playerInfo.vector2254);
	edited |= Editor("vector2260", playerInfo.vector2260);
	edited |= Editor("Boost gauge count", playerInfo.boostGaugeCount);
	edited |= Editor("byte274", playerInfo.byte274);
	edited |= Editor("Stamina", playerInfo.stamina);
	edited |= Editor("dword280", playerInfo.dword280);
	edited |= Editor("byte288", playerInfo.byte288);
	edited |= Editor("byte28A", playerInfo.byte28A);
	edited |= Editor("dword28C", playerInfo.dword28C);
	edited |= Editor("dword294", playerInfo.dword294);
	edited |= Editor("dword29C", playerInfo.dword29C);
	edited |= Editor("byte2A4", playerInfo.byte2A4);
	edited |= Editor("byte2A6", playerInfo.byte2A6);
	edited |= Editor("byte2A8", playerInfo.byte2A8);
	edited |= Editor("byte2AA", playerInfo.byte2AA);
	edited |= Editor("Ground normal", playerInfo.groundNormal);
	edited |= Editor("Altitude", playerInfo.altitude);
	edited |= Editor("vector42E0", playerInfo.vector42E0);
	edited |= Editor("dword300", playerInfo.dword300);
	edited |= Editor("dword308", playerInfo.dword308);
	edited |= Editor("byte310", playerInfo.byte310);
	edited |= Editor("byte312", playerInfo.byte312);
	edited |= Editor("byte314", playerInfo.byte314);
	edited |= Editor("byte316", playerInfo.byte316);
	edited |= Editor("byte318", playerInfo.byte318);
	edited |= Editor("byte31A", playerInfo.byte31A);
	edited |= Editor("byte31C", playerInfo.byte31C);
	edited |= Editor("byte31E", playerInfo.byte31E);
	edited |= Editor("byte320", playerInfo.byte320);
	edited |= Editor("byte322", playerInfo.byte322);
	edited |= Editor("byte324", playerInfo.byte324);
	edited |= Editor("byte326", playerInfo.byte326);
	edited |= Editor("byte328", playerInfo.byte328);
	edited |= Editor("byte32A", playerInfo.byte32A);
	edited |= Editor("byte32C", playerInfo.byte32C);
	edited |= Editor("byte32E", playerInfo.byte32E);
	edited |= Editor("byte330", playerInfo.byte330);
	edited |= Editor("byte332", playerInfo.byte332);
	edited |= Editor("byte334", playerInfo.byte334);
	edited |= Editor("byte336", playerInfo.byte336);
	edited |= Editor("byte338", playerInfo.byte338);
	edited |= Editor("byte33A", playerInfo.byte33A);
	edited |= Editor("byte33C", playerInfo.byte33C);
	edited |= Editor("byte33E", playerInfo.byte33E);
	edited |= Editor("byte340", playerInfo.byte340);
	edited |= Editor("byte342", playerInfo.byte342);
	edited |= Editor("byte344", playerInfo.byte344);
	edited |= Editor("byte346", playerInfo.byte346);
	edited |= Editor("byte348", playerInfo.byte348);
	edited |= Editor("byte34A", playerInfo.byte34A);
	edited |= Editor("byte34C", playerInfo.byte34C);
	edited |= Editor("byte34E", playerInfo.byte34E);
	edited |= Editor("byte350", playerInfo.byte350);
	edited |= Editor("byte352", playerInfo.byte352);
	edited |= Editor("byte354", playerInfo.byte354);
	edited |= Editor("byte356", playerInfo.byte356);
	edited |= Editor("byte358", playerInfo.byte358);
	edited |= Editor("byte35A", playerInfo.byte35A);
	edited |= Editor("byte35C", playerInfo.byte35C);
	edited |= Editor("byte35E", playerInfo.byte35E);
	edited |= Editor("byte360", playerInfo.byte360);
	edited |= Editor("byte362", playerInfo.byte362);
	edited |= Editor("byte364", playerInfo.byte364);
	edited |= Editor("byte366", playerInfo.byte366);
	edited |= Editor("byte368", playerInfo.byte368);
	edited |= Editor("byte36A", playerInfo.byte36A);
	edited |= Editor("byte36C", playerInfo.byte36C);
	edited |= Editor("byte36E", playerInfo.byte36E);
	edited |= Editor("byte370", playerInfo.byte370);
	edited |= Editor("byte372", playerInfo.byte372);
	edited |= Editor("byte374", playerInfo.byte374);
	edited |= Editor("byte376", playerInfo.byte376);
	edited |= Editor("byte378", playerInfo.byte378);
	edited |= Editor("byte37A", playerInfo.byte37A);
	edited |= Editor("byte37C", playerInfo.byte37C);
	edited |= Editor("byte37E", playerInfo.byte37E);
	edited |= Editor("byte380", playerInfo.byte380);
	edited |= Editor("dword384", playerInfo.dword384);
	edited |= Editor("dword38C", playerInfo.dword38C);
	edited |= Editor("byte394", playerInfo.byte394);
	edited |= Editor("byte396", playerInfo.byte396);
	edited |= Editor("byte398", playerInfo.byte398);
	//edited |= Editor("unk3A0", playerInfo.unk3A0);
	edited |= Editor("byte460", playerInfo.byte460);
	edited |= Editor("byte464", playerInfo.byte464);
	edited |= Editor("vector3470", playerInfo.vector3470);
	edited |= Editor("byte490", playerInfo.byte490);
	edited |= Editor("float494", playerInfo.float494);
	edited |= Editor("float49C", playerInfo.float49C);
	edited |= Editor("float4A4", playerInfo.float4A4);
	edited |= Editor("float4AC", playerInfo.float4AC);
	edited |= Editor("float4B4", playerInfo.float4B4);
	edited |= Editor("transform44C0", playerInfo.transform44C0);

	ImGui::PopID();
	return edited;
}
