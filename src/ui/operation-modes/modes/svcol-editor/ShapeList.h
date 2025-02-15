#pragma once
#include <ui/operation-modes/Panel.h>
#include "Context.h"

namespace ui::operation_modes::modes::svcol_editor {
	class ShapeList : public Panel<Context> {
	public:
		using Panel::Panel;

		virtual void RenderPanel() override;
		virtual PanelTraits GetPanelTraits() const override;
		void RenderShapeItem(ucsl::resources::svcol::v1::ShapeData& shape);
	};
}
