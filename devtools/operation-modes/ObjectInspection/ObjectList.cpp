#include "../../Pch.h"
#include "ObjectList.h"
#include "ObjectInspection.h"
#include "../../common/Icons.h"

using namespace hh::fnd;
using namespace hh::game;

ObjectList::ObjectList(csl::fnd::IAllocator* allocator, ObjectInspection& objectInspection) : BaseObject{ allocator }, objectInspection{ objectInspection }
{
}

void ObjectList::Render() {
	const ImGuiWindowFlags windowFlags
		= ImGuiWindowFlags_MenuBar;

	ImGui::SetNextWindowSize(ImVec2(250, ImGui::GetMainViewport()->WorkSize.y - 100), ImGuiCond_Once);
	if (ImGui::Begin("Objects", NULL, windowFlags)) {
		GameObject* objectClicked = nullptr;

		for (auto* layer : GameManager::GetInstance()->m_GameObjectLayers) {
			if (layer->objects.size() != 0 && ImGui::TreeNode(layer, layer->name)) {
				for (auto* obj : layer->objects) {
					const char* objName = obj->pObjectName;

					ImGuiTreeNodeFlags nodeflags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

					if (objectInspection.focusedObject == obj)
						nodeflags |= ImGuiTreeNodeFlags_Selected;

					//RenderIcon(IconId::OBJECT, objectInspection.desktop);
					//ImGui::SameLine();
					ImGui::TreeNodeEx(obj, nodeflags, objName == nullptr ? "" : objName);

					if (ImGui::IsItemClicked())
						objectClicked = obj;
				}
				ImGui::TreePop();
			}
		}

		if (objectClicked)
			objectInspection.focusedObject = objectClicked;
	}
	ImGui::End();
}
