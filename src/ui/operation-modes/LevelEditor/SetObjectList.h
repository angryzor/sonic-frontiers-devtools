#pragma once

class SetObjectList;
struct SetObjectListTreeNode : public hh::fnd::ReferencedObject {
	enum class Type {
		OBJECT,
		LAYER,
		GROUP,
	};

	struct GroupNodeInfo {
		const char* label{};
	};

	struct LayerNodeInfo {
		hh::game::ObjectWorldChunkLayer* layer;
	};

	struct ObjectNodeInfo {
		hh::game::ObjectData* object{};
	};

	Type type;

	union {
		GroupNodeInfo group;
		LayerNodeInfo layer;
		ObjectNodeInfo object;
	} info;

	csl::ut::MoveArray<SetObjectListTreeNode> children;
	SetObjectListTreeNode* parent{};

	SetObjectListTreeNode(csl::fnd::IAllocator* allocator);
	SetObjectListTreeNode(SetObjectListTreeNode& other) = delete;
	SetObjectListTreeNode(SetObjectListTreeNode&& other);
	SetObjectListTreeNode& operator=(SetObjectListTreeNode& other) = delete;
	const void* GetID() const;
	const char* GetLabel() const;
	int GetTotalVisibleObjects() const;
	bool RenderTreeNode(ImGuiTreeNodeFlags nodeflags, SetObjectList& list) const;
	void RenderChildren(SetObjectList& list, int startIdx, int count);
	void Render(SetObjectList& list, int startIdx, int count);
	void AddChild(SetObjectListTreeNode&& child);
	static SetObjectListTreeNode CreateObjectNode(csl::fnd::IAllocator* allocator, hh::game::ObjectData* objData);
	static SetObjectListTreeNode CreateLayerNode(csl::fnd::IAllocator* allocator, hh::game::ObjectWorldChunkLayer* layer);
	static SetObjectListTreeNode CreateGroupNode(csl::fnd::IAllocator* allocator, const char* label);
};

class LevelEditor;
class SetObjectList : public hh::fnd::BaseObject {
	friend struct SetObjectListTreeNode;

	LevelEditor& levelEditor;
	hh::fnd::Reference<SetObjectListTreeNode> tree;
	hh::fnd::Reference<SetObjectListTreeNode> layerTree;
	bool dirty{ false };
	SetObjectListTreeNode BuildTreeNode(csl::ut::PointerMap<hh::game::ObjectWorldChunkLayer*, hh::game::ObjectMap<csl::ut::MoveArray<hh::game::ObjectData*>>>& childMaps, hh::game::ObjectData* objData);
	SetObjectListTreeNode BuildSingleLayerTreeNode(hh::game::ObjectMap<csl::ut::MoveArray<hh::game::ObjectData*>>& childMaps, hh::game::ObjectWorldChunkLayer* layer, hh::game::ObjectData* objData);
	SetObjectListTreeNode BuildSingleLayerRootNode(hh::game::ObjectMap<csl::ut::MoveArray<hh::game::ObjectData*>>& childMaps, hh::game::ObjectWorldChunkLayer* layer);
	void RebuildTree();
public:
	SetObjectList(csl::fnd::IAllocator* allocator, LevelEditor& levelEditor);
	void Render();
	void RenderObjectTreeNode(SetObjectListTreeNode& obj, int startIdx, int endIdx);
	void InvalidateTree();
};
