#pragma once

class LevelEditor;
class SetObjectList : public hh::fnd::BaseObject {
    LevelEditor& levelEditor;
public:
    SetObjectList(csl::fnd::IAllocator* allocator, LevelEditor& levelEditor);
    void Render();
	void RenderObjectTreeNode(hh::game::ObjectData* obj);
};
