#include "../../Pch.h"
#include "SetObjectList.h"
#include "LevelEditor.h"
#include "../../common/Icons.h"
#include "../../imgui/ImGuiFileDialog.h"
#include "../../ResourceBrowser.h"

using namespace hh::fnd;
using namespace hh::game;

SetObjectListTreeNode::SetObjectListTreeNode(csl::fnd::IAllocator* allocator) : ReferencedObject{ allocator, true }, children{ allocator }, info{ nullptr } {
}

SetObjectListTreeNode::SetObjectListTreeNode(SetObjectListTreeNode&& other) : ReferencedObject{ other.GetAllocator(), true }, type{ other.type }, info{ other.info }, children{ std::move(other.children) }, totalChildrenCount{ other.totalChildrenCount }, parent{ other.parent } {
	for (auto& child : children)
		child.parent = this;
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

void SetObjectListTreeNode::SetIsOpen(bool value) {
	if (isOpen == value)
		return;

	isOpen = value;

	if (parent)
		parent->UpdateTotalChildrenCount(value ? totalChildrenCount : -totalChildrenCount);
}

int SetObjectListTreeNode::GetTotalVisibleObjects() const {
	return isOpen ? 1 + totalChildrenCount : 1;
}

void SetObjectListTreeNode::AddChild(SetObjectListTreeNode&& child) {
	child.parent = this;
	UpdateTotalChildrenCount(child.GetTotalVisibleObjects());
	children.push_back(std::move(child));
}

void SetObjectListTreeNode::UpdateTotalChildrenCount(int difference) {
	totalChildrenCount += difference;

	if (parent)
		parent->UpdateTotalChildrenCount(difference);
}

SetObjectList::SetObjectList(csl::fnd::IAllocator* allocator, LevelEditor& levelEditor) : BaseObject{ allocator }, levelEditor{ levelEditor }
{
}

void SetObjectList::RenderObjectTreeNode(SetObjectListTreeNode& node, int startIdx, int count) {
	ImGuiTreeNodeFlags nodeflags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

	if (node.type == SetObjectListTreeNode::Type::OBJECT && levelEditor.focusedObject == node.info.object.object)
		nodeflags |= ImGuiTreeNodeFlags_Selected;

	if (node.children.size() != 0) {
		if (startIdx < 1)
			count--;

		startIdx = std::max(0, startIdx - 1);

		// If the header would have fallen off the screen because we're scrolled down too far, we replace the
		// first real displayed element with the header. We then let it take an index to account for this.
		if (startIdx != 0) {
			count--;
			startIdx++;
		}

		ImGui::SetNextItemOpen(node.isOpen);
		auto isOpen = ImGui::TreeNodeEx(&node, nodeflags, "%s", node.GetLabel());
		node.SetIsOpen(isOpen);

		if (node.type == SetObjectListTreeNode::Type::OBJECT && ImGui::IsItemClicked())
			levelEditor.focusedObject = node.info.object.object;

		if (isOpen) {
			for (SetObjectListTreeNode& child : node.children) {
				if (count <= 0)
					break;

				if (startIdx < child.GetTotalVisibleObjects()) {
					RenderObjectTreeNode(child, startIdx, count);

					count -= child.GetTotalVisibleObjects() - startIdx;
				}

				startIdx = std::max(0, startIdx - child.GetTotalVisibleObjects());
			}
			ImGui::TreePop();
		}
	}
	else {
		ImGui::TreeNodeEx(&node, nodeflags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen, "%s", node.GetLabel());

		if (node.type == SetObjectListTreeNode::Type::OBJECT && ImGui::IsItemClicked())
			levelEditor.focusedObject = node.info.object.object;
	}
	if (node.type == SetObjectListTreeNode::Type::OBJECT && ImGui::BeginDragDropSource()) {
		ObjectData* obj = node.info.object.object;
		ImGui::SetDragDropPayload("ObjectData", &obj, sizeof(obj));
		ImGui::EndDragDropSource();
	}
	if (node.type == SetObjectListTreeNode::Type::OBJECT && ImGui::BeginDragDropTarget()) {
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ObjectData")) {
			ObjectData* parent = node.info.object.object;
			ObjectData* child = *static_cast<ObjectData**>(payload->Data);

			if (parent != child) {
				Eigen::Transform<float, 3, Eigen::Affine> parentAbsoluteTransform{};
				Eigen::Transform<float, 3, Eigen::Affine> childAbsoluteTransform{};

				parentAbsoluteTransform.fromPositionOrientationScale(
					parent->transform.position,
					Eigen::AngleAxisf(parent->transform.rotation[1], Eigen::Vector3f::UnitY()) * Eigen::AngleAxisf(parent->transform.rotation[0], Eigen::Vector3f::UnitX()) * Eigen::AngleAxisf(parent->transform.rotation[2], Eigen::Vector3f::UnitZ()),
					csl::math::Vector3{ 1.0f, 1.0f, 1.0f }
				);

				childAbsoluteTransform.fromPositionOrientationScale(
					child->transform.position,
					Eigen::AngleAxisf(child->transform.rotation[1], Eigen::Vector3f::UnitY()) * Eigen::AngleAxisf(child->transform.rotation[0], Eigen::Vector3f::UnitX()) * Eigen::AngleAxisf(child->transform.rotation[2], Eigen::Vector3f::UnitZ()),
					csl::math::Vector3{ 1.0f, 1.0f, 1.0f }
				);

				Eigen::Transform<float, 3, Eigen::Affine> localTransform = parentAbsoluteTransform.inverse() * childAbsoluteTransform;

				Eigen::Matrix3f localRotation;
				Eigen::Matrix3f localScaling;

				localTransform.computeRotationScaling(&localRotation, &localScaling);

				child->localTransform.position = { localTransform.translation() };
				auto localEuler = localRotation.eulerAngles(1, 0, 2);
				child->localTransform.rotation = { localEuler[1], localEuler[0], localEuler[2] };
			}
		}
		ImGui::EndDragDropTarget();
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

		if (levelEditor.focusedChunk && &*tree && ImGui::BeginTabBar("Object list views")) {
			if (ImGui::BeginTabItem("Tree view")) {
				if (ImGui::BeginChild("Content")) {
					ImGuiListClipper clipper;
					clipper.Begin(tree->GetTotalVisibleObjects(), ImGui::GetTextLineHeight());

					while (clipper.Step()) {
						int startIdx = clipper.DisplayStart;
						int count = clipper.DisplayEnd - clipper.DisplayStart;

						for (SetObjectListTreeNode& child : tree->children) {
							if (count <= 0)
								break;

							if (startIdx < child.GetTotalVisibleObjects()) {
								RenderObjectTreeNode(child, startIdx, count);

								count -= child.GetTotalVisibleObjects() - startIdx;
							}

							startIdx = std::max(0, startIdx - child.GetTotalVisibleObjects());
						}
					}

					clipper.End();
				}
				ImGui::EndChild();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Layer view")) {
				if (ImGui::BeginChild("Content")) {
					ImGuiListClipper clipper;
					clipper.Begin(layerTree->GetTotalVisibleObjects(), ImGui::GetTextLineHeight());

					while (clipper.Step()) {
						int startIdx = clipper.DisplayStart;
						int count = clipper.DisplayEnd - clipper.DisplayStart;

						for (SetObjectListTreeNode& child : layerTree->children) {
							if (count <= 0)
								break;

							if (startIdx < child.GetTotalVisibleObjects()) {
								RenderObjectTreeNode(child, startIdx, count);

								count -= child.GetTotalVisibleObjects() - startIdx;
							}

							startIdx = std::max(0, startIdx - child.GetTotalVisibleObjects());
						}
					}

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

		tree->SetIsOpen(true);
		layerTree->SetIsOpen(true);
	}
}
