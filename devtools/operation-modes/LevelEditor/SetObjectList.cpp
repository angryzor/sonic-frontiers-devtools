#include "../../Pch.h"
#include "SetObjectList.h"
#include "LevelEditor.h"
#include "../../common/Icons.h"
#include "../../imgui/ImGuiFileDialog.h"
#include "../../ResourceBrowser.h"

using namespace hh::fnd;
using namespace hh::game;

SetObjectListTreeNode::SetObjectListTreeNode(csl::fnd::IAllocator* allocator) : hh::fnd::ReferencedObject{ allocator, true }, children{ allocator } {
}

const char* SetObjectListTreeNode::GetLabel() const {
	if (type == SetObjectListTreeNode::Type::OBJECT)
		return objectInfo.object->name.c_str();
	else
		return groupInfo.label;
}

SetObjectListTreeNode* SetObjectListTreeNode::CreateObjectNode(csl::fnd::IAllocator* allocator, hh::game::ObjectData* objData) {
	auto* node = new (allocator) SetObjectListTreeNode(allocator);
	node->type = SetObjectListTreeNode::Type::OBJECT;
	node->objectInfo.object = objData;
	return node;
}

SetObjectListTreeNode* SetObjectListTreeNode::CreateGroupNode(csl::fnd::IAllocator* allocator, const char* label) {
	auto* node = new (allocator) SetObjectListTreeNode(allocator);
	node->type = SetObjectListTreeNode::Type::GROUP;
	node->groupInfo.label = label;
	return node;
}

SetObjectList::SetObjectList(csl::fnd::IAllocator* allocator, LevelEditor& levelEditor) : BaseObject{ allocator }, levelEditor{ levelEditor }
{
}

void SetObjectList::RenderObjectTreeNode(SetObjectListTreeNode* node) {
	ImGuiTreeNodeFlags nodeflags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

	if (node->type == SetObjectListTreeNode::Type::OBJECT && levelEditor.focusedObject == node->objectInfo.object)
		nodeflags |= ImGuiTreeNodeFlags_Selected;

	if (node->children.size() != 0) {
		auto isOpen = ImGui::TreeNodeEx(node, nodeflags, "%s", node->GetLabel());

		if (node->type == SetObjectListTreeNode::Type::OBJECT && ImGui::IsItemClicked())
			levelEditor.focusedObject = node->objectInfo.object;

		if (isOpen) {
			for (SetObjectListTreeNode* child : node->children) {
				RenderObjectTreeNode(child);
			}
			ImGui::TreePop();
		}
	}
	else {
		ImGui::TreeNodeEx(node, nodeflags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen, "%s", node->GetLabel());

		if (node->type == SetObjectListTreeNode::Type::OBJECT && ImGui::IsItemClicked())
			levelEditor.focusedObject = node->objectInfo.object;
	}
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
					for (SetObjectListTreeNode* child : tree->children) {
						RenderObjectTreeNode(child);
					}
				}
				ImGui::EndChild();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Layer view")) {
				if (ImGui::BeginChild("Content")) {
					for (auto* layer : levelEditor.focusedChunk->GetLayers()) {
						ImGui::PushID(layer);
						if (ImGui::TreeNode(layer, "%s", layer->GetName())) {
							for (SetObjectListTreeNode* child : treesByLayer.GetValueOrFallback(layer, nullptr)->children) {
								RenderObjectTreeNode(child);
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
						ImGui::PopID();
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

SetObjectListTreeNode* SetObjectList::BuildTreeNode(ObjectData* objData) {
	auto objId = objData == nullptr ? ObjectId{ 0, 0 } : objData->id;
	auto* node = SetObjectListTreeNode::CreateObjectNode(GetAllocator(), objData);

	for (auto* layer : levelEditor.focusedChunk->GetLayers())
		for (auto* object : layer->GetResource()->GetObjects())
			if (object->parentID == objId)
				node->children.push_back(BuildTreeNode(object));

	return node;
}

SetObjectListTreeNode* SetObjectList::BuildSingleLayerTreeNode(hh::game::ObjectWorldChunkLayer* layer, ObjectData* objData) {
	auto objId = objData == nullptr ? ObjectId{ 0, 0 } : objData->id;
	auto* node = SetObjectListTreeNode::CreateObjectNode(GetAllocator(), objData);

	for (auto* object : layer->GetResource()->GetObjects())
		if (object->parentID == objId)
			node->children.push_back(BuildSingleLayerTreeNode(layer, object));

	return node;
}

void SetObjectList::RebuildTree() {
	if (levelEditor.focusedChunk == nullptr) {
		tree = nullptr;
		treesByLayer.clear();
	}
	else {
		tree = BuildTreeNode(nullptr);

		for (auto* layer : levelEditor.focusedChunk->GetLayers())
			treesByLayer.Insert(layer, BuildSingleLayerTreeNode(layer, nullptr));
	}
	std::vector<int> v;
}
