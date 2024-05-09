#pragma once

class SurfRideEditor;
class ElementInspector : public hh::fnd::BaseObject {
    SurfRideEditor& editor;
public:
    ElementInspector(csl::fnd::IAllocator* allocator, SurfRideEditor& editor);
    void Render();
    void RenderSceneInspector(SurfRide::Scene& scene);
    void RenderCameraDataInspector(SurfRide::SRS_CAMERA& cameraData);
    void RenderLayerInspector(SurfRide::Layer& layer);
    void RenderCastInspector(SurfRide::Cast& cast);
};
