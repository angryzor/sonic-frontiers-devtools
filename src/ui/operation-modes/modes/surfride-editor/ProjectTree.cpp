#include "ProjectTree.h"
#include "SurfRideElement.h"
#include "Behaviors.h"
#include <io/binary/containers/swif/SWIF.h>

namespace ui::operation_modes::modes::surfride_editor {
	void ProjectTree::RenderPanel()
	{
		char spriteName[400];
		auto& context = GetContext();

		if (context.gocSprite != nullptr)
			snprintf(spriteName, sizeof(spriteName), "%s - %zx", context.gocSprite->owner->name.c_str(), reinterpret_cast<size_t>(&*context.gocSprite));
		else
			strcpy_s(spriteName, "none");

		if (ImGui::BeginCombo("GOCSprite", spriteName)) {
			for (auto* obj : hh::game::GameManager::GetInstance()->objects)
				for (auto* goc : obj->components)
					if (goc->pStaticClass == hh::ui::GOCSprite::GetClass()) {
						snprintf(spriteName, sizeof(spriteName), "%s - %zx", goc->owner->name.c_str(), reinterpret_cast<size_t>(goc));
						if (ImGui::Selectable(spriteName, goc == context.gocSprite)) {
							GetBehavior<SelectionBehavior<Context>>()->DeselectAll();
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

		if (ImGui::Button("Export")) {
			IGFD::FileDialogConfig cfg{};
			cfg.path = ".";
			cfg.flags = ImGuiFileDialogFlags_Modal | ImGuiFileDialogFlags_ConfirmOverwrite;
			cfg.userDatas = project->projectData;
			ImGuiFileDialog::Instance()->OpenDialog("ResSurfRideProjectExportDialog", "Choose File", ".swif", cfg);
		}

		if (ImGuiFileDialog::Instance()->Display("ResSurfRideProjectExportDialog", ImGuiWindowFlags_NoCollapse, ImVec2(800, 500))) {
			if (ImGuiFileDialog::Instance()->IsOk()) {
				auto* exportProjectData = static_cast<SurfRide::SRS_PROJECT*>(ImGuiFileDialog::Instance()->GetUserDatas());

				std::string filePath = ImGuiFileDialog::Instance()->GetFilePathName();
				std::wstring wFilePath(filePath.begin(), filePath.end());

				devtools::io::binary::containers::swif::Serialize(wFilePath.c_str(), exportProjectData);
			}
			ImGuiFileDialog::Instance()->Close();
		}

		const char* sceneName = context.focusedScene ? context.focusedScene->sceneData->name : "<none>";

		if (ImGui::BeginCombo("Scene", sceneName)) {
			for (auto* scene : project->GetScenes()) {
				if (ImGui::Selectable(scene->sceneData->name, scene == context.focusedScene)) {
					GetBehavior<SelectionBehavior<Context>>()->DeselectAll();
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

		auto* selectionBehavior = GetBehavior<SelectionBehavior<Context>>();
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
			for (auto* layer : scene->GetLayers())
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

		auto* selectionBehavior = GetBehavior<SelectionBehavior<Context>>();
		auto& selected = selectionBehavior->GetSelection();

		if (selected.find(selection) != -1)
			nodeFlags |= ImGuiTreeNodeFlags_Selected;

		bool isOpen = ImGui::TreeNodeEx(layer, nodeFlags, "%s", layer->layerData->name);

		if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
			selectionBehavior->Select(selection);

		if (isOpen) {
			for (auto* cast : layer->GetCasts())
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

		auto* selectionBehavior = GetBehavior<SelectionBehavior<Context>>();
		auto& selected = selectionBehavior->GetSelection();

		if (selected.find(selection) != -1)
			nodeFlags |= ImGuiTreeNodeFlags_Selected;

		if (!cast->GetChildren().empty() == 0 && !refLayer)
			ImGui::TreeNodeEx(cast, nodeFlags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen, "%s", cast->castData->name);
		else
			isOpen = ImGui::TreeNodeEx(cast, nodeFlags, "%s", cast->castData->name);

		if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
			selectionBehavior->Select(selection);

		if (isOpen) {
			if (refLayer)
				RenderLayer(refLayer);

			for (auto* child : cast->GetChildren())
				RenderCast(child);

			ImGui::TreePop();
		}
	}
}
