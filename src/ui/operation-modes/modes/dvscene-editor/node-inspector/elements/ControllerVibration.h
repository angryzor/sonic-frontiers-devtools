#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    void RenderElementInspector<25>(hh::dv::DvElementBase* element) {
        auto* elem = reinterpret_cast<hh::dv::DvElementControllerVibration*>(element);
        auto& data = elem->binaryData;
        auto* vibMgr = hh::hid::DeviceManagerWin32::GetInstance()->vibrationManager;
        csl::ut::MoveArray32<const char*> vibNames{ element->GetAllocator() };
        int selected = 0;
        int y = 0;
        for (auto x : vibMgr->vibrations) {
            vibNames.push_back(x->name);
            if (strcmp(x->name, data.vibrationName) == 0)
                selected = y;
            y++;
        }

        CheckboxFlags("Ignore End", data.flags, hh::dv::DvElementControllerVibration::Data::Flags::IGNORE_END);
        Editor("Group Name", data.groupName);
        if (ImGui::Combo("Vibration", &selected, vibNames.begin(), vibNames.size()))
            strcpy(data.vibrationName, vibNames[selected]);
        vibNames.clear();
    }
}
