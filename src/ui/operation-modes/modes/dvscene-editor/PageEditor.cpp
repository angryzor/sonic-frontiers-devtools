#include "PageEditor.h"
#include "Behaviors.h"
#include <ui/common/StandaloneOperationModeHost.h>

namespace ui::operation_modes::modes::dvscene_editor {
    namespace NodeEd = ax::NodeEditor;

	ImVec4 NodeEditorInterface::GetColor(PinType type)
	{
		switch (type) {
		case PinType::TRANSITION: return { 0.0f, 0.5f, 0.8f, 1.0f };
		default: assert(false); return { 0.0f, 0.0f, 0.0f, 0.0f }; break;
		}
	}

	void NodeEditorInterface::RenderPinIcon(PinType type) {
		auto size = ImGui::GetTextLineHeight();
		auto color = ImGui::GetColorU32(GetColor(type));

		ImGui::Dummy({ size, size });

		auto itemRectMin = ImGui::GetItemRectMin();
		auto itemRectMax = ImGui::GetItemRectMax();

		switch (type) {
		case PinType::TRANSITION: {
			ImVec2 arrowHead[] = { { itemRectMin.x, itemRectMin.y }, { itemRectMin.x + size / 2, itemRectMin.y }, { itemRectMin.x + size, itemRectMin.y + size / 2 }, { itemRectMin.x + size / 2, itemRectMin.y + size }, { itemRectMin.x, itemRectMin.y + size } };
			ImGui::GetWindowDrawList()->AddConvexPolyFilled(arrowHead, 5, color);
			break;
		}
		default: {
			assert(false);
			break;
		}
		}
	}

    PageEditor::PageEditor(csl::fnd::IAllocator* allocator, OperationMode<Context>& operationMode) : Panel{ allocator, operationMode }, nodeEditor{ allocator } {}

    void PageEditor::RenderPanel() {
		auto& context = GetContext();
        if (context.goDVSC == nullptr) {
            ImGui::Text("No DvSceneControl selected");
			return;
		}
		if (ImGui::Selectable("Auto Layout"))
			nodeEditor.RunAutoLayout();
        nodeEditor.Begin();
        for(auto* page : context.goDVSC->timeline->pages)
            RenderPage(page->binaryData.pageIndex);
		for (auto* page : context.goDVSC->timeline->pages)
			for (auto* trans : page->transitions)
				RenderTransition(page->binaryData.pageIndex, trans);
        nodeEditor.End();
    }

    void PageEditor::RenderPage(int pageIdx) {
		auto& context = GetContext();
		auto* page = context.goDVSC->timeline->pages[pageIdx];
        ax::NodeEditor::PushStyleColor(ax::NodeEditor::StyleColor_NodeBorder, {0.5f, 0.5f, 0.5f, 1.0f});
		NodeId nodeId{ NodeType::PAGE, pageIdx };

		nodeEditor.BeginNode(nodeId, ImGui::CalcTextSize(page->binaryData.pageName).x);

		nodeEditor.BeginInputPins();
		InputPinId inpin{ nodeId, PinType::TRANSITION, 0 };
        nodeEditor.BeginInputPin(inpin, PinType::TRANSITION);
        nodeEditor.EndInputPin();
		nodeEditor.EndInputPins();

		nodeEditor.BeginControls();
		float progress = (static_cast<float>(context.goDVSC->timeline->preCurrentFrame) - page->binaryData.start) / (page->binaryData.end - page->binaryData.start);
		if (context.goDVSC->timeline->currentPage == pageIdx)
			ImGui::ProgressBar(progress, {150, 0});
		int start = page->binaryData.start / 100;
		if (Editor("Start", start))
			page->binaryData.start = start * 100;
		int end = page->binaryData.end / 100;
		if (Editor("End", end))
			page->binaryData.end = end * 100;
		int skipFrame = page->binaryData.skipFrame / 100;
		if (Editor("Skip Frame", skipFrame))
			page->binaryData.skipFrame = skipFrame * 100;
        Editor("Page Name", page->binaryData.pageName);
		nodeEditor.EndControls();

		nodeEditor.BeginOutputPins();
		OutputPinId outpin{ nodeId, PinType::TRANSITION, 0 };
        nodeEditor.BeginOutputPin(outpin, 0.0f, PinType::TRANSITION);
        nodeEditor.EndOutputPin();
		nodeEditor.EndOutputPins();

		nodeEditor.EndNode();

		ax::NodeEditor::PopStyleColor();
    }

