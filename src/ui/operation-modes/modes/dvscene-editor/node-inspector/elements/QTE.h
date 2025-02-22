#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    const char* qteTypeNames[] = {
        "PRESS_PROMPT",
        "MASH",
        "RED_CIRCLE",
        "THE_END_VARIANT",
        "UNK0"
    };

    const char* qteButtonNames[] = {
        "A",
        "B",
        "X",
        "Y",
        "LB_RB",
        "LB",
        "RB",
        "MASH_A",
        "MASH_B",
        "MASH_X",
        "MASH_Y",
        "MASH_LB_RB",
        "MASH_LB",
        "MASH_RB",
        "X_LEFTRIGHT",
        "X_LEFTRIGHT_ALT",
        "RB_ALT"
    };

    template<>
#ifdef DEVTOOLS_TARGET_SDK_rangers
    void RenderElementInspector<1024>(hh::dv::DvElementBase* element) {
#elif DEVTOOLS_TARGET_SDK_miller
    void RenderElementInspector<1026>(hh::dv::DvElementBase* element) {
#endif
        auto* elem = reinterpret_cast<app::dv::DvElementQTE*>(element);
        auto* data = elem->GetData();
		int curQteType = static_cast<int>(data->qteType);
        if (ImGui::Combo("QTE Type", &curQteType, qteTypeNames, 5))
			data->qteType = static_cast<app::dv::DvElementQTE::Data::QTEType>(curQteType);
        int curQteBtn = static_cast<int>(data->qteButton);
        if (ImGui::Combo("QTE Button", &curQteBtn, qteButtonNames, 17))
            data->qteButton = static_cast<app::dv::DvElementQTE::Data::QTEButton>(curQteBtn);
        if(ImGui::TreeNode("Red Circle")){
            Editor("Size", data->redCircleSize);
            Editor("Thickness", data->redCircleThickness);
            Editor("Outline Thickness", data->redCircleOutlineThickness);
            ImGui::TreePop();
        }
        if(ImGui::TreeNode("White Line")){
            Editor("Speed", data->whiteLineSpeed);
            Editor("Thickness", data->whiteLineThickness);
            Editor("Outline Thickness", data->whiteLineOutlineThickness);
            ImGui::TreePop();
        }
        Editor("Multiplier", data->multiplier);
        Editor("Fail Count", data->failCount);
        Editor("QTE Name", data->qteName);
        Editor("Unk0", data->unk0);
        Editor("Unk1", data->unk1);
        Editor("Unk2", data->unk2);
#ifdef DEVTOOLS_TARGET_SDK_rangers
        Editor("Unk5", data->unk5);
#elif DEVTOOLS_TARGET_SDK_miller
        Editor("Unk3", data->unk3);
        Editor("Unk4", data->unk4);
#endif
        Editor("Position", data->offset);
        Editor("Sound Cue Name", data->soundCueName);
    }
}
