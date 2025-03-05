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
	void NodeInspector::RenderPanel()
	{
		auto& context = GetContext();

		if (context.goDVSC == nullptr) {
			ImGui::Text("No DvSceneControl selected");
			return;
		}

		auto& selection = GetBehavior<SelectionBehavior<Context>>()->GetSelection();

		if (selection.size() == 0)
			ImGui::Text("Select an item in the Node Tree.");
		else if (selection.size() > 1)
			ImGui::Text("Multiple items selected.");
		else {
			auto& focusedNode = selection[0];
			hh::dv::DvNodeBase* node = focusedNode.node;
			char guidBuffer[37];
			unsigned char guid[16];
			memcpy(guid, node->guid, 16);
			sprintf(guidBuffer,
				"%02X%02X%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X%02X%02X%02X%02X",
				guid[0], guid[1], guid[2], guid[3],
				guid[4], guid[5],
				guid[6], guid[7],
				guid[8], guid[9],
				guid[10], guid[11], guid[12], guid[13], guid[14], guid[15]);
			ImGui::Text("GUID: {%s}", guidBuffer);
			Editor("Node Name", node->nodeName);
			int type = static_cast<int>(node->nodeType);
			Viewer("Node Type", nodeTypeNames[type]);
#ifdef DEVTOOLS_TARGET_SDK_rangers
			if (node->nodeType == hh::dv::DvNodeBase::NodeType::CHARACTER ||
				node->nodeType == hh::dv::DvNodeBase::NodeType::CAMERA_MOTION || 
				node->nodeType == hh::dv::DvNodeBase::NodeType::CHARACTER_MOTION) {
				const char* curRes = "NULL";
				if (auto* dvres = node->dvResource)
					if (auto* res = dvres->resource)
						curRes = res->GetName();
				ImGui::Text("Current Resource: %s (Drag and drop a resource to change)", curRes);
				if (ImGui::BeginDragDropTarget()) {
					if (auto* payload = ImGui::AcceptDragDropPayload("Resource")) {
						auto* resource = *static_cast<hh::fnd::ManagedResource**>(payload->Data);
						const hh::fnd::ResourceTypeInfo* resInfo;
						switch (node->nodeType) {
						case hh::dv::DvNodeBase::NodeType::CHARACTER:
							resInfo = hh::anim::ResAnimator::GetTypeInfo();
							break;
						case hh::dv::DvNodeBase::NodeType::CAMERA_MOTION:
							resInfo = hh::gfx::ResAnimCameraContainer::GetTypeInfo();
							break;
						case hh::dv::DvNodeBase::NodeType::CHARACTER_MOTION:
							resInfo = hh::anim::ResAnimationPxd::GetTypeInfo();
							break;
						}
						if (resource->resourceTypeInfo == resInfo)
							if (auto* res = node->dvResource)
								res->resource = resource;
					}
					ImGui::EndDragDropTarget();
				}
			}
#endif
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
