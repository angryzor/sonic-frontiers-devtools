#pragma once
#include "../Nodes.h"
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    const char* elemPlayTypes[] = {
		"NORMAL",
		"ONESHOT",
		"ALWAYS"
	};

	const char* elemUpdateTimings[] = {
		"ON_EXEC_PATH",
		"ON_PRE_UPDATE",
		"CHARACTER_FIX_POSTURE",
		"ON_POST_UPDATE_CHARACTER",
		"ON_UPDATE_POS",
		"ON_FIX_BONE_POSTURE",
		"ON_EVALUATE_DETAIL_MOTION",
		"CHARACTER_JOB_UPDATE",
		"MODIFY_POSE_AFTER",
		"JOB_REGISTER",
		"MOTION_UPDATE"
	};

    template<>
    bool RenderNodeInspector<12>(char* node) {
		bool changed = false;
        auto* data = reinterpret_cast<hh::dv::DvNodeElement::Data*>(node);
        int type = static_cast<int>(data->elementId);
		if(type >= 1000)
			Viewer("Element ID", elementIDStrings[type - 1000 + hhElementCount]);
		else
			Viewer("Element ID", elementIDStrings[type]);
		int curPlayType = static_cast<int>(data->playType);
		if (changed |= ImGui::Combo("Play Type", &curPlayType, elemPlayTypes, 3))
			data->playType = static_cast<hh::dv::DvNodeElement::PlayType>(curPlayType);
		int curUpdateTiming = static_cast<int>(data->updateTiming);
		if (changed |= ImGui::Combo("Update Timing", &curUpdateTiming, elemUpdateTimings, 11))
			data->updateTiming = static_cast<hh::dv::DvNodeElement::UpdateTiming>(curUpdateTiming);
        ElementFuncType render = GetElementInspectorRender(type);
		if(render){
			ImGui::SeparatorText("Element Properties");
			changed |= render(&node[sizeof(hh::dv::DvNodeElement::Data)]);
		}
		return changed;
    }
}
