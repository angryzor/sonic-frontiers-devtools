#include "../Behaviors.h"
#include "NodeInspector.h"
#include "../DvNode.h"
#include "Nodes.h"
#include <ui/common/StandaloneOperationModeHost.h>
#include <ui/common/viewers/Basic.h>
#include <ui/common/editors/Basic.h>
#include <ui/common/inputs/Basic.h>
#include <ui/GlobalSettings.h>

namespace ui::operation_modes::modes::dvscene_editor {
#ifdef DEVTOOLS_TARGET_SDK_rangers
	const char* nodeTypeNames0[] = {
		"",
		"Path",
		"PathMotion",
		"Camera",
		"CameraMotion",
		"Character",
		"CharacterMotion",
		"CharacterBehavior",
		"Model",
		"",
		"ModelMotion",
		"ModelNode",
		"Element"
	};
#endif

	void NodeInspector::RenderPanel()
	{
		auto& selection = GetBehavior<SelectionBehavior<Context>>()->GetSelection();

		if (selection.size() == 0)
			ImGui::Text("Select an item in the Node Tree.");
		else if (selection.size() > 1)
			ImGui::Text("Multiple items selected.");
		else {
			auto& focusedNode = selection[0];
			hh::dv::DvNodeBase* node = focusedNode.node;
			Viewer("ID", node->guid);
			Editor("Node Name", node->nodeName);
			int type = static_cast<int>(node->nodeType);
			Viewer("Node Type", nodeTypeNames0[type]);
			NodeFuncType render = GetNodeInspectorRender(type);
			if(render){
				ImGui::SeparatorText("Properties");
				render(node);
			}
		}
	}

	PanelTraits NodeInspector::GetPanelTraits() const
	{
		return { "Node Inspector", ImVec2(ImGui::GetMainViewport()->WorkSize.x-250, 0), ImVec2(250, ImGui::GetMainViewport()->WorkSize.y - 200) };
	}
}
