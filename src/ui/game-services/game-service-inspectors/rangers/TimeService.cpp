#include <ui/game-services/game-service-inspectors/TimeService.h>

bool Editor(const char* label, app::game::TimeService::TimeProgressSpeed& speed);

#include <ui/common/editors/TimeService.h>
#include <ui/common/editors/Basic.h>
#include <ui/common/viewers/Basic.h>

bool Editor(const char* label, app::game::TimeService::TimeProgressSpeed& speed) {
    bool edited{};

    if (ImGui::TreeNode(label)) {
        edited |= Editor("Start time", speed.start);
        edited |= Editor("Speed", speed.speed);
        ImGui::TreePop();
    }

    return edited;
}

void RenderGameServiceInspector(app::game::TimeService& service) {
    Editor("Current time", service.currentTime);
    //Editor("Previous time", service.previousTime);

    Editor("Start of night in hours", service.startOfNightInHours);
    Editor("End of night in hours", service.endOfNightInHours);

    Editor("Time progress speeds", service.timeProgressSpeeds);

    Viewer("Night times loaded", service.sceneDataLoaded);
    Editor("Time progress enabled", service.timeProgressEnabled);
    Editor("Using fixed time", service.fixedTime);

    Viewer("Seconds until next update", service.secondsUntilNextUpdate);
}
