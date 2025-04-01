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

	template<hh::dv::DvNodeBase::NodeType type>
	bool RenderNodeInspector(char* node) {}

	template<hh::dv::DvNodeElement::ElementID type>
	bool RenderElementInspector(char* element) {}
}
