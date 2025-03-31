#pragma once
#include <ui/operation-modes/Panel.h>
#include "Context.h"

namespace ui::operation_modes::modes::pcmodel_editor {
	class InstanceList : public Panel<Context> {
	public:
		using Panel::Panel;

		virtual void RenderPanel() override;
		virtual PanelTraits GetPanelTraits() const override;
		void RenderInstance(ucsl::resources::pointcloud::v2::InstanceData& shape);
	};
}
