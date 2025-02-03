#include "ProjectTree.h"
#include "SurfRideElement.h"
#include "texture-editor/TextureEditor.h"
#include <ucsl-reflection/reflections/resources/swif/v6.h>
//#include <rip/binary/containers/swif/SWIF.h>
#include <ui/common/StandaloneOperationModeHost.h>
#include <ui/common/viewers/Basic.h>
#include <ui/GlobalSettings.h>
#include <span>

namespace ui::operation_modes::modes::surfride_editor {
	void ProjectTree::RenderPanel() {
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
							context.projectResource = context.gocSprite->projectResource;
							context.project = context.gocSprite->project->projectData;
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

#ifdef DEVTOOLS_TARGET_SDK_rangers
		Viewer("Resource", context.gocSprite->projectResource->GetName());
#endif

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
				//auto* exportProjectData = static_cast<ucsl::resources::swif::v6::SRS_PROJECT*>(ImGuiFileDialog::Instance()->GetUserDatas());

				//std::ofstream ofs{ ImGuiFileDialog::Instance()->GetFilePathName(), std::ios::trunc | std::ios::binary };
				//rip::binary::containers::swif::v6::SWIFSerializer serializer{ ofs };
				//serializer.serialize<he2sdk::ucsl::GameInterface>(*exportProjectData);
			}
			ImGuiFileDialog::Instance()->Close();
		}

		const char* sceneName = context.focusedScene ? context.focusedScene->name : "<none>";

		if (ImGui::BeginCombo("Scene", sceneName)) {
			for (auto& scene : std::span(context.project->scenes, context.project->sceneCount)) {
				if (ImGui::Selectable(scene.name, &scene == context.focusedScene)) {
					GetBehavior<SelectionBehavior<Context>>()->DeselectAll();
					context.focusedScene = &scene;
				}
				if (context.focusedScene == &scene)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		if (!context.focusedScene) {
			return;
		}

		const ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

		auto* selectionBehavior = GetBehavior<SelectionBehavior<Context>>();
		auto& selected = selectionBehavior->GetSelection();

		RenderElement(*context.focusedScene);
	}

	PanelTraits ProjectTree::GetPanelTraits() const {
		return { "Project tree", ImVec2(0, 0), ImVec2(250, ImGui::GetMainViewport()->WorkSize.y - 600) };
	}

	void ProjectTree::RenderElement(ucsl::resources::swif::v6::SRS_CAMERA& camera) {
		BeginElement(camera, false);
	}

	void ProjectTree::RenderElement(ucsl::resources::swif::v6::SRS_SCENE& scene) {
		if (BeginElement(scene, true)) {
			ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

			if (ImGui::TreeNodeEx("Cameras", nodeFlags)) {
				for (auto& camera : std::span(scene.cameras, scene.cameraCount))
					RenderElement(camera);
				ImGui::TreePop();
			}
			if (ImGui::TreeNodeEx("Layers", nodeFlags)) {
				for (auto& layer : std::span(scene.layers, scene.layerCount))
					RenderElement(layer);
				ImGui::TreePop();
			}

			EndElement();
		}
	}

	void ProjectTree::RenderElement(ucsl::resources::swif::v6::SRS_LAYER& layer) {
		if (BeginElement(layer, layer.castCount > 0)) {
			GetContext().ForEachRoot(layer, [this, &layer](auto& cast) {
				RenderElement(layer, cast);
			});
			EndElement();
		}
	}

	void ProjectTree::RenderElement(ucsl::resources::swif::v6::SRS_LAYER& layer, ucsl::resources::swif::v6::SRS_CASTNODE& cast) {
		bool hasChildren = cast.childIndex != -1;
		bool hasRefLayer = cast.GetType() == ucsl::resources::swif::v6::SRS_CASTNODE::Type::REFERENCE && cast.data.reference->layer != nullptr;

		if (BeginElement(cast, hasChildren || hasRefLayer)) {
			if (hasRefLayer) {
				if (auto* refLayer = cast.data.reference->layer) {
					auto textCol = ImGui::GetStyleColorVec4(ImGuiCol_Text);
					textCol.w = 0.5;
					ImGui::PushStyleColor(ImGuiCol_Text, textCol);
					RenderElement(*cast.data.reference->layer);
					ImGui::PopStyleColor();
				}
			}

			GetContext().ForEachChild(layer, cast, [this, &layer](auto& child) {
				RenderElement(layer, child);
			});

			EndElement();
		}
	}


	bool ProjectTree::HasContextMenu(ucsl::resources::swif::v6::SRS_SCENE& scene) { return false; }
	bool ProjectTree::HasContextMenu(ucsl::resources::swif::v6::SRS_CAMERA& camera) { return false; }
	bool ProjectTree::HasContextMenu(ucsl::resources::swif::v6::SRS_LAYER& layer) { return true; }
	bool ProjectTree::HasContextMenu(ucsl::resources::swif::v6::SRS_CASTNODE& cast) { return true; }
	

	void ProjectTree::RenderContextMenu(ucsl::resources::swif::v6::SRS_SCENE& scene) {}
	void ProjectTree::RenderContextMenu(ucsl::resources::swif::v6::SRS_CAMERA& camera) {}

	void ProjectTree::RenderContextMenu(ucsl::resources::swif::v6::SRS_LAYER& layer) {
		if (ImGui::BeginMenu("Add")) {
			if (ImGui::MenuItem("Image cast"))
				GetContext().AddImageCast(layer);
			ImGui::EndMenu();
		}
	}

	void ProjectTree::RenderContextMenu(ucsl::resources::swif::v6::SRS_CASTNODE& cast) {
		if (ImGui::BeginMenu("Add")) {
			if (ImGui::MenuItem("Image cast"))
				GetContext().AddImageCast(cast);
			ImGui::EndMenu();
		}
	}
}
