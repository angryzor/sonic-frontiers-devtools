#pragma once
#include <utilities/CompatibleObject.h>

class LevelEditor;
class ObjectDataInspector : public CompatibleObject {
    LevelEditor& levelEditor;

public:
    ObjectDataInspector(csl::fnd::IAllocator* allocator, LevelEditor& levelEditor);
    void Render();
};
