#include "ObjectDataInspector.h"
#include "LevelEditor.h"
#include <ui/common/ObjectDataEditor.h>
#include <imgui_internal.h>

using namespace hh::fnd;
using namespace hh::game;

ObjectDataInspector::ObjectDataInspector(csl::fnd::IAllocator* allocator, LevelEditor& levelEditor) : BaseObject{ allocator }, levelEditor{ levelEditor }
{
}

void ObjectDataInspector::Render() {
	const ImGuiWindowFlags windowFlags
		= 0;

	ImGui::SetNextWindowPos(ImVec2(ImGui::GetMainViewport()->WorkSize.x, 100), ImGuiCond_FirstUseEver, ImVec2(1, 0));
	ImGui::SetNextWindowSize(ImVec2(800, ImGui::GetMainViewport()->WorkSize.y - 140), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Object data inspector", NULL, windowFlags)) {
		if (levelEditor.focusedObjects.size() == 0) {
			ImGui::Text("Select an object in the left pane.");
		}
		else if (levelEditor.focusedObjects.size() > 1) {
			ImGui::Text("Multiple objects selected");
		}
		else {
			auto focusedObject = levelEditor.focusedObjects[0];
			ObjectDataEditor::Render(focusedObject);

			if (ImGui::IsItemEdited()) {
				if (auto* obj = levelEditor.focusedChunk->GetGameObjectByObjectId(focusedObject->id)) {
					hh::dbg::MsgParamChangedInEditor msg{};

					obj->ProcessMessage(msg);
				}
			}

			if (ImGui::IsItemDeactivatedAfterEdit()) {
				auto status = levelEditor.focusedChunk->GetWorldObjectStatusByObjectId(focusedObject->id);
				auto idx = levelEditor.focusedChunk->GetObjectIndexById(focusedObject->id);

				if (status.objectData && idx != -1) {
					levelEditor.focusedChunk->DespawnByIndex(idx);
					status.Restart();
				}
			}
		}
	}
	ImGui::End();
}

