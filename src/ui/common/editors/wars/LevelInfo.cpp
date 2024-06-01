#include <ui/common/editors/Basic.h>
#include <ui/common/inputs/Basic.h>

using namespace app::level;

static const char* chunkTypeNames[] = { "INITIAL", "AFTER" };
static const char* characterNames[] = { "SONIC", "BUDDY", "TAG", "CLASSIC", "NONE", "SHADOW" };

bool Editor(const char* label, app::player::CharacterId& charId) {
	return ComboEnum(label, charId, characterNames);
}

template<typename T, size_t Len>
bool Editor(const char* label, StageData::List<T, Len>& list) {
	bool edited{};

	if (ImGui::TreeNode(label)) {
		edited |= Editor("Count", list.count);
		edited |= Editor("Items", list.items);
		ImGui::TreePop();
	}

	return edited;
}

bool Editor(const char* label, StageData& data) {
	bool edited{};

	ImGui::PushID(&data);

	edited |= Editor("Name", data.name);
	edited |= Editor("Directory", data.dir);
	edited |= Editor("Background music", data.bgm);
	edited |= Editor("Ambience", data.amb);
	edited |= Editor("Background music retry start", data.bgmRetryStart);
	edited |= Editor("Background music retry fade", data.bgmRetryFade);

	int missingIdx{ -1 };
	edited |= DragScalar("Stage number", data.stageNumber, &missingIdx);
	edited |= DragScalar("Category number", data.categoryNumber, &missingIdx);
	edited |= DragScalar("Sequence number", data.sequenceNumber, &missingIdx);

	if (ImGui::TreeNode("Attributes")) {
		edited |= CheckboxFlags("Boss", data.attributeFlags, StageData::AttributeFlag::BOSS);
		edited |= CheckboxFlags("Extra", data.attributeFlags, StageData::AttributeFlag::EXTRA);
		edited |= CheckboxFlags("Special", data.attributeFlags, StageData::AttributeFlag::SPECIAL);
		edited |= CheckboxFlags("DLC", data.attributeFlags, StageData::AttributeFlag::DLC);
		edited |= CheckboxFlags("Credit", data.attributeFlags, StageData::AttributeFlag::CREDIT);
		edited |= CheckboxFlags("Character change", data.attributeFlags, StageData::AttributeFlag::CHARA_CHANGE);
		edited |= CheckboxFlags("Shift BP", data.attributeFlags, StageData::AttributeFlag::SHIFTBP);
		edited |= CheckboxFlags("Top view", data.attributeFlags, StageData::AttributeFlag::TOP_VIEW);
		edited |= CheckboxFlags("AppGfxParam", data.attributeFlags, StageData::AttributeFlag::APP_GFX_PARAM);
		edited |= CheckboxFlags("Number ring", data.attributeFlags, StageData::AttributeFlag::NUMBER_RING);
		edited |= CheckboxFlags("Yellow moon ring / Title", data.attributeFlags, StageData::AttributeFlag::YELLOW_MOON_RING_OR_TITLE);
		edited |= CheckboxFlags("Use SOS", data.attributeFlags, StageData::AttributeFlag::USE_SOS);
		ImGui::TreePop();
	}

    edited |= Editor("Character ID", data.characterId);

    edited |= Editor("Static sectors", data.staticSectors);
    edited |= Editor("Dynamic sectors", data.dynamicSectors);
    edited |= Editor("Opening events", data.openingEvents);
    edited |= Editor("Cleared events", data.clearedEvents);

	edited |= ComboEnum("Chunk type", data.chunkType, chunkTypeNames);

	ImGui::PopID();

	return edited;
}


// template<typename T>
// bool Editor(const char* label, PlayerInformation::Unk1<T>& data) {
// 	bool edited{};
// 	ImGui::PushID(label);

// 	char checkboxLabel[100];
// 	snprintf(checkboxLabel, sizeof(checkboxLabel), "%s###%s", data.isSet ? "" : label, "Is Set");

// 	edited |= Editor(checkboxLabel, data.isSet);
		
// 	if (data.isSet) {
// 		ImGui::SameLine();
// 		edited |= Editor(label, data.value);
// 	}

// 	ImGui::PopID();
// 	return edited;
// }

