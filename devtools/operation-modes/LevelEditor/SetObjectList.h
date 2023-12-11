#pragma once

struct SetObjectListTreeNode : public hh::fnd::ReferencedObject {
	enum class Type {
		OBJECT,
		GROUP,
	};

	struct GroupNodeInfo {
		const char* label{};
	};

	struct ObjectNodeInfo {
		hh::game::ObjectData* object{};
	};

	Type type;

	union {
		GroupNodeInfo group;
		ObjectNodeInfo object;
	} info;

	csl::ut::MoveArray<SetObjectListTreeNode> children;
	bool isOpen{ false };
	int totalChildrenCount{ 0 };
	SetObjectListTreeNode* parent{};

	SetObjectListTreeNode(csl::fnd::IAllocator* allocator);
	SetObjectListTreeNode(SetObjectListTreeNode& other) = delete;
	SetObjectListTreeNode(SetObjectListTreeNode&& other);
	SetObjectListTreeNode& operator=(SetObjectListTreeNode& other) = delete;
	const char* GetLabel() const;
	int GetTotalVisibleObjects() const;
	bool RenderTreeNode(ImGuiTreeNodeFlags nodeflags) const;
	void SetIsOpen(bool value);
	void AddChild(SetObjectListTreeNode&& child);
	void UpdateTotalChildrenCount(int difference);
	static SetObjectListTreeNode CreateObjectNode(csl::fnd::IAllocator* allocator, hh::game::ObjectData* objData);
	static SetObjectListTreeNode CreateGroupNode(csl::fnd::IAllocator* allocator, const char* label);
};

class LevelEditor;
class SetObjectList : public hh::fnd::BaseObject {
	LevelEditor& levelEditor;
	hh::fnd::Reference<SetObjectListTreeNode> tree;
	hh::fnd::Reference<SetObjectListTreeNode> layerTree;
public:
	SetObjectList(csl::fnd::IAllocator* allocator, LevelEditor& levelEditor);
	void Render();
	void RenderObjectTreeNode(SetObjectListTreeNode& obj, int startIdx, int endIdx);
	SetObjectListTreeNode BuildTreeNode(csl::ut::PointerMap<hh::game::ObjectWorldChunkLayer*, hh::game::ObjectMap<csl::ut::MoveArray<hh::game::ObjectData*>>>& childMaps, hh::game::ObjectData* objData);
	SetObjectListTreeNode BuildSingleLayerTreeNode(hh::game::ObjectMap<csl::ut::MoveArray<hh::game::ObjectData*>>& childMaps, hh::game::ObjectWorldChunkLayer* layer, hh::game::ObjectData* objData);
	SetObjectListTreeNode BuildSingleLayerRootNode(hh::game::ObjectMap<csl::ut::MoveArray<hh::game::ObjectData*>>& childMaps, hh::game::ObjectWorldChunkLayer* layer);
	void RebuildTree();
};
