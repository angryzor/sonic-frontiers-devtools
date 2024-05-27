#include "ObjCameraVolume.h"

void GameObjectInspector(app_cmn::camera::ObjCameraVolume& cameraVolume) {
    ImGui::SeparatorText("Players inside volume");
    for (size_t i = 0; i < std::min(8ull, cameraVolume.enterCountPerPlayer.size()); i++) {
        char txt[20];
        snprintf(txt, sizeof(txt), "Player %zd", i);
        CheckboxFlags(txt, cameraVolume.playersInsideFlags, static_cast<uint8_t>(1 << i));
    }

    size_t i{};
    ImGui::SeparatorText("Player entry count");
    for (auto& count : cameraVolume.enterCountPerPlayer) {
        char txt[20];
        snprintf(txt, sizeof(txt), "Player %zd", i++);
        Editor(txt, count);
    }
}
