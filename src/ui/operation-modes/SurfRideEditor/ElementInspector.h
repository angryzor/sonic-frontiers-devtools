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
    void RenderNormalCastInspector(SurfRide::Cast& cast);
    void RenderBaseCastInspector(SurfRide::Cast& cast);
    void RenderImageCastInspector(SurfRide::ImageCast& cast);
    void RenderReferenceCastInspector(SurfRide::ReferenceCast& cast);
    void RenderSliceCastInspector(SurfRide::SliceCast& cast);
};
