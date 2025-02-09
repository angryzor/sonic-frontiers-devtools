#pragma once
#include <ui/operation-modes/Panel.h>
#include "Context.h"

namespace ui::operation_modes::modes::svcol_editor {
    class ShapeInspector : public Panel<Context> {
    public:
        using Panel::Panel;

        virtual void RenderPanel() override;
        virtual PanelTraits GetPanelTraits() const override;
    };
}
