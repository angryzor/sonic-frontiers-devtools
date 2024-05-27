#pragma once
#include <utilities/CompatibleObject.h>

class ObjectInspection;
class ObjectList : public CompatibleObject {
    ObjectInspection& objectInspection;
public:
    ObjectList(csl::fnd::IAllocator* allocator, ObjectInspection& objectInspection);
    void Render();
	void RenderObjectTreeNode(hh::game::GameObject* obj);
};
