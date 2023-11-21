#include "../../Pch.h"
#include "SetObjectList.h"
#include "LevelEditor.h"
#include "../../common/Icons.h"

using namespace hh::fnd;
using namespace hh::game;

SetObjectList::SetObjectList(csl::fnd::IAllocator* allocator, LevelEditor& levelEditor) : BaseObject{ allocator }, levelEditor{ levelEditor }
{
}

void SetObjectList::RenderObjectTreeNode(ObjectData* objData) {
	ImGuiTreeNodeFlags nodeflags = 0;

	if (levelEditor.focusedObject == objData)
		nodeflags |= ImGuiTreeNodeFlags_Selected;

	//auto* transform = objData->GetComponent<GOCTransform>();

	//if (transform && transform->GetChildren().size() != 0) {
	//	if (ImGui::TreeNodeEx(objData, nodeflags, "%s", objData->pObjectName.c_str())) {
	//		for (auto& child : transform->GetChildren()) {
	//			RenderObjectTreeNode(child.GetOwnerGameObject());
	//		}
	//		ImGui::TreePop();
	//	}
	//}
	//else {
		ImGui::TreeNodeEx(&objData, nodeflags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen, "%s", objData->name);
	//}

	if (ImGui::IsItemClicked())
		levelEditor.focusedObject = objData;
}

void SetObjectList::Render() {
	auto* objWorld = GameManager::GetInstance()->GetService<ObjectWorld>();

	const ImGuiWindowFlags windowFlags
		= 0;

	ImGui::SetNextWindowSize(ImVec2(250, ImGui::GetMainViewport()->WorkSize.y - 100), ImGuiCond_Once);
	if (ImGui::Begin("World objects", NULL, windowFlags)) {
		if (objWorld == nullptr) {
			ImGui::Text("ObjectWorld service not active. Load a level first.");
			ImGui::End();
			return;
		}

		if (ImGui::BeginTabBar("Object list views")) {
			if (ImGui::BeginTabItem("Tree view")) {
				if (ImGui::BeginChild("Content")) {
					int i = 0;
					for (auto* chunk : objWorld->GetWorldChunks()) {
						if (ImGui::TreeNode(chunk, "Chunk %d", i++)) {
							for (auto& status : chunk->GetObjectStatuses()) {
								RenderObjectTreeNode(status.objectData);
								//for (auto* obj : layer->) {
								//	auto* transform = obj->GetComponent<GOCTransform>();

								//	if (!transform || !transform->IsExistParent())
								//		RenderObjectTreeNode(obj);
								//}
							}
							ImGui::TreePop();
						}
					}
				}
				ImGui::EndChild();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Layer view")) {
				if (ImGui::BeginChild("Content")) {
					int i = 0;
					for (auto* chunk : objWorld->GetWorldChunks()) {
						if (ImGui::TreeNode(chunk, "Chunk %d", i++)) {
							for (auto* layer : chunk->GetLayers()) {
								if (ImGui::TreeNode(layer, "%s", layer->GetName())) {
									for (auto& obj : layer->GetResource()->GetObjects()) {
										ImGuiTreeNodeFlags nodeflags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

										if (levelEditor.focusedObject == obj)
											nodeflags |= ImGuiTreeNodeFlags_Selected;

										RenderObjectTreeNode(obj);

										ImGui::TreeNodeEx(obj, nodeflags, "%s", obj->name);

										if (ImGui::IsItemClicked())
											levelEditor.focusedObject = obj;
									}
									ImGui::TreePop();
								}
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
