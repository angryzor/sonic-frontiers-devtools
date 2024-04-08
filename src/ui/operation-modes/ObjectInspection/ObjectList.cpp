#include "ObjectList.h"
#include "ObjectInspection.h"
#include <ui/common/Icons.h>

using namespace hh::fnd;
using namespace hh::game;

ObjectList::ObjectList(csl::fnd::IAllocator* allocator, ObjectInspection& objectInspection) : BaseObject{ allocator }, objectInspection{ objectInspection }
{
}

void ObjectList::RenderObjectTreeNode(GameObject* obj) {
	ImGuiTreeNodeFlags nodeflags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

	if (objectInspection.focusedObject == obj)
		nodeflags |= ImGuiTreeNodeFlags_Selected;

	auto* transform = obj->GetComponent<GOCTransform>();

	if (transform && transform->GetChildren().size() != 0) {
		bool isOpen = ImGui::TreeNodeEx(obj, nodeflags, "%s", obj->pObjectName.c_str());

		if (ImGui::IsItemClicked())
			objectInspection.focusedObject = obj;
		
		if (isOpen) {
			for (auto& child : transform->GetChildren()) {
				RenderObjectTreeNode(child.GetOwnerGameObject());
			}
			ImGui::TreePop();
		}
	}
	else {
		ImGui::TreeNodeEx(obj, nodeflags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen, "%s", obj->pObjectName.c_str());

		if (ImGui::IsItemClicked())
			objectInspection.focusedObject = obj;
	}
}

void ObjectList::Render() {
	const ImGuiWindowFlags windowFlags
		= 0;

	ImGui::SetNextWindowSize(ImVec2(250, ImGui::GetMainViewport()->WorkSize.y - 100), ImGuiCond_Once);
	if (ImGui::Begin("Objects", NULL, windowFlags)) {
		if (ImGui::BeginTabBar("Object list views")) {
			if (ImGui::BeginTabItem("Tree view")) {
				if (ImGui::BeginChild("Content")) {
					for (auto* layer : GameManager::GetInstance()->m_GameObjectLayers) {
						for (auto* obj : layer->objects) {
							auto* transform = obj->GetComponent<GOCTransform>();

							if (!transform || !transform->IsExistParent())
								RenderObjectTreeNode(obj);
						}
					}
				}
				ImGui::EndChild();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Layer view")) {
				if (ImGui::BeginChild("Content")) {
					for (auto* layer : GameManager::GetInstance()->m_GameObjectLayers) {
						if (layer->objects.size() != 0 && ImGui::TreeNode(layer, layer->name)) {
							for (auto* obj : layer->objects) {
								ImGuiTreeNodeFlags nodeflags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

								if (objectInspection.focusedObject == obj)
									nodeflags |= ImGuiTreeNodeFlags_Selected;

								ImGui::TreeNodeEx(obj, nodeflags, "%s", obj->pObjectName.c_str());

								if (ImGui::IsItemClicked())
									objectInspection.focusedObject = obj;
							}
							ImGui::TreePop();
						}
					}
				}
				ImGui::EndChild();
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
	}
	ImGui::End();
}
