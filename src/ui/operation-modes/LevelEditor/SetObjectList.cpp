#include "SetObjectList.h"
#include "LevelEditor.h"
#include <ui/common/Icons.h>
#include <ui/resources/ResourceBrowser.h>
#include <utilities/ObjectDataUtils.h>

using namespace hh::fnd;
using namespace hh::game;

SetObjectListTreeViewNode::SetObjectListTreeViewNode(SetObjectList& list, ObjectData* objData) : list{ list }, type { SetObjectListTreeViewNode::Type::OBJECT }, object{ objData } {}
SetObjectListTreeViewNode::SetObjectListTreeViewNode(SetObjectList& list, ObjectWorldChunkLayer* layer) : list{ list }, type{ SetObjectListTreeViewNode::Type::LAYER }, layer{ layer } {}
SetObjectListTreeViewNode::SetObjectListTreeViewNode(SetObjectList& list, const char* label) : list{ list }, type{ SetObjectListTreeViewNode::Type::GROUP }, group{ label } {}
SetObjectListTreeViewNode::SetObjectListTreeViewNode(SetObjectListTreeViewNode&& other) : list{ other.list }, type { other.type } {
	switch (type) {
	case SetObjectListTreeViewNode::Type::OBJECT: object = other.object;
	case SetObjectListTreeViewNode::Type::LAYER: layer = other.layer;
	case SetObjectListTreeViewNode::Type::GROUP: group = other.group;
	}
}

const void* SetObjectListTreeViewNode::GetID() const {
	switch (type) {
	case SetObjectListTreeViewNode::Type::OBJECT:
		return object.object;
	case SetObjectListTreeViewNode::Type::LAYER:
		return layer.layer;
	case SetObjectListTreeViewNode::Type::GROUP:
		return group.label;
	}
}

const char* SetObjectListTreeViewNode::GetLabel() const {
	switch (type) {
	case SetObjectListTreeViewNode::Type::OBJECT:
		return object.object->GetName();
	case SetObjectListTreeViewNode::Type::LAYER:
		return layer.layer->GetName();
	case SetObjectListTreeViewNode::Type::GROUP:
		return group.label;
	}
}

bool SetObjectListTreeViewNode::MatchesSearchString(const char* searchString) const
{
	return strstr(GetLabel(), searchString);
}

bool SetObjectListTreeViewNode::Render(ImGuiTreeNodeFlags nodeflags) const
{
	if (type == SetObjectListTreeViewNode::Type::OBJECT && list.levelEditor.focusedObjects.find(object.object) != -1)
		nodeflags |= ImGuiTreeNodeFlags_Selected;

	bool isOpen = ImGui::TreeNodeEx(GetID(), nodeflags, "%s", GetLabel());

	if (type == SetObjectListTreeViewNode::Type::OBJECT) {
		if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) && !ImGui::IsMouseDragging(ImGuiMouseButton_Left) && ImGui::IsItemHovered())
			list.levelEditor.Select(object.object);
		if (ImGui::BeginDragDropSource()) {
			ObjectData* obj = object.object;
			ImGui::SetDragDropPayload("ObjectData", &obj, sizeof(obj));
			ImGui::EndDragDropSource();
		}
		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ObjectData")) {
				ObjectData* parent = object.object;
				ObjectData* child = *static_cast<ObjectData**>(payload->Data);

				if (parent != child) {
					auto parentAbsoluteTransform = ObjectTransformDataToAffine3f(parent->transform);
					auto childAbsoluteTransform = ObjectTransformDataToAffine3f(child->transform);

					child->localTransform = Affine3fToObjectTransformData(parentAbsoluteTransform.inverse() * childAbsoluteTransform);
					child->parentID = parent->id;

					auto childStatus = list.levelEditor.focusedChunk->GetWorldObjectStatusByObjectId(child->id);
					auto childIdx = list.levelEditor.focusedChunk->GetObjectIndexById(child->id);

					if (childStatus.objectData && childIdx != -1) {
						list.levelEditor.focusedChunk->DespawnByIndex(childIdx);
						childStatus.Restart();
					}

					list.InvalidateTree();
				}
			}
			ImGui::EndDragDropTarget();
		}
	}

	if (type == SetObjectListTreeViewNode::Type::LAYER) {
		ImGui::PushID(GetID());
		if (ImGui::BeginPopupContextItem("Layer context menu")) {
			if (ImGui::MenuItem("Load from file..."))
				ResourceBrowser::ShowLoadResourceDialog(layer.layer->GetResource());

			if (ImGui::MenuItem("Export..."))
				ResourceBrowser::ShowExportResourceDialog(layer.layer->GetResource());

			ImGui::EndPopup();
		}
		ImGui::PopID();
	}

	return isOpen;
}

