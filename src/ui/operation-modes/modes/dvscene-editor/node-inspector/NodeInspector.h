#pragma once
#include <ui/operation-modes/Panel.h>
#include "../Context.h"

namespace ui::operation_modes::modes::dvscene_editor {
	class NodeInspector : public Panel<Context> {
	public:
		using Panel::Panel;

		virtual void RenderPanel() override;
		virtual PanelTraits GetPanelTraits() const override;

		bool NodeEditor(dv::DvNode* node);
	};

	template<int type>
	bool RenderNodeInspector(char* node) {}

	template<int type>
	bool RenderElementInspector(char* element) {}
}
