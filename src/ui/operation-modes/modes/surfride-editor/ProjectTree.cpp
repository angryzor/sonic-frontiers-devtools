#include "ProjectTree.h"
#include "SurfRideElement.h"
#include <ui/operation-modes/behaviors/Selection.h>

namespace ui::operation_modes::modes::surfride_editor {
	void ProjectTree::RenderPanel()
	{
		char spriteName[400];
		auto& context = GetContext();

		if (context.gocSprite != nullptr)
			snprintf(spriteName, sizeof(spriteName), "%s - %zx", context.gocSprite->owner->name.c_str(), reinterpret_cast<size_t>(&*context.gocSprite));
		else
			strcpy(spriteName, "none");

		if (ImGui::BeginCombo("GOCSprite", spriteName)) {
			for (auto* obj : hh::game::GameManager::GetInstance()->objects)
				for (auto* goc : obj->components)
					if (goc->pStaticClass == hh::ui::GOCSprite::GetClass()) {
						snprintf(spriteName, sizeof(spriteName), "%s - %zx", goc->owner->name.c_str(), reinterpret_cast<size_t>(goc));
						if (ImGui::Selectable(spriteName, goc == context.gocSprite)) {
							GetBehavior<SelectionBehavior<SurfRideElement>>()->DeselectAll();
							context.gocSprite = static_cast<hh::ui::GOCSprite*>(goc);
							context.focusedScene = nullptr;
						}
						if (context.gocSprite == goc)
							ImGui::SetItemDefaultFocus();
					}
			ImGui::EndCombo();
		}

		if (context.gocSprite == nullptr) {
			return;
		}

		auto* project = context.gocSprite->GetProject();
		if (project == nullptr) {
			return;
		}

		const char* sceneName = context.focusedScene ? context.focusedScene->sceneData->name : "<none>";

		if (ImGui::BeginCombo("Scene", sceneName)) {
			for (auto* scene : project->scenes) {
				if (ImGui::Selectable(scene->sceneData->name, scene == context.focusedScene)) {
					GetBehavior<SelectionBehavior<SurfRideElement>>()->DeselectAll();
					context.focusedScene = scene;
				}
				if (context.focusedScene == scene)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		auto* scene = context.focusedScene;
		if (scene == nullptr) {
			return;
		}

		const ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

		auto* selectionBehavior = GetBehavior<SelectionBehavior<SurfRideElement>>();
		auto& selected = selectionBehavior->GetSelection();

		if (ImGui::TreeNodeEx("Cameras", nodeFlags)) {
			for (size_t i = 0; i < scene->sceneData->cameraCount; i++) {
				ImGuiTreeNodeFlags itemNodeFlags = nodeFlags;
				SurfRideElement selection{ SurfRideElement::Type::CAMERA_DATA, &scene->sceneData->cameras[i] };

				if (selected.find(selection) != -1)
					itemNodeFlags |= ImGuiTreeNodeFlags_Selected;

				ImGui::TreeNodeEx(&scene->sceneData->cameras[i], nodeFlags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen, "%s", scene->sceneData->cameras[i].name);

				if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
					selectionBehavior->Select(selection);
			}
			ImGui::TreePop();
		}
		if (ImGui::TreeNodeEx("Layers", nodeFlags)) {
			for (auto* layer : scene->layers)
				RenderLayer(layer);
			ImGui::TreePop();
		}
	}

	PanelTraits ProjectTree::GetPanelTraits() const
	{
		return { "Project tree", ImVec2(0, 0), ImVec2(250, ImGui::GetMainViewport()->WorkSize.y - 600) };
	}

	void ProjectTree::RenderLayer(SurfRide::Layer* layer)
	{
		ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
		SurfRideElement selection{ SurfRideElement::Type::LAYER, layer };

		auto* selectionBehavior = GetBehavior<SelectionBehavior<SurfRideElement>>();
		auto& selected = selectionBehavior->GetSelection();

		if (selected.find(selection) != -1)
			nodeFlags |= ImGuiTreeNodeFlags_Selected;

		bool isOpen = ImGui::TreeNodeEx(layer, nodeFlags, "%s", layer->layerData->name);

		if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
			selectionBehavior->Select(selection);

		if (isOpen) {
			for (auto* cast : layer->topLevelCasts)
				RenderCast(cast);
			ImGui::TreePop();
		}
	}

	void ProjectTree::RenderCast(SurfRide::Cast* cast)
	{
		bool isOpen{ false };
		ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
		SurfRideElement selection{ SurfRideElement::Type::CAST, cast };
		SurfRide::Layer* refLayer{ static_cast<SurfRide::SRS_CASTNODE::Type>(cast->flags & 0xF) == SurfRide::SRS_CASTNODE::Type::REFERENCE ? static_cast<SurfRide::ReferenceCast*>(cast)->layer : nullptr };

		auto* selectionBehavior = GetBehavior<SelectionBehavior<SurfRideElement>>();
		auto& selected = selectionBehavior->GetSelection();

		if (selected.find(selection) != -1)
			nodeFlags |= ImGuiTreeNodeFlags_Selected;

		if (cast->children.size() == 0 && !refLayer)
			ImGui::TreeNodeEx(cast, nodeFlags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen, "%s", cast->castData->name);
		else
			isOpen = ImGui::TreeNodeEx(cast, nodeFlags, "%s", cast->castData->name);

		if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
			selectionBehavior->Select(selection);

		if (isOpen) {
			if (refLayer)
				RenderLayer(refLayer);

			for (auto* child : cast->children)
				RenderCast(child);

			ImGui::TreePop();
		}
	}
}