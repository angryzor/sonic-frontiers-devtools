#include "../../Pch.h"
#include "ObjectDataInspector.h"
#include "LevelEditor.h"
#include "../../common/ObjectDataEditor.h"

using namespace hh::fnd;
using namespace hh::game;

ObjectDataInspector::ObjectDataInspector(csl::fnd::IAllocator* allocator, LevelEditor& levelEditor) : BaseObject{ allocator }, levelEditor{ levelEditor }
{
}

void ObjectDataInspector::Render() {
	const ImGuiWindowFlags windowFlags
		= 0;

	ImGui::SetNextWindowPos(ImVec2(ImGui::GetMainViewport()->WorkSize.x, 100), ImGuiCond_Once, ImVec2(1, 0));
	ImGui::SetNextWindowSize(ImVec2(800, ImGui::GetMainViewport()->WorkSize.y - 140), ImGuiCond_Once);
	if (ImGui::Begin("Object data inspector", NULL, windowFlags)) {
		if (levelEditor.focusedObject == nullptr) {
			ImGui::Text("Select an object in the left pane.");
		}
		else {
			ObjectDataEditor::Render(levelEditor.focusedObject);

			if (ImGui::IsItemDeactivatedAfterEdit()) {
				auto status = levelEditor.focusedChunk->GetWorldObjectStatusByObjectId(levelEditor.focusedObject->id);
				auto idx = levelEditor.focusedChunk->GetObjectIndexById(levelEditor.focusedObject->id);

				if (status.objectData && idx != -1) {
					levelEditor.focusedChunk->DespawnByIndex(idx);
					status.Restart();
				}
			}
		}
	}
	ImGui::End();
}

