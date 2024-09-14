#include "RespawnTimeManager.h"
#include <ui/resources/editors/ResReflectionEditor.h>
#include <ui/Desktop.h>

#include <ui/common/editors/Basic.h>

void RenderGameServiceInspector(app::game::MonologueService& service) {
    if (ImGui::Button("MonologueParameters"))
        ResReflectionEditor::Create(Desktop::instance->GetAllocator(), service.parametersResource);

    Editor("unk1", service.unk1);
    Editor("unk2", service.unk2);
    Editor("unk3", service.monologueTimeout);
    Editor("unk4", service.timeUntilNextMonologue);
    Editor("unk5", service.unk5);
    Editor("unk6", service.unk6);
    Editor("unk7a", service.unk7a);
}
