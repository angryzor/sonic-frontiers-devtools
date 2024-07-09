#pragma once
#include <ui/common/TreeView.h>
#include <ui/operation-modes/Panel.h>
#include "Context.h"

namespace ui::operation_modes::modes::level_editor {
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

	class SetObjectList : public Panel<Context> {
		friend struct SetObjectListTreeViewNode;

		hh::fnd::Reference<TreeView<SetObjectListTreeViewNode>> tree;
		hh::fnd::Reference<TreeView<SetObjectListTreeViewNode>> layerTree;
		bool dirty{ false };
		TreeViewNode<SetObjectListTreeViewNode> BuildTreeNode(csl::ut::PointerMap<hh::game::ObjectWorldChunkLayer*, hh::game::ObjectMap<csl::ut::MoveArray<hh::game::ObjectData*>>>& childMaps, hh::game::ObjectData* objData);
		TreeViewNode<SetObjectListTreeViewNode> BuildSingleLayerTreeNode(hh::game::ObjectMap<csl::ut::MoveArray<hh::game::ObjectData*>>& childMaps, hh::game::ObjectWorldChunkLayer* layer, hh::game::ObjectData* objData);
		TreeViewNode<SetObjectListTreeViewNode> BuildSingleLayerRootNode(hh::game::ObjectMap<csl::ut::MoveArray<hh::game::ObjectData*>>& childMaps, hh::game::ObjectWorldChunkLayer* layer);
		void RebuildTree();
	public:
		using Panel::Panel;

		virtual void ProcessAction(const ActionBase& action) override;
		virtual void RenderPanel() override;
		virtual PanelTraits GetPanelTraits() const override;
		void InvalidateTree();
	};
}
