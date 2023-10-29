#pragma once

class ObjectInspection;
class ObjectInspector : public hh::fnd::BaseObject {
    ObjectInspection& objectInspection;
    void RenderComponentInspector(hh::game::GOComponent& component);
	void RenderGOCTransformInspector(hh::game::GOCTransform& component);
	void RenderGOCPlayerParameterInspector(app::player::GOCPlayerParameter& component);
	void RenderUnknownComponentInspector(hh::game::GOComponent& component);

public:
    ObjectInspector(csl::fnd::IAllocator* allocator, ObjectInspection& objectInspection);
    void Render();
};
