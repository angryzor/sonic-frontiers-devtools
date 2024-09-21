#include "ProjectTree.h"
#include "Behaviors.h"
#include <io/binary/containers/swif/SWIF.h>
#include <ui/common/StandaloneOperationModeHost.h>
#include <ui/common/viewers/Basic.h>
#include <ui/GlobalSettings.h>

namespace ui::operation_modes::modes::asm_editor {
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

		Viewer("Resource", context.gocSprite->projectResource->GetName());

		if (ImGui::Button("Export")) {
			IGFD::FileDialogConfig cfg{};
			cfg.path = GlobalSettings::defaultFileDialogDirectory;
			cfg.flags = ImGuiFileDialogFlags_Modal | ImGuiFileDialogFlags_ConfirmOverwrite;
			cfg.userDatas = project->projectData;
			ImGuiFileDialog::Instance()->OpenDialog("ResSurfRideProjectExportDialog", "Choose File", ".swif", cfg);
		}

		ImGui::SameLine();
		if (ImGui::Button("Texture editor")) {
			auto* host = new (GetAllocator()) StandaloneOperationModeHost<texture_editor::TextureEditor>{ GetAllocator() };
			host->operationMode.GetContext().gocSprite = context.gocSprite;
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
}
