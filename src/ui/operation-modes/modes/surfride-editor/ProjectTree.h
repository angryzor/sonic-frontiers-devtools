#pragma once
#include <ui/operation-modes/Panel.h>
#include "Context.h"
#include "Behaviors.h"

namespace ui::operation_modes::modes::surfride_editor {
	class ProjectTree : public Panel<Context> {
		void RenderElement(ucsl::resources::swif::swif_version::SRS_SCENE& scene, SurfRide::Scene* runtimeScene);
		void RenderElement(ucsl::resources::swif::swif_version::SRS_CAMERA& camera, SurfRide::Camera* runtimeCamera);
		void RenderElement(ucsl::resources::swif::swif_version::SRS_LAYER& layer, SurfRide::Layer* runtimeLayer);
		void RenderElement(ucsl::resources::swif::swif_version::SRS_LAYER& layer, ucsl::resources::swif::swif_version::SRS_CASTNODE& cast, SurfRide::Cast* runtimeCast);
		bool HasContextMenu(ucsl::resources::swif::swif_version::SRS_SCENE& scene);
		bool HasContextMenu(ucsl::resources::swif::swif_version::SRS_CAMERA& camera);
		bool HasContextMenu(ucsl::resources::swif::swif_version::SRS_LAYER& layer);
		bool HasContextMenu(ucsl::resources::swif::swif_version::SRS_CASTNODE& cast);
		void RenderContextMenu(ucsl::resources::swif::swif_version::SRS_SCENE& scene);
		void RenderContextMenu(ucsl::resources::swif::swif_version::SRS_CAMERA& camera);
		void RenderContextMenu(ucsl::resources::swif::swif_version::SRS_LAYER& layer);
		void RenderContextMenu(ucsl::resources::swif::swif_version::SRS_CASTNODE& cast);
	public:
		using Panel::Panel;

		virtual void RenderPanel() override;
		virtual PanelTraits GetPanelTraits() const override;

		template<typename T, typename U, typename... A>
		bool BeginElement(T& element, U* runtimeElement, bool hasContent, const A&... prefix) {
			ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

			if (!hasContent)
				nodeFlags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

			auto* selectionBehavior = GetBehavior<SelectionBehavior<Context>>();
			auto& selected = selectionBehavior->GetSelection();

			if (selected.find(SurfRideElement{ prefix..., element, runtimeElement }) != -1)
				nodeFlags |= ImGuiTreeNodeFlags_Selected;

			char ids[20];
			sprintf_s(ids, "%d", element.id);
			bool isOpen = ImGui::TreeNodeEx(ids, nodeFlags, "%s", element.name);

			if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
				selectionBehavior->Select(SurfRideElement{ prefix..., element, runtimeElement });

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
