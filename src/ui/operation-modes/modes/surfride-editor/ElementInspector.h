#pragma once
#include <ui/operation-modes/Panel.h>
#include "Context.h"

namespace ui::operation_modes::modes::surfride_editor {
    class ElementInspector : public Panel<Context> {
    public:
        using Panel::Panel;

        virtual void RenderPanel() override;
        virtual PanelTraits GetPanelTraits() const override;
        void RenderSceneInspector(SurfRide::Scene& scene);
        void RenderCameraDataInspector(ucsl::resources::swif::v6::SRS_CAMERA& cameraData);
        void RenderLayerInspector(SurfRide::Layer& layer);
        void RenderCastInspector(SurfRide::Cast& cast);
        void RenderNormalCastInspector(SurfRide::Cast& cast);
        void RenderBaseCastInspector(SurfRide::Cast& cast);
        void RenderImageCastInspector(SurfRide::ImageCast& cast);
        void RenderReferenceCastInspector(SurfRide::ReferenceCast& cast);
        void RenderSliceCastInspector(SurfRide::SliceCast& cast);
    };
}
