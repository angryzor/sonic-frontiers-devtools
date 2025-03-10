#include "Tree.h"
#include "Behaviors.h"
#include <ui/common/StandaloneOperationModeHost.h>
#include <ui/common/viewers/Basic.h>
#include <ui/common/editors/Basic.h>
#include <ui/GlobalSettings.h>

namespace ui::operation_modes::modes::dvscene_editor {
    void Tree::RenderNode(hh::dv::DvNodeBase* node){
		ImGui::PushID(node);

		ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
		DvNode selection{ node };

        auto* selectionBehavior = GetBehavior<SelectionBehavior<Context>>();
		auto& selected = selectionBehavior->GetSelection();

		if (selected.find(selection) != -1)
			nodeFlags |= ImGuiTreeNodeFlags_Selected;

		char nodeName[256];
		const char* type = nodeTypeNames[static_cast<int>(node->nodeType)];
		if (node->nodeType == hh::dv::DvNodeBase::NodeType::ELEMENT) {
			unsigned int elemId = static_cast<unsigned int>(reinterpret_cast<hh::dv::DvNodeElement*>(node)->binaryData.elementId);
			if (elemId >= 1000)
				type = elementIDStrings[elemId - 1000 + hhElementCount];
			else
				type = elementIDStrings[elemId];
		}
		snprintf(nodeName, sizeof(nodeName), "%s (%s) - %zx##%s", node->nodeName.c_str(), type, (size_t)node, node->guid);

		bool isOpen = false;

		if(node->childrenElements0.size() == 0 && node->childrenPath.size() == 0 && node->childrenCamera.size() == 0 && node->childrenMotion.size() == 0 && node->childrenElements1.size() == 0 && node->childrenElements2.size() == 0)
			ImGui::TreeNodeEx(nodeName, nodeFlags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen);
		else
			isOpen = ImGui::TreeNodeEx(nodeName, nodeFlags);;

        if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
			selectionBehavior->Select(selection);
        
		if (ImGui::BeginPopupContextItem("Options")) {
			auto& ctx = GetContext();
			if (ImGui::TreeNode("Add Node")) {
				for (auto& category : categorizedNodes) {
					if (ImGui::TreeNode(category.name)) {
						for (auto& cnode : category.items) {
							if (cnode != std::pair<unsigned int, bool>{}) {
								const char* nodeName = nodeTypeNames[cnode.first];
								unsigned int nodeType = cnode.first;
								unsigned int elementId = 0;
								if (cnode.second) {
									nodeType = static_cast<unsigned int>(hh::dv::DvNodeBase::NodeType::ELEMENT);
									elementId = cnode.first;
									if (cnode.first >= 1000)
										nodeName = elementIDStrings[cnode.first - 1000 + hhElementCount];
									else
										nodeName = elementIDStrings[cnode.first];
								}
								if (ImGui::Selectable(nodeName)) {
									char* name = new char[strlen(nodeName) + 6];
									strcpy(name, nodeName);
									srand(time(nullptr));
									for (int i = 0; i < 5; ++i)
										name[strlen(nodeName) + i] = '0' + (rand() % 10);
									name[strlen(nodeName) + 5] = '\0';
									auto* newNode = ctx.CreateNode(name, nodeType, elementId, node);
									ctx.ParentNode(node, newNode);
									delete[] name;
								}
							}
						}
						ImGui::TreePop();
					}
				}
				ImGui::TreePop();
			}
			ImGui::EndPopup();
		}

        if(isOpen){
            for(auto* x : node->childrenElements0)
                RenderNode(x);
			for (auto* x : node->childrenPath)
				RenderNode(x);
			for (auto* x : node->childrenCamera)
				RenderNode(x);
			for (auto* x : node->childrenMotion)
				RenderNode(x);
			for (auto* x : node->childrenElements1)
				RenderNode(x);
			for (auto* x : node->childrenElements2)
				RenderNode(x);
            ImGui::TreePop();
        }

		ImGui::PopID();
    }

	void Tree::RenderPanel()
	{
		auto& context = GetContext();

		if (context.goDVSC == nullptr){
            ImGui::Text("No DvSceneControl selected");
			return;
		}
        
        RenderNode(context.goDVSC->nodeTree->mainNode);
	}

	PanelTraits Tree::GetPanelTraits() const
	{
		return { "Node Tree", ImVec2(0, 0), ImVec2(250, ImGui::GetMainViewport()->WorkSize.y - 600) };
	}
}
