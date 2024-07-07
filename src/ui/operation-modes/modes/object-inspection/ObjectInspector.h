#pragma once
#include <ui/common/editors/Reflection.h>
#include <utilities/CompatibleObject.h>

class ObjectInspection;
class ObjectInspector : public CompatibleObject {
    ObjectInspection& objectInspection;

public:
    ObjectInspector(csl::fnd::IAllocator* allocator, ObjectInspection& objectInspection);
    void Render();
};
