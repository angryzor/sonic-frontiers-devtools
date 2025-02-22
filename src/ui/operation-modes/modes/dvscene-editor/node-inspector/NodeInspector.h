#pragma once
#include <ui/operation-modes/Panel.h>
#include "../Context.h"

namespace ui::operation_modes::modes::dvscene_editor {
	class NodeInspector : public Panel<Context> {
	public:
		using Panel::Panel;

		virtual void RenderPanel() override;
		virtual PanelTraits GetPanelTraits() const override;
	};

	template<int type>
	void RenderNodeInspector(hh::dv::DvNodeBase* node) {}

	template<int type>
	void RenderElementInspector(hh::dv::DvElementBase* element) {}
}
