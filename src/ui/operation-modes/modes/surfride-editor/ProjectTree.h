#pragma once
#include <ui/operation-modes/Panel.h>
#include "Context.h"
#include "Behaviors.h"

namespace ui::operation_modes::modes::surfride_editor {
	class ProjectTree : public Panel<Context> {
		void RenderElement(ucsl::resources::swif::v6::SRS_SCENE& scene);
		void RenderElement(ucsl::resources::swif::v6::SRS_CAMERA& camera);
		void RenderElement(ucsl::resources::swif::v6::SRS_LAYER& layer);
		void RenderElement(ucsl::resources::swif::v6::SRS_LAYER& layer, ucsl::resources::swif::v6::SRS_CASTNODE& cast);
		bool HasContextMenu(ucsl::resources::swif::v6::SRS_SCENE& scene);
		bool HasContextMenu(ucsl::resources::swif::v6::SRS_CAMERA& camera);
		bool HasContextMenu(ucsl::resources::swif::v6::SRS_LAYER& layer);
		bool HasContextMenu(ucsl::resources::swif::v6::SRS_CASTNODE& cast);
		void RenderContextMenu(ucsl::resources::swif::v6::SRS_SCENE& scene);
		void RenderContextMenu(ucsl::resources::swif::v6::SRS_CAMERA& camera);
		void RenderContextMenu(ucsl::resources::swif::v6::SRS_LAYER& layer);
		void RenderContextMenu(ucsl::resources::swif::v6::SRS_CASTNODE& cast);
	public:
		using Panel::Panel;

		virtual void RenderPanel() override;
		virtual PanelTraits GetPanelTraits() const override;

		template<typename T>
		bool BeginElement(T& element, bool hasContent) {
			ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

			if (!hasContent)
				nodeFlags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

			auto* selectionBehavior = GetBehavior<SelectionBehavior<Context>>();
			auto& selected = selectionBehavior->GetSelection();

			if (selected.find(element) != -1)
				nodeFlags |= ImGuiTreeNodeFlags_Selected;

			char ids[20];
			sprintf_s(ids, "%d", element.id);
			bool isOpen = ImGui::TreeNodeEx(ids, nodeFlags, "%s", element.name);

			if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
				selectionBehavior->Select(element);

			if (HasContextMenu(element) && ImGui::BeginPopupContextItem()) {
				RenderContextMenu(element);
				ImGui::EndPopup();
			}

			return hasContent && isOpen;
		}

		void EndElement() {
			ImGui::TreePop();
		}
	};
}
