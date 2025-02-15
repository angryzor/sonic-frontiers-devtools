#pragma once
#include <ui/operation-modes/Panel.h>
#include "Context.h"

namespace ui::operation_modes::modes::surfride_editor {
    class ElementInspector : public Panel<Context> {
    public:
        using Panel::Panel;

        virtual void RenderPanel() override;
        virtual PanelTraits GetPanelTraits() const override;
        void RenderSceneInspector(ucsl::resources::swif::swif_version::SRS_SCENE& scene, SurfRide::Scene* runtimeScene);
        void RenderCameraInspector(ucsl::resources::swif::swif_version::SRS_CAMERA& cameraData);
        void RenderLayerInspector(ucsl::resources::swif::swif_version::SRS_LAYER& layer, SurfRide::Layer* runtimeLayer);
        void RenderCastInspector(ucsl::resources::swif::swif_version::SRS_CASTNODE& cast, SurfRide::Cast* runtimeCast);
        void RenderNormalCastInspector(ucsl::resources::swif::swif_version::SRS_CASTNODE& cast, SurfRide::Cast* runtimeCast);
        void RenderBaseCastInspector(ucsl::resources::swif::swif_version::SRS_CASTNODE& cast, SurfRide::Cast* runtimeCast);
        void RenderImageCastInspector(ucsl::resources::swif::swif_version::SRS_CASTNODE& cast, SurfRide::ImageCast* runtimeCast);
        void RenderReferenceCastInspector(ucsl::resources::swif::swif_version::SRS_CASTNODE& cast, SurfRide::ReferenceCast* runtimeCast);
        void RenderSliceCastInspector(ucsl::resources::swif::swif_version::SRS_CASTNODE& cast, SurfRide::SliceCast* runtimeCast);
    };
}