	void PageEditor::RenderTransition(int pageIdx, hh::dv::DvPageTransition* trans)
	{
		auto x = OutputPinId{ { NodeType::PAGE, pageIdx }, PinType::TRANSITION, 0 };
		auto y = InputPinId{ { NodeType::PAGE, trans->binaryData.destinationPageID }, PinType::TRANSITION, 0 };
		Link(x, y);
	}

    PanelTraits PageEditor::GetPanelTraits() const
	{
		return { "Page Editor", ImVec2(0, 0), ImVec2(250, 250) };
	}

    NodeId::NodeId(NodeType type, int idx) : type{ type }, idx{ idx } {}

	NodeId::NodeId(unsigned long long nodeId)
	{
		type = static_cast<NodeType>((nodeId >> 16) & 0xF);
		idx = nodeId & 0xFFFFFFFF;
	}

	NodeId::NodeId(ax::NodeEditor::NodeId nodeId) : NodeId(nodeId.Get()) {}

	NodeId::operator unsigned long long() const
	{
		unsigned long long lType = static_cast<unsigned long long>(type);
		unsigned long long lIdx = idx;

		return (lType << 16) | lIdx;
	}

	NodeId::operator ax::NodeEditor::NodeId() const
	{
		return static_cast<unsigned long long>(*this);
	}

	PinId::PinId(const NodeId& nodeId, ax::NodeEditor::PinKind kind, PinType type, int idx) : nodeId{ nodeId }, kind{ kind }, type{ type }, idx{ idx } {}

	PinId::PinId(unsigned long long pinId)
	{
		ax::NodeEditor::NodeId lNodeId = (pinId >> 12) & 0xFFFFF;

		nodeId = lNodeId;
		kind = static_cast<ax::NodeEditor::PinKind>((pinId >> 11) & 0x1);
		type = static_cast<PinType>((pinId >> 7) & 0xF);
		idx = pinId & 0x7F;
	}

	PinId::PinId(ax::NodeEditor::PinId pinId) : PinId(pinId.Get()) {}

	PinId::operator unsigned long long() const
	{
		unsigned long long lNodeId = nodeId;
		unsigned long long lKind = static_cast<unsigned long long>(kind);
		unsigned long long lType = static_cast<unsigned long long>(type);
		unsigned long long lIdx = idx;

		return (lNodeId << 12) | (lKind << 11) | (lType << 7) | lIdx;
	}

	PinId::operator ax::NodeEditor::PinId() const
	{
		return static_cast<unsigned long long>(*this);
	}

    InputPinId::InputPinId(const NodeId& nodeId, PinType type, int idx) : PinId{ nodeId, ax::NodeEditor::PinKind::Input, type, idx } {}
	InputPinId::InputPinId(unsigned long long nodeId) : PinId{ nodeId } {}
	InputPinId::InputPinId(ax::NodeEditor::PinId nodeId) : PinId{ nodeId } {}

	OutputPinId::OutputPinId(const NodeId& nodeId, PinType type, int idx) : PinId{ nodeId, ax::NodeEditor::PinKind::Output, type, idx } {}
	OutputPinId::OutputPinId(unsigned long long nodeId) : PinId{ nodeId } {}
	OutputPinId::OutputPinId(ax::NodeEditor::PinId nodeId) : PinId{ nodeId } {}

	ax::NodeEditor::LinkId PageEditor::GetLinkId(ax::NodeEditor::PinId fromPinId, ax::NodeEditor::PinId toPinId) {
		unsigned long long lFromPinId = fromPinId.Get();
		unsigned long long lToPinId = toPinId.Get();

		return (lFromPinId << 32) | lToPinId;
	}

	void PageEditor::Link(const OutputPinId& fromPin, const InputPinId& toPin)
	{
		nodeEditor.Link(GetLinkId(fromPin, toPin), fromPin, toPin, fromPin.type);
	}
}