#include "TimeService.h"

#include <ui/common/editors/Basic.h>

bool Editor(const char* label, app::game::Timestamp& timestamp) {
    bool edited{};

    if (ImGui::TreeNode(label)) {
        edited |= Editor("Days", timestamp.days);
        edited |= Editor("Hours", timestamp.hours);
        edited |= Editor("Minutes", timestamp.minutes);
        edited |= Editor("Seconds", timestamp.seconds);
        ImGui::TreePop();
    }

    if (edited)
        timestamp.Normalize();

    return edited;
}