// bool Editor(const char* label, PlayerInformation& playerInfo)
// {
// 	bool edited{};
// 	ImGui::PushID(label);
// 	//Unk1<hh::fnd::Handle<hh::fnd::Messenger>> playerObject;
// 	edited |= Editor("characterId", playerInfo.characterId);
// 	edited |= Editor("position", playerInfo.position);
// 	edited |= Editor("vector440", playerInfo.vector440);
// 	edited |= Editor("rotation", playerInfo.rotation);
// 	edited |= Editor("dword80", playerInfo.dword80);
// 	edited |= Editor("vector490", playerInfo.vector490);
// 	edited |= Editor("vector4B0", playerInfo.vector4B0);
// 	edited |= Editor("vector4D0", playerInfo.vector4D0);
// 	edited |= Editor("vector4F0", playerInfo.vector4F0);
// 	edited |= Editor("vector4110", playerInfo.vector4110);
// 	edited |= Editor("vector4130", playerInfo.vector4130);
// 	edited |= Editor("vector4150", playerInfo.vector4150);
// 	edited |= Editor("vector4170", playerInfo.vector4170);
// 	edited |= Editor("vector4190", playerInfo.vector4190);
// 	edited |= Editor("vector41B0", playerInfo.vector41B0);
// 	edited |= Editor("vector41D0", playerInfo.vector41D0);
// 	edited |= Editor("vector41F0", playerInfo.vector41F0);
// 	edited |= Editor("vector4210", playerInfo.vector4210);
// 	edited |= Editor("vector4230", playerInfo.vector4230);
// 	edited |= Editor("vector4250", playerInfo.vector4250);
// 	edited |= Editor("vector4270", playerInfo.vector4270);
// 	edited |= Editor("word290", playerInfo.word290);
// 	//if (playerInfo->qword294.isSet) Editor("qword294", playerInfo->qword294.value);
// 	//if (playerInfo->qword2A0.isSet) Editor("qword2A0", playerInfo->qword2A0.value);
// 	edited |= Editor("Boost gauge count", playerInfo.boostGaugeCount);
// 	edited |= Editor("Stamina", playerInfo.stamina);
// 	edited |= Editor("dword2BC", playerInfo.dword2BC);
// 	edited |= Editor("word2C4", playerInfo.word2C4);
// 	edited |= Editor("dword2C8", playerInfo.dword2C8);
// 	edited |= Editor("Quick CyLoop amount", playerInfo.quickCyloopAmount);
// 	edited |= Editor("qword2D8", playerInfo.qword2D8);
// 	edited |= Editor("dword2E4", playerInfo.dword2E4);
// 	//uint32_t dword2EC;
// 	//uint32_t dword2F0;
// 	edited |= Editor("Ground normal", playerInfo.groundNormal);
// 	edited |= Editor("Altitude", playerInfo.altitude);
// 	edited |= Editor("vector4330", playerInfo.vector4330);
// 	edited |= Editor("dword350", playerInfo.dword350);
// 	edited |= Editor("dword358", playerInfo.dword358);
// 	//uint32_t dword360;
// 	//uint32_t dword364;
// 	//uint32_t dword368;
// 	//uint32_t dword36C;
// 	//uint32_t dword370;
// 	//uint32_t dword374;
// 	//uint32_t dword378;
// 	//uint32_t dword37C;
// 	//uint32_t dword380;
// 	//uint32_t dword384;
// 	//uint32_t dword388;
// 	//uint32_t dword38C;
// 	//uint32_t dword390;
// 	//uint32_t dword394;
// 	//uint64_t qword398;
// 	//uint32_t dword3A0;
// 	//uint32_t dword3A4;
// 	//uint32_t dword3A8;
// 	//uint32_t dword3AC;
// 	//uint32_t dword3B0;
// 	//uint32_t dword3B4;
// 	//uint32_t dword3B8;
// 	//uint32_t dword3BC;
// 	//if (playerInfo->qword3C0.isSet) Editor("qword3C0", playerInfo->qword3C0.value);
// 	edited |= Editor("Max rings", playerInfo.maxRings);
// 	//uint32_t dword3D4;
// 	//uint32_t dword3D8;
// 	//if (playerInfo->unk13241.isSet) Editor("unk13241", playerInfo->unk13241.value);
// 	edited |= Editor("vector4480", playerInfo.vector4480);
// 	//if (playerInfo->qword4A0.isSet) Editor("qword4A0", playerInfo->qword4A0.value);
// 	edited |= Editor("dword4AC", playerInfo.dword4AC);
// 	edited |= Editor("word4B4", playerInfo.word4B4);
// 	edited |= Editor("dword4B8", playerInfo.dword4B8);
// 	edited |= Editor("dword4C0", playerInfo.dword4C0);
// 	edited |= Editor("transform44F0", playerInfo.transform44F0);
// 	//Unk4 qword510[3];
// 	ImGui::PopID();
// 	return edited;
// }