SetObjectList::SetObjectList(csl::fnd::IAllocator* allocator, LevelEditor& levelEditor) : CompatibleObject{ allocator }, levelEditor{ levelEditor }
{
}

void SetObjectList::Render() {
	auto* objWorld = GameManager::GetInstance()->GetService<ObjectWorld>();

	const ImGuiWindowFlags windowFlags
		= 0;

	ImGui::SetNextWindowSize(ImVec2(250, ImGui::GetMainViewport()->WorkSize.y - 100), ImGuiCond_FirstUseEver);
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
				snprintf(chunkid, sizeof(chunkid), "Chunk %d", i++);
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
				snprintf(chunkid, sizeof(chunkid), "Chunk %d", i++);

				if (ImGui::Selectable(chunkid, is_selected))
					levelEditor.SetFocusedChunk(chunk);

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		if (dirty)
			RebuildTree();

		if (levelEditor.focusedChunk && &*tree && ImGui::BeginTabBar("Object list views")) {
			if (ImGui::BeginTabItem("Tree view")) {
				tree->Render("Content");
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Layer view")) {
				layerTree->Render("Content");
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
	}
	ImGui::End();
}

size_t count = 0;

TreeViewNode<SetObjectListTreeViewNode> SetObjectList::BuildTreeNode(csl::ut::PointerMap<ObjectWorldChunkLayer*, ObjectMap<csl::ut::MoveArray<ObjectData*>>>& childMaps, ObjectData* objData) {
	auto objId = objData == nullptr ? ObjectId{} : objData->id;
	TreeViewNode<SetObjectListTreeViewNode> node{ GetAllocator(), { *this, objData } };

	for (auto* layer : levelEditor.focusedChunk->GetLayers()) {
		auto childLayerMapIt = childMaps.Find(layer);
		auto childMapIt = childLayerMapIt->Find(objId);

		if (childMapIt == childLayerMapIt->end())
			continue;

		for (auto* object : *childMapIt)
			node.AddChild(BuildTreeNode(childMaps, object));
	}

	return node;
}

TreeViewNode<SetObjectListTreeViewNode> SetObjectList::BuildSingleLayerTreeNode(ObjectMap<csl::ut::MoveArray<ObjectData*>>& childMap, ObjectWorldChunkLayer* layer, ObjectData* objData) {
	TreeViewNode<SetObjectListTreeViewNode> node{ GetAllocator(), { *this, objData } };

	auto childMapIt = childMap.Find(objData->id);

	if (childMapIt == childMap.end())
		return node;

	for (auto* object : *childMapIt)
		node.AddChild(BuildSingleLayerTreeNode(childMap, layer, object));

	return node;
}

TreeViewNode<SetObjectListTreeViewNode> SetObjectList::BuildSingleLayerRootNode(ObjectMap<csl::ut::MoveArray<ObjectData*>>& childMap, ObjectWorldChunkLayer* layer) {
	TreeViewNode<SetObjectListTreeViewNode> node{ GetAllocator(), { *this, layer } };

	auto childMapIt = childMap.Find({});

	if (childMapIt == childMap.end())
		return node;

	for (auto* object : *childMapIt)
		node.AddChild(BuildSingleLayerTreeNode(childMap, layer, object));

	return node;
}

void SetObjectList::RebuildTree() {
	dirty = false;
	tree = nullptr;
	layerTree = nullptr;

	if (levelEditor.focusedChunk != nullptr) {
		csl::ut::PointerMap<ObjectWorldChunkLayer*, ObjectMap<csl::ut::MoveArray<ObjectData*>>> childMaps{ GetAllocator() };

		for (auto* layer : levelEditor.focusedChunk->GetLayers()) {
			auto& childMap = *childMaps.InsertAndGet(layer, { GetAllocator() });

			for (auto* object : layer->GetResource()->GetObjects()) {
				auto parentIt = childMap.Find(object->parentID);

				if (parentIt == childMap.end())
					parentIt = childMap.InsertAndGet(object->parentID, { GetAllocator() });

				parentIt->push_back(object);
			}
		}

		tree = new (GetAllocator()) TreeView<SetObjectListTreeViewNode>(GetAllocator(), BuildTreeNode(childMaps, nullptr));
		layerTree = new (GetAllocator()) TreeView<SetObjectListTreeViewNode>(GetAllocator(), { GetAllocator(), { *this, "ROOT" } });

		for (auto* layer : levelEditor.focusedChunk->GetLayers())
			layerTree->rootNode.AddChild(BuildSingleLayerRootNode(*childMaps.Find(layer), layer));
	}
}

void SetObjectList::InvalidateTree() {
	dirty = true;
}
