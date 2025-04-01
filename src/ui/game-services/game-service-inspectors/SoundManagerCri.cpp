#include "SoundManagerCri.h"
#include <ui/common/editors/Basic.h>
#include <ui/common/viewers/Basic.h>

void RenderGameServiceInspector(hh::snd::SoundManagerCri& service)
{
	Viewer("GOCSound Count", service.gocSounds.size());
    if(ImGui::TreeNode("Sounds")){
        for(auto& x : service.sounds)
        {
            char buffer[128];
            snprintf(buffer, sizeof(buffer), "0x%zx", (size_t)&x);
            if (ImGui::TreeNode(buffer)) {
                char bufferx[128];
                snprintf(bufferx, sizeof(bufferx), "%f", x.GetPlayTime());
                ImGui::Text(bufferx);
                ImGui::TreePop();
            }
        }
        ImGui::TreePop();
    }
}
