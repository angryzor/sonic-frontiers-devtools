#include "EventPlayer.h"
#include <ui/common/editors/Basic.h>
#include <ui/common/viewers/Basic.h>

void FrameStrEditor(const char* label, hh::fmv::MoviePlayerCri::FrameStr* str) {
    char buffer[256];
    snprintf(buffer, sizeof(buffer), "%s - 0x%zx", label, (size_t)str);
    if (ImGui::TreeNode(buffer)) {
        Editor("framesRunning", str->framesRunning);
        Editor("current Frame", str->currentFrame);
        Editor("width0", str->width0);
        Editor("height0", str->height0);
        Editor("width1", str->width1);
        Editor("height1", str->height1);
        Editor("raw fps", str->rawFps);
        Editor("framerateN", str->framerateN);
        Editor("framerateD", str->framerateD);
        Editor("total Frames", str->totalFrames);
        Editor("unk1", str->unk1);
        ImGui::TreePop();
    }
}

void RenderGameServiceInspector(hh::fmv::MovieManager& service)
{
    if(ImGui::TreeNode("Movies")){
        for (auto& x : service.movies) {
            char buffer[512];
            snprintf(buffer, sizeof(buffer), "%s##%zx", x->moviePlayer->usmFilePath, (size_t)*x);
            if (ImGui::TreeNode(buffer)) {
                auto* movPly = x->moviePlayer;
                Viewer("Filepath", movPly->usmFilePath);
                Viewer("Progress", movPly->secondsRunning);
                Viewer("Frames Running", movPly->framesRunning);
                Editor("qword30", movPly->qword30);
                Editor("dword8c", movPly->dword8C);
                Editor("word90", movPly->word90);
                Editor("word92", movPly->word92);
                Editor("dwordA0", movPly->dwordA0);
                Editor("unk4[0]", movPly->unk4[0]);
                Editor("unk4[1]", movPly->unk4[1]);
                if (ImGui::TreeNode("Texture Infos")) {
                    for (auto& y : movPly->textureInfos) {
                        char xbuffer[128];
                        snprintf(xbuffer, sizeof(xbuffer), "0x%zx", (size_t)&y);
                        if (ImGui::TreeNode(xbuffer)) {
                            Editor("width", y.width);
                            Editor("height", y.height);
                            Viewer("unk0", y.unk0);
                            Editor("unk1", y.unk1);
                            Viewer("unk2", y.unk2);
                            Editor("unk3", y.unk3);
                            Editor("unk4", y.unk4);
                            Editor("unk5", y.unk5);
                            ImGui::TreePop();
                        }
                    }
                    ImGui::TreePop();
                }
                FrameStrEditor("Frame Str0", movPly->frameStr0);
                FrameStrEditor("Frame Str1", movPly->frameStr1);
                FrameStrEditor("Frame Str2", movPly->frameStr2);
                Viewer("isPlaying", movPly->isPlaying);
                Viewer("unk9", movPly->unk9);
                Editor("unk9b", movPly->unk95);
                Viewer("unk10", movPly->unk10);
                Viewer("unk11", movPly->unk11);
                Viewer("unk12", movPly->unk12);
                Viewer("unk13", movPly->unk13);
                Viewer("unk14", movPly->unk14);
                Viewer("unk15", movPly->unk15);
                Viewer("unk16", movPly->unk16);
                Viewer("unk17", movPly->unk17);
                Viewer("unk18", movPly->unk18);
                Viewer("unk19", movPly->unk19);
                Viewer("unk20", movPly->unk20);
                Viewer("unk21", movPly->unk21);
                Viewer("unk22", movPly->unk22);
                Viewer("unk23", movPly->unk23);
                Viewer("unk24", movPly->unk24);
                Viewer("unk25", movPly->unk25);
                Viewer("unk26", movPly->unk26);
                Viewer("unk27", movPly->unk27);
                Viewer("unk28", movPly->unk28);
                ImGui::TreePop();
            }
        }
        ImGui::TreePop();
    }
    Editor("paused", service.paused);
    Viewer("shader name", service.shaderName);
}
