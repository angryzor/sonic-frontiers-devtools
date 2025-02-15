#include "Tree.h"
#include "Behaviors.h"
#include <ui/common/StandaloneOperationModeHost.h>
#include <ui/common/viewers/Basic.h>
#include <ui/GlobalSettings.h>

namespace ui::operation_modes::modes::dvscene_editor {
#ifdef DEVTOOLS_TARGET_SDK_rangers
	const char* nodeTypeNames1[] = {
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

#ifdef DEVTOOLS_TARGET_SDK_miller
	const char* nodeTypeNames1[] = {
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

    void Tree::RenderNode(hh::dv::DvNodeBase* node){
		ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
		DvNode selection{ node };

        auto* selectionBehavior = GetBehavior<SelectionBehavior<Context>>();
		auto& selected = selectionBehavior->GetSelection();

		if (selected.find(selection) != -1)
			nodeFlags |= ImGuiTreeNodeFlags_Selected;

		char nodeName[256];
		snprintf(nodeName, sizeof(nodeName), "%s (%s) - %zx##%s", node->nodeName.c_str(), nodeTypeNames1[static_cast<int>(node->nodeType)], (size_t)node, node->guid);

		bool isOpen = false;

		if(node->childrenElements0.size() == 0 && node->childrenPath.size() == 0 && node->childrenCamera.size() == 0 && node->childrenMotion.size() == 0 && node->childrenElements1.size() == 0 && node->childrenElements2.size() == 0)
			ImGui::TreeNodeEx(nodeName, nodeFlags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen);
		else
			isOpen = ImGui::TreeNodeEx(nodeName, nodeFlags);;

        if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
			selectionBehavior->Select(selection);
        
        if(isOpen){
			#ifdef DEVTOOLS_TARGET_SDK_rangers
			if (ImGui::BeginPopupContextItem("Options")) {
				if (ImGui::Selectable("Add Element")) {
					char* setupInfoBuffer = new char[sizeof(hh::dv::DvNodeElement::Data) + sizeof(hh::dv::DvElementEffect::Data)];
					memset(setupInfoBuffer, 0x00, sizeof(hh::dv::DvNodeElement::Data) + sizeof(hh::dv::DvElementEffect::Data));
					auto* elementInfo = reinterpret_cast<hh::dv::DvNodeElement::Data*>(setupInfoBuffer);
					elementInfo->start = 0.0f;
					elementInfo->end = 100.0f;
					elementInfo->elementId = hh::dv::DvNodeElement::ElementID::EFFECT;
					auto* effectInfo = reinterpret_cast<hh::dv::DvElementEffect::Data*>(&setupInfoBuffer[32]);
					strcpy(effectInfo->effectName, "eff_bo4110_ge01");
					effectInfo->effectMatrix = csl::math::Matrix44::Identity();
					void* rawMem = malloc(sizeof(hh::dv::DvNodeElement));
					hh::dv::DvNodeElement* dvNodeElement = new (rawMem) hh::dv::DvNodeElement(hh::fnd::MemoryRouter::GetModuleAllocator());
					dvNodeElement->dvsceneNodeTree = node->dvsceneNodeTree;
					dvNodeElement->nodeType = hh::dv::DvNodeBase::NodeType::ELEMENT;
					dvNodeElement->parent = node;
					strcpy(dvNodeElement->guid, "\xe8\xbe\x49\x56\x04\x97\x4b\xd4\x97\xe0\x70\xb9\x4e\x75\x98\x84");
					dvNodeElement->nodeName.push_back('l');
					dvNodeElement->nodeName.push_back('o');
					dvNodeElement->nodeName.push_back('l');
					dvNodeElement->nodeName.push_back('\0');
					dvNodeElement->Setup(setupInfoBuffer);
					node->childrenElements0.push_back(dvNodeElement);
				}
				ImGui::EndPopup();
			}
			#endif

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
