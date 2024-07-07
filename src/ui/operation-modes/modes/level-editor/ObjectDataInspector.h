#pragma once
#include <utilities/CompatibleObject.h>

class LevelEditor;
class ObjectDataInspector : public CompatibleObject {
    LevelEditor& levelEditor;
    Eigen::Vector3f distributeSpacing{ 0.0f, 0.0f, 0.0f };
    bool alignX, alignY, alignZ;

public:
    ObjectDataInspector(csl::fnd::IAllocator* allocator, LevelEditor& levelEditor);
    void Render();
    void Distribute();
    void DistributeAlongBasis(const Eigen::Vector3f& basis, float spacing);
};
