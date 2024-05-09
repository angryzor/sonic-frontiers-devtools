#include "ObjectDataInspector.h"
#include "LevelEditor.h"
#include <ui/common/editors/ObjectData.h>
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
			bool edited = Editor("Focused object", *focusedObject);

			if (edited || ImGui::IsItemDeactivatedAfterEdit()) {
				hh::dbg::MsgParamChangedInEditor msg{};

				levelEditor.NotifyActiveObject(msg);
				// The only reason I do this is because apparently sending MsgParamChanged sets the debug visual visibility to false on volumes??
				levelEditor.NotifySelectedObject();
				levelEditor.NotifyUpdatedObject();

				hh::dbg::MsgUpdateSetEditor msg3{};
			}

			if (ImGui::IsItemDeactivatedAfterEdit()) {
				auto status = levelEditor.focusedChunk->GetWorldObjectStatusByObjectId(focusedObject->id);
				auto idx = levelEditor.focusedChunk->GetObjectIndexById(focusedObject->id);

				if (status.objectData && idx != -1) {
					levelEditor.focusedChunk->DespawnByIndex(idx);
					levelEditor.focusedChunk->ShutdownPendingObjects();
					levelEditor.focusedChunk->SpawnByIndex(idx, nullptr);
					levelEditor.NotifySelectedObject();
					
					//for (auto* obj : GameManager::GetInstance()->objects) {
					//	hh::dbg::MsgUpdateActiveObjectInEditor msg{};
					//	obj->SendMessageImm(msg);
					//}
				}
			}
		}
	}
	ImGui::End();
}

