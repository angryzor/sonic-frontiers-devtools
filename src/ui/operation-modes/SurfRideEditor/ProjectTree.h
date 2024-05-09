#pragma once

class SurfRideEditor;
class ProjectTree : public hh::fnd::BaseObject {
    SurfRideEditor& editor;
public:
    ProjectTree(csl::fnd::IAllocator* allocator, SurfRideEditor& editor);
    void Render();
    void RenderLayer(SurfRide::Layer* layer);
    void RenderCast(SurfRide::Cast* cast);
};
