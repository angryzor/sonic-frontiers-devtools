#include "EventPlayer.h"
#include <ui/common/editors/Basic.h>
#include <ui/common/viewers/Basic.h>

void RenderGameServiceInspector(app::evt::EventPlayer& service)
{
    Viewer("eventPlayerListeners amount", service.listeners.size());
    if (ImGui::TreeNode("EventSceneManager")) {
        Viewer("evtScenesCount", service.evtSceneMgr->evtScenes.size());
        if (ImGui::TreeNode("cutsceneNames")) {
            for (auto& x : service.evtSceneMgr->cutsceneNames)
                ImGui::Text(x.c_str());
            ImGui::TreePop();
        }
        if (service.evtSceneMgr->evtScenes.size() > 0) {
            if (auto* x = service.evtSceneMgr->evtScenes[0]) {
                Editor("unk0", x->unk0);
                Editor("unk1", x->unk1);
                Editor("unk2", x->unk2);
                Editor("unk3", x->unk3);
            }
        }
        ImGui::TreePop();
    }
    if (ImGui::TreeNode("EventEnvironmentManager")) {
        Viewer("evtEnvsCount", service.evtEnvMgr->evtEnvs.size());
        ImGui::TreePop();
    }
    if (service.IsntPlaying()) {
        if (ImGui::Button("TestPlay")) {
            app::evt::EventSetupData setupData{};
            setupData.Setup("ev6020_mov");
            service.PlayEvent(&setupData);
        }
    }
}
