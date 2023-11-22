#pragma once

class LevelEditor;
class ObjectLibrary : public hh::fnd::BaseObject {
    LevelEditor& levelEditor;
    const hh::game::GameObjectClass* selectedClass;
public:
    ObjectLibrary(csl::fnd::IAllocator* allocator, LevelEditor& levelEditor);
    void Render();
	void RenderObjectTreeNode(hh::game::ObjectData* obj);
};
