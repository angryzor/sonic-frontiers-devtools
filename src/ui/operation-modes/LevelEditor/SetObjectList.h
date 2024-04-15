#pragma once

#include <ui/common/TreeView.h>

class SetObjectList;
struct SetObjectListTreeViewNode {
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
	};
	SetObjectList& list;

	const void* GetID() const;
	const char* GetLabel() const;
	bool MatchesSearchString(const char* searchString) const;
	bool Render(ImGuiTreeNodeFlags nodeflags) const;
	SetObjectListTreeViewNode(SetObjectList& list, hh::game::ObjectData* objData);
	SetObjectListTreeViewNode(SetObjectList& list, hh::game::ObjectWorldChunkLayer* layer);
	SetObjectListTreeViewNode(SetObjectList& list, const char* label);
	SetObjectListTreeViewNode(SetObjectListTreeViewNode&& other);
};

class LevelEditor;
class SetObjectList : public hh::fnd::BaseObject {
	friend struct SetObjectListTreeViewNode;

	LevelEditor& levelEditor;
	hh::fnd::Reference<TreeView<SetObjectListTreeViewNode>> tree;
	hh::fnd::Reference<TreeView<SetObjectListTreeViewNode>> layerTree;
	bool dirty{ false };
	TreeViewNode<SetObjectListTreeViewNode> BuildTreeNode(csl::ut::PointerMap<hh::game::ObjectWorldChunkLayer*, hh::game::ObjectMap<csl::ut::MoveArray<hh::game::ObjectData*>>>& childMaps, hh::game::ObjectData* objData);
	TreeViewNode<SetObjectListTreeViewNode> BuildSingleLayerTreeNode(hh::game::ObjectMap<csl::ut::MoveArray<hh::game::ObjectData*>>& childMaps, hh::game::ObjectWorldChunkLayer* layer, hh::game::ObjectData* objData);
	TreeViewNode<SetObjectListTreeViewNode> BuildSingleLayerRootNode(hh::game::ObjectMap<csl::ut::MoveArray<hh::game::ObjectData*>>& childMaps, hh::game::ObjectWorldChunkLayer* layer);
	void RebuildTree();
public:
	SetObjectList(csl::fnd::IAllocator* allocator, LevelEditor& levelEditor);
	void Render();
	void InvalidateTree();
};
