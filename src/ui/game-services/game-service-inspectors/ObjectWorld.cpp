#include "ObjectWorld.h"
#include <ui/Desktop.h>
#include <ui/resources/editors/ResObjectWorldEditor.h>
#include <ui/common/inputs/Basic.h>
#include <ui/common/editors/Basic.h>
#include <ui/common/viewers/Basic.h>
#include <ui/common/editors/ObjectData.h>

using namespace hh::game;

void RenderGameServiceInspector(hh::game::ObjectWorld& objWorld) {
	int i = 0;
	for (auto* chunk : objWorld.GetWorldChunks()) {
		if (ImGui::TreeNode(chunk, "Chunk %d", i++)) {
			bool editorMode = chunk->IsStatusEditor();

			if (ImGui::Checkbox("Editor mode", &editorMode))
				chunk->SetEditorStatus(editorMode);

			if (ImGui::CollapsingHeader("Layers")) {
				for (auto* layer : chunk->GetLayers()) {
					if (ImGui::TreeNode(layer, layer->GetName())) {
						bool enabled = layer->IsEnable();

						if (ImGui::Checkbox("Enabled", &enabled))
							chunk->SetLayerEnabled(layer->GetName(), enabled);

						if (ImGui::Button("Edit resource"))
							ResObjectWorldEditor::Create(Desktop::instance->GetAllocator(), layer->GetResource());

						ImGui::TreePop();
					}
				}
			}

			if (ImGui::CollapsingHeader("Object statuses")) {
				for (auto& status : chunk->GetObjectStatuses()) {
					if (ImGui::TreeNode(&status, status.objectData->name)) {
						CheckboxFlags("Enabled", status.flags, WorldObjectStatus::Flag::ENABLED);
						CheckboxFlags("Is alive", status.flags, WorldObjectStatus::Flag::IS_ALIVE);
						CheckboxFlags("Shutdown", status.flags, WorldObjectStatus::Flag::SHUTDOWN);
						CheckboxFlags("No restart", status.flags, WorldObjectStatus::Flag::NO_RESTART);

						if (ImGui::Button("Spawn"))
							chunk->Spawn(status.objectData);
						if (ImGui::Button("Despawn"))
							chunk->Despawn(status.objectData);
						if (ImGui::Button("Shutdown"))
							status.Shutdown();
						if (ImGui::Button("Restart"))
							status.Restart();

#ifndef DEVTOOLS_TARGET_SDK_wars
						Editor("Spawn priority", status.spawnPriority);
#endif

						ImGui::SeparatorText("Object Data");
						Editor("Object data", *status.objectData);

						ImGui::TreePop();
					}
				}
			}
			ImGui::TreePop();
		}
	}
}
