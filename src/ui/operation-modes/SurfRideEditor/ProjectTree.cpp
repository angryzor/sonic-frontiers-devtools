#include "ProjectTree.h"
#include "SurfRideEditor.h"

ProjectTree::ProjectTree(csl::fnd::IAllocator* allocator, SurfRideEditor& editor) : BaseObject{ allocator }, editor{ editor }
{
}

void ProjectTree::Render()
{
	const ImGuiWindowFlags windowFlags
		= 0;

	ImGui::SetNextWindowSize(ImVec2(250, ImGui::GetMainViewport()->WorkSize.y - 600), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Project tree", NULL, windowFlags)) {
		char spriteName[400];

		if (editor.gocSprite != nullptr)
			snprintf(spriteName, sizeof(spriteName), "%s - %zx", editor.gocSprite->owner->name.c_str(), reinterpret_cast<size_t>(&*editor.gocSprite));
		else
			strcpy(spriteName, "none");

		if (ImGui::BeginCombo("GOCSprite", spriteName)) {
			for (auto* obj : hh::game::GameManager::GetInstance()->objects)
				for (auto* goc : obj->components)
					if (goc->pStaticClass == hh::ui::GOCSprite::GetClass()) {
						snprintf(spriteName, sizeof(spriteName), "%s - %zx", goc->owner->name.c_str(), reinterpret_cast<size_t>(goc));
						if (ImGui::Selectable(spriteName, goc == editor.gocSprite))
							editor.gocSprite = static_cast<hh::ui::GOCSprite*>(goc);
						if (editor.gocSprite == goc)
							ImGui::SetItemDefaultFocus();
					}
			ImGui::EndCombo();
		}

		if (editor.gocSprite == nullptr) {
			ImGui::End();
			return;
		}

		if (!editor.gocSprite->GetProject()) {
			ImGui::End();
			return;
		}

		const ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnDoubleClick;

		for (auto* scene : editor.gocSprite->GetProject()->scenes) {
			bool isOpen = ImGui::TreeNode(scene, "%s", scene->sceneData->pName);

			if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
				editor.focusedElements.clear();
				editor.focusedElements.push_back({ SurfRideEditor::Selection::Type::SCENE, scene });
			}

			if (isOpen) {
				if (ImGui::TreeNode("Cameras")) {
					for (size_t i = 0; i < scene->sceneData->CameraCount; i++) {
						ImGui::TreeNodeEx(&scene->sceneData->pCameras[i], ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen, "%s", scene->sceneData->pCameras[i].pName);

						if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
							editor.focusedElements.clear();
							editor.focusedElements.push_back({ SurfRideEditor::Selection::Type::CAMERA_DATA, &scene->sceneData->pCameras[i] });
						}
					}
					ImGui::TreePop();
				}
				if (ImGui::TreeNode("Layers")) {
					for (auto* layer : scene->layers)
						RenderLayer(layer);
					ImGui::TreePop();
				}
				ImGui::TreePop();
			}
		}
	}
	ImGui::End();
}

void ProjectTree::RenderLayer(SurfRide::Layer* layer)
{
	bool isOpen = ImGui::TreeNode(layer, "%s", layer->layerData->pName);

	if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
		editor.focusedElements.clear();
		editor.focusedElements.push_back({ SurfRideEditor::Selection::Type::LAYER, layer });
	}

	if (isOpen) {
		for (auto* cast : layer->casts)
			RenderCast(cast);
		ImGui::TreePop();
	}
}

void ProjectTree::RenderCast(SurfRide::Cast* cast)
{
	bool isOpen{ false };
	if (cast->children.size() == 0)
		ImGui::TreeNodeEx(cast, ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen, "%s", cast->castData->m_pName);
	else
		isOpen = ImGui::TreeNode(cast, "%s", cast->castData->m_pName);

	if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
		editor.focusedElements.clear();
		editor.focusedElements.push_back({ SurfRideEditor::Selection::Type::CAST, cast });
	}
	
	if (isOpen) {
		for (auto* child : cast->children)
			RenderCast(child);
		ImGui::TreePop();
	}
}
