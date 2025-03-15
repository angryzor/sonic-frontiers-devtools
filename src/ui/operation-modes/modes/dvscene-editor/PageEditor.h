#pragma once
#include <ui/operation-modes/Panel.h>
#include <ui/common/NodeEditor.h>
#include "Context.h"
#include <ui/common/editors/Basic.h>
#include <ui/common/viewers/Basic.h>

namespace ui::operation_modes::modes::dvscene_editor
{
    enum class NodeType {
		PAGE,
		CONDITION
	};

	enum class PinType {
		TRANSITION
	};

    struct NodeId {
		NodeType type{};
		int idx{};

		NodeId() = default;
		NodeId(NodeType type, int idx);
        NodeId(unsigned long long nodeId);
		NodeId(ax::NodeEditor::NodeId nodeId);
		operator unsigned long long() const;
		operator ax::NodeEditor::NodeId() const;
	};

	struct PinId {
		NodeId nodeId{};
		ax::NodeEditor::PinKind kind{};
		PinType type{};
		int idx{};

		PinId() = default;
		PinId(const NodeId& nodeId, ax::NodeEditor::PinKind kind, PinType type, int idx);
		PinId(unsigned long long nodeId);
		PinId(ax::NodeEditor::PinId nodeId);
		operator unsigned long long() const;
		operator ax::NodeEditor::PinId() const;
	};

    struct InputPinId : PinId {
		InputPinId() = default;
		InputPinId(const NodeId& nodeId, PinType type, int idx);
		InputPinId(unsigned long long nodeId);
		InputPinId(ax::NodeEditor::PinId nodeId);
	};

	struct OutputPinId : PinId {
		OutputPinId() = default;
		OutputPinId(const NodeId& nodeId, PinType type, int idx);
		OutputPinId(unsigned long long nodeId);
		OutputPinId(ax::NodeEditor::PinId nodeId);
	};

    class NodeEditorInterface {
    public:
        using PinType = PinType;
        static void RenderPinIcon(PinType type);
        static ImVec4 GetColor(PinType type);
    };

    class PageEditor : public Panel<Context> {
    public:
        ::NodeEditor<NodeEditorInterface> nodeEditor;

        using Panel::Panel;

        PageEditor(csl::fnd::IAllocator* allocator, OperationMode<Context>& operationMode);

        virtual void RenderPanel() override;
        bool RenderPage(int pageIdx);
		bool RenderCondition(int pageIdx, int transitionIdx, int conditionIdx);
		void RenderTransition(int pageIdx, int transitionIdx);
        virtual PanelTraits GetPanelTraits() const override;

		ax::NodeEditor::LinkId GetLinkId(ax::NodeEditor::PinId fromPinId, ax::NodeEditor::PinId toPinId);
		void Link(const OutputPinId& fromPin, const InputPinId& toPin);
    };
}