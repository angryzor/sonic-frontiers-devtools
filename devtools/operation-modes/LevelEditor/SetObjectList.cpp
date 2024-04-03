#include "../../Pch.h"
#include "SetObjectList.h"
#include "LevelEditor.h"
#include "../../common/Icons.h"
#include "../../imgui/ImGuiFileDialog.h"
#include "../../ResourceBrowser.h"
#include "../../ObjectDataUtils.h"

using namespace hh::fnd;
using namespace hh::game;

SetObjectListTreeNode::SetObjectListTreeNode(csl::fnd::IAllocator* allocator) : ReferencedObject{ allocator, true }, children{ allocator }, info{ nullptr } {
}

SetObjectListTreeNode::SetObjectListTreeNode(SetObjectListTreeNode&& other) : ReferencedObject{ other.GetAllocator(), true }, type{ other.type }, info{ other.info }, children{ std::move(other.children) }, parent{ other.parent } {
	for (auto& child : children)
		child.parent = this;
}

const void* SetObjectListTreeNode::GetID() const {
	if (type == SetObjectListTreeNode::Type::OBJECT)
		return info.object.object;
	else
		return info.group.label;
}

const char* SetObjectListTreeNode::GetLabel() const {
	if (type == SetObjectListTreeNode::Type::OBJECT)
		return info.object.object->name.c_str();
	else
		return info.group.label;
}

SetObjectListTreeNode SetObjectListTreeNode::CreateObjectNode(csl::fnd::IAllocator* allocator, ObjectData* objData) {
	SetObjectListTreeNode node{ allocator };
	node.type = SetObjectListTreeNode::Type::OBJECT;
	node.info.object.object = objData;
	return node;
}

SetObjectListTreeNode SetObjectListTreeNode::CreateGroupNode(csl::fnd::IAllocator* allocator, const char* label) {
	SetObjectListTreeNode node{ allocator };
	node.type = SetObjectListTreeNode::Type::GROUP;
	node.info.group.label = label;
	return node;
}

