#include "DiEventManager.h"
#include <ui/common/editors/Basic.h>
#include <ui/common/viewers/Basic.h>

void RenderGameServiceInspector(hh::dv::DiEventManager& service)
{
    if(ImGui::TreeNode("Scene Playback Infos")){
        for(auto& x : service.scenePlaybackInfos){
            auto* dvSceneControl = hh::game::GameObjectSystem::GetGameObjectByHandle(x->dvSceneControl);
            char buffer[64];
            snprintf(buffer, sizeof(buffer), "0x%zx", (size_t)&dvSceneControl->listeners);
            if(ImGui::TreeNode(buffer)) {
                Viewer("Cutscene Name", dvSceneControl->cutsceneName);
                Viewer("Cut Count", dvSceneControl->timeline->cuts.size());
                Viewer("Current Cut", dvSceneControl->timeline->currentCut);
                Editor("Current Frame0", dvSceneControl->timeline->currentFrame0);
                Editor("Current Frame1", dvSceneControl->timeline->currentFrame1);
                ImGui::TreePop();
            }
        }
        ImGui::TreePop();
    }

    Viewer("unk0", service.unk0);
    Viewer("unk1", service.unk1);
    Viewer("dvObjectsLayer", service.dvObjectsLayer);
    Viewer("unk3", service.unk3);
    Viewer("currentScenePlaybackInfoId", service.currentScenePlaybackInfoId);
}
