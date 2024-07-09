#pragma once
#include <ui/common/editors/Reflection.h>
#include <ui/operation-modes/Panel.h>
#include "Context.h"

namespace ui::operation_modes::modes::object_inspection {
    class ObjectInspector : public Panel<Context> {
    public:
        using Panel::Panel;

        virtual void RenderPanel() override;
        virtual PanelTraits GetPanelTraits() const override;
    };
}
