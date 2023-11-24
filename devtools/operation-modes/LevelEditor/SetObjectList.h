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
        GroupNodeInfo groupInfo;
        ObjectNodeInfo objectInfo;
    };

    csl::ut::MoveArray<hh::fnd::Reference<SetObjectListTreeNode>> children{ GetAllocator() };

    SetObjectListTreeNode(csl::fnd::IAllocator* allocator);
    const char* GetLabel() const;
    static SetObjectListTreeNode* CreateObjectNode(csl::fnd::IAllocator* allocator, hh::game::ObjectData* objData);
    static SetObjectListTreeNode* CreateGroupNode(csl::fnd::IAllocator* allocator, const char* label);
};

class LevelEditor;
class SetObjectList : public hh::fnd::BaseObject {
    LevelEditor& levelEditor;
    hh::fnd::Reference<SetObjectListTreeNode> tree{};
    csl::ut::PointerMap<hh::game::ObjectWorldChunkLayer*, hh::fnd::Reference<SetObjectListTreeNode>> treesByLayer{ GetAllocator() };
public:
    SetObjectList(csl::fnd::IAllocator* allocator, LevelEditor& levelEditor);
    void Render();
	void RenderObjectTreeNode(SetObjectListTreeNode* obj);
    SetObjectListTreeNode* BuildTreeNode(hh::game::ObjectData* objData);
    SetObjectListTreeNode* BuildSingleLayerTreeNode(hh::game::ObjectWorldChunkLayer* layer, hh::game::ObjectData* objData);
    void RebuildTree();
};
