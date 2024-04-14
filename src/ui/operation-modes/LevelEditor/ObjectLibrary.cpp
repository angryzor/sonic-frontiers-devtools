#include "ObjectLibrary.h"
#include "LevelEditor.h"
#include <ui/common/Icons.h>

using namespace hh::fnd;
using namespace hh::game;

ObjectLibrary::ObjectLibrary(csl::fnd::IAllocator* allocator, LevelEditor& levelEditor) : BaseObject{ allocator }, levelEditor{ levelEditor }
{
}

void ObjectLibrary::Render() {
	auto* registry = GameObjectSystem::GetInstance()->gameObjectRegistry;

	const ImGuiWindowFlags windowFlags
		= 0;

	ImGui::SetNextWindowSize(ImVec2(250, ImGui::GetMainViewport()->WorkSize.y - 100), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Object library", NULL, windowFlags)) {
		if (levelEditor.focusedChunk) {
			const char* targetLayerPreview = "<none>";

			for (auto* layer : levelEditor.focusedChunk->GetLayers())
				if (layer == levelEditor.placeTargetLayer)
					targetLayerPreview = layer->GetName();

			if (ImGui::BeginCombo("Target placement layer", targetLayerPreview)) {
				for (auto* layer : levelEditor.focusedChunk->GetLayers()) {
					if (ImGui::Selectable(layer->GetName(), layer == levelEditor.placeTargetLayer))
						levelEditor.placeTargetLayer = layer;
					if (levelEditor.placeTargetLayer == layer)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			if (levelEditor.placeTargetLayer) {
				if (levelEditor.objectClassToPlace) {
					bool clickedStop = ImGui::Button("Stop placing");
					ImGui::SameLine();
					ImGui::Text("Placing %s", levelEditor.objectClassToPlace->pName);
					if (clickedStop)
						levelEditor.objectClassToPlace = nullptr;
				}
				else {
					if (!selectedClass)
						ImGui::BeginDisabled();
					if (ImGui::Button("Place"))
						levelEditor.objectClassToPlace = selectedClass;
					if (!selectedClass)
						ImGui::EndDisabled();
				}
			}
		}
		if (ImGui::BeginChild("List of objects")) {
			for (auto* obj : registry->GetGameObjectClasses()) {
				if (ImGui::Selectable(obj->pName, selectedClass == obj)) {
					selectedClass = obj;

					if (levelEditor.objectClassToPlace)
						levelEditor.objectClassToPlace = selectedClass;
				}
				if (selectedClass == obj)
					ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndChild();
	}
	ImGui::End();
}
