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
						if (ImGui::Selectable(spriteName, goc == editor.gocSprite)) {
							editor.gocSprite = static_cast<hh::ui::GOCSprite*>(goc);
							editor.focusedScene = nullptr;
						}
						if (editor.gocSprite == goc)
							ImGui::SetItemDefaultFocus();
					}
			ImGui::EndCombo();
		}

		if (editor.gocSprite == nullptr) {
			ImGui::End();
			return;
		}

		auto* project = editor.gocSprite->GetProject();
		if (project == nullptr) {
			ImGui::End();
			return;
		}

		const char* sceneName = editor.focusedScene ? editor.focusedScene->sceneData->name : "<none>";

		if (ImGui::BeginCombo("Scene", sceneName)) {
			for (auto* scene : project->scenes) {
				if (ImGui::Selectable(scene->sceneData->name, scene == editor.focusedScene))
					editor.focusedScene = scene;
				if (editor.focusedScene == scene)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		if (editor.focusedScene == nullptr) {
			ImGui::End();
			return;
		}

		const ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

		auto* selectionBehavior = editor.GetBehavior<SelectionBehavior<SurfRideSelection>>();
		auto& selected = selectionBehavior->GetSelection();

		for (auto* scene : project->scenes) {
			ImGuiTreeNodeFlags itemNodeFlags = nodeFlags;
			SurfRideSelection selection{ SurfRideSelection::Type::SCENE, scene };

			if (selected.find(selection) != -1)
				itemNodeFlags |= ImGuiTreeNodeFlags_Selected;

			bool isOpen = ImGui::TreeNode(scene, "%s", scene->sceneData->name);

			if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
				selectionBehavior->Select(selection);

			if (isOpen) {
				if (ImGui::TreeNodeEx("Cameras", nodeFlags)) {
					for (size_t i = 0; i < scene->sceneData->cameraCount; i++) {
						ImGuiTreeNodeFlags itemNodeFlags = nodeFlags;
						SurfRideSelection selection{ SurfRideSelection::Type::CAMERA_DATA, &scene->sceneData->cameras[i] };

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
				ImGui::TreePop();
			}
		}
	}
	ImGui::End();
}

void ProjectTree::RenderLayer(SurfRide::Layer* layer)
{
	ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
	SurfRideSelection selection{ SurfRideSelection::Type::LAYER, layer };

	auto* selectionBehavior = editor.GetBehavior<SelectionBehavior<SurfRideSelection>>();
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
	SurfRideSelection selection{ SurfRideSelection::Type::CAST, cast };
	SurfRide::Layer* refLayer{ static_cast<SurfRide::SRS_CASTNODE::Type>(cast->flags & 0xF) == SurfRide::SRS_CASTNODE::Type::REFERENCE ? static_cast<SurfRide::ReferenceCast*>(cast)->layer : nullptr };

	auto* selectionBehavior = editor.GetBehavior<SelectionBehavior<SurfRideSelection>>();
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