bool SetObjectListTreeNode::RenderTreeNode(ImGuiTreeNodeFlags nodeflags, SetObjectList& list) const
{
	if (type == SetObjectListTreeNode::Type::OBJECT && list.levelEditor.focusedObject == info.object.object)
		nodeflags |= ImGuiTreeNodeFlags_Selected;

	bool isOpen = ImGui::TreeNodeEx(GetID(), nodeflags, "%s", GetLabel());

	if (type == SetObjectListTreeNode::Type::OBJECT) {
		if (ImGui::IsItemClicked())
			list.levelEditor.focusedObject = info.object.object;

		if (ImGui::BeginDragDropSource()) {
			ObjectData* obj = info.object.object;
			ImGui::SetDragDropPayload("ObjectData", &obj, sizeof(obj));
			ImGui::EndDragDropSource();
		}

		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ObjectData")) {
				ObjectData* parent = info.object.object;
				ObjectData* child = *static_cast<ObjectData**>(payload->Data);

				if (parent != child) {
					auto parentAbsoluteTransform = ObjectTransformDataToAffine3f(parent->transform);
					auto childAbsoluteTransform = ObjectTransformDataToAffine3f(child->transform);

					child->localTransform = Affine3fToObjectTransformData(parentAbsoluteTransform.inverse() * childAbsoluteTransform);
					child->parentID = parent->id;

					auto childStatus = list.levelEditor.focusedChunk->GetWorldObjectStatusByObjectId(list.levelEditor.focusedObject->id);
					auto childIdx = list.levelEditor.focusedChunk->GetObjectIndexById(list.levelEditor.focusedObject->id);

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

	return isOpen;
}

void SetObjectListTreeNode::RenderChildren(SetObjectList& list, int startIdx, int count) {
	for (SetObjectListTreeNode& child : children) {
		if (count <= 0)
			break;

		auto visibleChildCount = child.GetTotalVisibleObjects();

		if (startIdx < visibleChildCount) {
			child.Render(list, startIdx, count);

			count -= visibleChildCount - startIdx;
		}

		startIdx = std::max(0, startIdx - visibleChildCount);
	}
}

void SetObjectListTreeNode::Render(SetObjectList& list, int startIdx, int count) {
	ImGuiTreeNodeFlags nodeflags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

	if (children.size() != 0) {
		if (startIdx < 1)
			count--;

		startIdx = std::max(0, startIdx - 1);

		// If the header would have fallen off the screen because we're scrolled down too far, we replace the
		// first real displayed element with the header. We then let it take an index to account for this.
		if (startIdx != 0) {
			count--;
			startIdx++;
		}

		if (RenderTreeNode(nodeflags, list)) {
			RenderChildren(list, startIdx, count);
			ImGui::TreePop();
		}
	}
	else
		RenderTreeNode(nodeflags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen, list);
}

int SetObjectListTreeNode::GetTotalVisibleObjects() const {
	int result = 1;

	if (parent == nullptr || ImGui::GetStateStorage()->GetInt(ImGui::GetID(GetID())))
		for (auto& child : children)
			result += child.GetTotalVisibleObjects();

	return result;
}

void SetObjectListTreeNode::AddChild(SetObjectListTreeNode&& child) {
	child.parent = this;
	children.push_back(std::move(child));
}

SetObjectList::SetObjectList(csl::fnd::IAllocator* allocator, LevelEditor& levelEditor) : BaseObject{ allocator }, levelEditor{ levelEditor }
{
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
				if (ImGui::BeginChild("Content")) {
					ImGuiListClipper clipper;
					clipper.Begin(tree->GetTotalVisibleObjects(), ImGui::GetTextLineHeight());

					while (clipper.Step())
						tree->RenderChildren(*this, clipper.DisplayStart, clipper.DisplayEnd - clipper.DisplayStart);

					clipper.End();
				}
				ImGui::EndChild();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Layer view")) {
				if (ImGui::BeginChild("Content")) {
					ImGuiListClipper clipper;
					clipper.Begin(layerTree->GetTotalVisibleObjects(), ImGui::GetTextLineHeight());

					while (clipper.Step())
						layerTree->RenderChildren(*this, clipper.DisplayStart, clipper.DisplayEnd - clipper.DisplayStart);

					clipper.End();
				}
				ImGui::EndChild();
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
	}
	ImGui::End();
}

size_t count = 0;

SetObjectListTreeNode SetObjectList::BuildTreeNode(csl::ut::PointerMap<ObjectWorldChunkLayer*, ObjectMap<csl::ut::MoveArray<ObjectData*>>>& childMaps, ObjectData* objData) {
	auto objId = objData == nullptr ? ObjectId{ 0, 0 } : objData->id;
	auto node = SetObjectListTreeNode::CreateObjectNode(GetAllocator(), objData);

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

SetObjectListTreeNode SetObjectList::BuildSingleLayerTreeNode(ObjectMap<csl::ut::MoveArray<ObjectData*>>& childMap, ObjectWorldChunkLayer* layer, ObjectData* objData) {
	auto node = SetObjectListTreeNode::CreateObjectNode(GetAllocator(), objData);

	auto childMapIt = childMap.Find(objData->id);

	if (childMapIt == childMap.end())
		return node;

	for (auto* object : *childMapIt)
		node.AddChild(BuildSingleLayerTreeNode(childMap, layer, object));

	return node;
}

SetObjectListTreeNode SetObjectList::BuildSingleLayerRootNode(ObjectMap<csl::ut::MoveArray<ObjectData*>>& childMap, ObjectWorldChunkLayer* layer) {
	auto node = SetObjectListTreeNode::CreateGroupNode(GetAllocator(), layer->GetName());

	auto childMapIt = childMap.Find(ObjectId{ 0, 0 });

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
			auto& childMap = *childMaps.Insert(layer, { GetAllocator() });

			for (auto* object : layer->GetResource()->GetObjects()) {
				auto parentIt = childMap.Find(object->parentID);

				if (parentIt == childMap.end())
					parentIt = childMap.Insert(object->parentID, { GetAllocator() });

				parentIt->push_back(object);
			}
		}

		tree = new (GetAllocator()) SetObjectListTreeNode(BuildTreeNode(childMaps, nullptr));
		layerTree = new (GetAllocator()) SetObjectListTreeNode(SetObjectListTreeNode::CreateGroupNode(GetAllocator(), "ROOT"));

		for (auto* layer : levelEditor.focusedChunk->GetLayers())
			layerTree->AddChild(BuildSingleLayerRootNode(*childMaps.Find(layer), layer));
	}
}

void SetObjectList::InvalidateTree() {
	dirty = true;
}
