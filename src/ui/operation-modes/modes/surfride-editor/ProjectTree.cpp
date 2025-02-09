#include "ProjectTree.h"
#include "SurfRideElement.h"
#include "Actions.h"
#include <ui/common/viewers/Basic.h>
#include <span>

namespace ui::operation_modes::modes::surfride_editor {
	using namespace ucsl::resources::swif::v6;

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
						if (ImGui::Selectable(spriteName, goc == context.gocSprite))
							Dispatch(SetResourceAction{ static_cast<hh::ui::GOCSprite*>(goc) });
						if (context.gocSprite == goc)
							ImGui::SetItemDefaultFocus();
					}
			ImGui::EndCombo();
		}

		if (context.gocSprite == nullptr || context.projectResource == nullptr || context.project == nullptr)
			return;

#ifdef DEVTOOLS_TARGET_SDK_rangers
		Viewer("Resource", context.projectResource->GetName());
#endif

		if (ImGui::Button("Export"))
			Dispatch(ExportResourceAction{});

		ImGui::SameLine();
		if (ImGui::Button("Texture editor"))
			Dispatch(OpenTextureEditorAction{});

		const char* sceneName = context.focusedScene ? context.focusedScene->name : "<none>";

		if (ImGui::BeginCombo("Scene", sceneName)) {
			for (auto& scene : std::span(context.project->scenes, context.project->sceneCount)) {
				if (ImGui::Selectable(scene.name, &scene == context.focusedScene))
					Dispatch(SetFocusedSceneAction{ &scene });
				if (context.focusedScene == &scene)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		if (!context.focusedScene)
			return;

		RenderElement(*context.focusedScene);
	}

	PanelTraits ProjectTree::GetPanelTraits() const {
		return { "Project tree", ImVec2(0, 0), ImVec2(250, ImGui::GetMainViewport()->WorkSize.y - 600) };
	}

	void ProjectTree::RenderElement(SRS_CAMERA& camera) {
		BeginElement(camera, false);
	}

	void ProjectTree::RenderElement(SRS_SCENE& scene) {
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

	void ProjectTree::RenderElement(SRS_LAYER& layer) {
		if (BeginElement(layer, layer.castCount > 0)) {
			GetContext().ForEachRoot(layer, [this, &layer](auto& cast) {
				RenderElement(layer, cast);
			});
			EndElement();
		}
	}

	void ProjectTree::RenderElement(SRS_LAYER& layer, SRS_CASTNODE& cast) {
		bool hasChildren = cast.childIndex != -1;
		bool hasRefLayer = cast.GetType() == SRS_CASTNODE::Type::REFERENCE && cast.data.reference->layer != nullptr;
		
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


	bool ProjectTree::HasContextMenu(SRS_SCENE& scene) { return false; }
	bool ProjectTree::HasContextMenu(SRS_CAMERA& camera) { return false; }
	bool ProjectTree::HasContextMenu(SRS_LAYER& layer) { return true; }
	bool ProjectTree::HasContextMenu(SRS_CASTNODE& cast) { return true; }
	

	void ProjectTree::RenderContextMenu(SRS_SCENE& scene) {}
	void ProjectTree::RenderContextMenu(SRS_CAMERA& camera) {}

	void ProjectTree::RenderContextMenu(SRS_LAYER& layer) {
		if (ImGui::BeginMenu("Add")) {
			if (ImGui::MenuItem("Cast"))
				Dispatch(AddCastToLayerAction{ { layer, SRS_CASTNODE::Type::NORMAL } });
			if (ImGui::MenuItem("Image cast"))
				Dispatch(AddCastToLayerAction{ { layer, SRS_CASTNODE::Type::IMAGE } });
			if (ImGui::MenuItem("Slice cast"))
				Dispatch(AddCastToLayerAction{ { layer, SRS_CASTNODE::Type::SLICE } });
			if (ImGui::MenuItem("Reference cast"))
				Dispatch(AddCastToLayerAction{ { layer, SRS_CASTNODE::Type::REFERENCE } });
			ImGui::EndMenu();
		}
	}

	void ProjectTree::RenderContextMenu(SRS_CASTNODE& cast) {
		if (ImGui::BeginMenu("Add")) {
			if (ImGui::MenuItem("Cast"))
				Dispatch(AddCastToCastAction{ { cast, SRS_CASTNODE::Type::NORMAL } });
			if (ImGui::MenuItem("Image cast"))
				Dispatch(AddCastToCastAction{ { cast, SRS_CASTNODE::Type::IMAGE } });
			if (ImGui::MenuItem("Slice cast"))
				Dispatch(AddCastToCastAction{ { cast, SRS_CASTNODE::Type::SLICE } });
			if (ImGui::MenuItem("Reference cast"))
				Dispatch(AddCastToCastAction{ { cast, SRS_CASTNODE::Type::REFERENCE } });
			ImGui::EndMenu();
		}
		if (ImGui::MenuItem("Remove")) {
		}
	}
}
