#include "../../Pch.h"
#include "SetObjectList.h"
#include "LevelEditor.h"
#include "../../common/Icons.h"
#include "../../imgui/ImGuiFileDialog.h"
#include "../../ResourceBrowser.h"

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
		ImGui::TreeNodeEx(&objData, nodeflags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen, "%s", objData->name.c_str());
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

		char chunkid[30];
		strcpy_s(chunkid, "<none>");

		int i = 0;
		for (auto* chunk : objWorld->GetWorldChunks()) {
			if (chunk == levelEditor.focusedChunk)
				snprintf(chunkid, sizeof(chunkid), "Chunk %d", i);
			i++;
		}

		if (ImGui::BeginCombo("Chunk to be edited", chunkid)) {
			bool none_selected = levelEditor.focusedChunk == nullptr;
			strcpy_s(chunkid, "<none>");

			if (ImGui::Selectable(chunkid, none_selected))
				levelEditor.SetFocusedChunk(nullptr);
			if (none_selected)
				ImGui::SetItemDefaultFocus();

			i = 0;
			for (auto* chunk : objWorld->GetWorldChunks()) {
				bool is_selected = levelEditor.focusedChunk == chunk;
				snprintf(chunkid, sizeof(chunkid), "Chunk %d", i);

				if (ImGui::Selectable(chunkid, is_selected))
					levelEditor.SetFocusedChunk(chunk);

				if (is_selected)
					ImGui::SetItemDefaultFocus();
				i++;
			}
			ImGui::EndCombo();
		}

		if (levelEditor.focusedChunk && ImGui::BeginTabBar("Object list views")) {
			if (ImGui::BeginTabItem("Tree view")) {
				if (ImGui::BeginChild("Content")) {
					for (auto& status : levelEditor.focusedChunk->GetObjectStatuses()) {
						RenderObjectTreeNode(status.objectData);
						//for (auto* obj : layer->) {
						//	auto* transform = obj->GetComponent<GOCTransform>();

						//	if (!transform || !transform->IsExistParent())
						//		RenderObjectTreeNode(obj);
						//}
					}
				}
				ImGui::EndChild();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Layer view")) {
				if (ImGui::BeginChild("Content")) {
					for (auto* layer : levelEditor.focusedChunk->GetLayers()) {
						if (ImGui::TreeNode(layer, "%s", layer->GetName())) {
							for (auto& obj : layer->GetResource()->GetObjects()) {
								ImGuiTreeNodeFlags nodeflags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

								if (levelEditor.focusedObject == obj)
									nodeflags |= ImGuiTreeNodeFlags_Selected;

								ImGui::TreeNodeEx(obj, nodeflags, "%s", obj->name.c_str());

								if (ImGui::IsItemClicked())
									levelEditor.focusedObject = obj;
							}
							ImGui::TreePop();
						}

						if (ImGui::BeginPopupContextItem("Layer context menu")) {
							if (ImGui::MenuItem("Load from file...")) {
								ResourceBrowser::ShowLoadResourceDialog(layer->GetResource());
							}
							if (ImGui::MenuItem("Export...")) {
								ResourceBrowser::ShowExportResourceDialog(layer->GetResource());
							}
							ImGui::EndPopup();
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
