#pragma once

class LevelEditor;
class ObjectDataInspector : public hh::fnd::BaseObject {
    LevelEditor& levelEditor;
    void RenderComponentInspector(hh::game::GOComponent& component);
	void RenderGOCTransformInspector(hh::game::GOCTransform& component);
	void RenderGOCPlayerParameterInspector(app::player::GOCPlayerParameter& component);
    void RenderGOCInputInspector(hh::game::GOComponent& component);
	void RenderUnknownComponentInspector(hh::game::GOComponent& component);
    void RenderTransformViewer(const char* name, const csl::math::Transform& transform);

public:
    ObjectDataInspector(csl::fnd::IAllocator* allocator, LevelEditor& levelEditor);
    void Render();
};
