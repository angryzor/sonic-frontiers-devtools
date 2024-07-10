#pragma once

namespace ui::operation_modes::modes::bases::list_and_inspector {
	template<typename Context>
	class TreePanel : public Panel<Context> {
	public:
		using Panel::Panel;

		virtual PanelTraits GetPanelTraits() const override {
			return { "Objects", ImVec2(0, 0), ImVec2(250, ImGui::GetMainViewport()->WorkSize.y - 100) };
		}
	};

	template<typename Context>
	class DynamicTreePanel : public TreePanel<Context> {
	public:
		using TreePanel::TreePanel;

		virtual void RenderPanel() override {
			auto* selectionBehavior = this->template GetBehavior<SelectionBehavior<T>>();

			for (auto& item : this->GetContext()->GetRootItems()) {
				if (ImGui::Selectable(item->GetName(), selectionBehavior->GetSelection().find(item) != -1))
					selectionBehavior->Select(item);
			}
		}
	};

	template<typename Context>
	class CachedTreePanel : public TreePanel<Context> {
	public:
		using TreePanel::TreePanel;

		virtual void RenderPanel() override {
			auto* selectionBehavior = this->template GetBehavior<SelectionBehavior<T>>();

			for (auto& item : this->GetContext()->GetRootItems()) {
				if (ImGui::Selectable(item->GetName(), selectionBehavior->GetSelection().find(item) != -1))
					selectionBehavior->Select(item);
			}
		}
	};
}
