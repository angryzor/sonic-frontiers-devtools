#pragma once

class ObjectInspection;
class ObjectList : public hh::fnd::BaseObject {
    ObjectInspection& objectInspection;
public:
    ObjectList(csl::fnd::IAllocator* allocator, ObjectInspection& objectInspection);
    void Render();
	void RenderObjectTreeNode(hh::game::GameObject* obj);
};
