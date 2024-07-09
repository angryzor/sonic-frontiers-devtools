#pragma once
#include <ui/operation-modes/Panel.h>
#include "Context.h"

namespace ui::operation_modes::modes::surfride_editor {
	class ProjectTree : public Panel<Context> {
	public:
		using Panel::Panel;

		virtual void RenderPanel() override;
		virtual PanelTraits GetPanelTraits() const override;
		void RenderLayer(SurfRide::Layer* layer);
		void RenderCast(SurfRide::Cast* cast);
	};
}
