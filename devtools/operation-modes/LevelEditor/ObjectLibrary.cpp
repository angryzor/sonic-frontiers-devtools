#include "../../Pch.h"
#include "ObjectLibrary.h"
#include "LevelEditor.h"
#include "../../common/Icons.h"
#include "../../imgui/ImGuiFileDialog.h"
#include "../../ResourceBrowser.h"

using namespace hh::fnd;
using namespace hh::game;

ObjectLibrary::ObjectLibrary(csl::fnd::IAllocator* allocator, LevelEditor& levelEditor) : BaseObject{ allocator }, levelEditor{ levelEditor }
{
}

void ObjectLibrary::Render() {
	auto* registry = GameObjectSystem::GetInstance()->gameObjectRegistry;

	const ImGuiWindowFlags windowFlags
		= 0;

	ImGui::SetNextWindowSize(ImVec2(250, ImGui::GetMainViewport()->WorkSize.y - 100), ImGuiCond_Once);
	if (ImGui::Begin("Object library", NULL, windowFlags)) {
		for (auto* obj : registry->GetGameObjectClasses()) {
			if (ImGui::Selectable(obj->pName, selectedClass == obj))
				selectedClass = obj;
			if (selectedClass == obj)
				ImGui::SetItemDefaultFocus();
		}
		if (selectedClass && ImGui::Button("Place"))
			levelEditor.objectClassToPlace = selectedClass;
	}
	ImGui::End();
}
