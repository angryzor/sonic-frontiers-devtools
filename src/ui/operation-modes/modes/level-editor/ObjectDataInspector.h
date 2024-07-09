#pragma once
#include <ui/operation-modes/Panel.h>
#include "Context.h"

namespace ui::operation_modes::modes::level_editor {
    class ObjectDataInspector : public Panel<Context> {
        Eigen::Vector3f distributeSpacing{ 0.0f, 0.0f, 0.0f };
        bool alignX{}, alignY{}, alignZ{};

    public:
        using Panel::Panel;

        virtual void RenderPanel() override;
        virtual PanelTraits GetPanelTraits() const override;
        void Distribute();
        void DistributeAlongBasis(const Eigen::Vector3f& basis, float spacing);
    };
}
