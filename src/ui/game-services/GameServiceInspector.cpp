#include "GameServiceInspector.h"
#include <imgui_internal.h>
#include <ui/Desktop.h>
//#include <ui/resources/editors/ResObjectWorldEditor.h>
//#include <ui/common/viewers/Basic.h>
//#include <ui/common/editors/Basic.h>
//#include <ui/common/editors/ObjectData.h>
//#include <ui/common/editors/LevelInfo.h>
//#include <ui/common/editors/GraphicsContext.h>
//#include <ui/common/editors/Reflection.h>
//#include <ui/common/inputs/Basic.h>

#include "game-service-inspectors/FxParamManager.h"

using namespace hh::game;

GameServiceInspector::GameServiceInspector(csl::fnd::IAllocator* allocator) : StandaloneWindow{ allocator } {
	SetTitle("GameService inspector");
}

void GameServiceInspector::RenderContents() {
	auto* gameManager = GameManager::GetInstance();

	for (auto* service : gameManager->GetServices()) {
		ImGui::PushID(service);
		if (ImGui::CollapsingHeader(service->pStaticClass->pName))
			RenderServiceInspector(*service);
		ImGui::PopID();
	}
}

void GameServiceInspector::RenderServiceInspector(hh::game::GameService& service) {
	if (service.pStaticClass == app::gfx::FxParamManager::GetClass()) {
		RenderGameServiceInspector(static_cast<app::gfx::FxParamManager&>(service));
	}
	// else if (service.pStaticClass == hh::game::ObjectWorld::GetClass()) {
	// 	RenderObjectWorldInspector(static_cast<hh::game::ObjectWorld&>(service));
	// }
	// else if (service.pStaticClass == app::level::StageInfo::GetClass()) {
	// 	RenderStageInfoInspector(static_cast<app::level::StageInfo&>(service));
	// }
	// else if (service.pStaticClass == app::level::LevelInfo::GetClass()) {
	// 	RenderLevelInfoInspector(static_cast<app::level::LevelInfo&>(service));
	// }
	// else if (service.pStaticClass == app::game::GameModeResourceManager::GetClass()) {
	// 	RenderGameModeResourceManagerInspector(static_cast<app::game::GameModeResourceManager&>(service));
	// }
	// else if (service.pStaticClass == app::trr::TerrainManager::GetClass()) {
	// 	RenderTerrainManagerInspector(static_cast<app::trr::TerrainManager&>(service));
	// }
	// else if (service.pStaticClass == hh::game::CameraManager::GetClass()) {
	// 	RenderCameraManagerInspector(static_cast<hh::game::CameraManager&>(service));
	// }
	// else if (service.pStaticClass == app::camera::CameraService::GetClass()) {
	// 	RenderCameraServiceInspector(static_cast<app::camera::CameraService&>(service));
	// }
	else {
		ImGui::Text("Inspector for this service not yet implemented");
	}
}

// void GameServiceInspector::RenderObjectWorldInspector(hh::game::ObjectWorld& objWorld) {
// 	int i = 0;
// 	for (auto* chunk : objWorld.GetWorldChunks()) {
// 		if (ImGui::TreeNode(chunk, "Chunk %d", i++)) {
// 			bool editorMode = chunk->IsStatusEditor();

// 			if (ImGui::Checkbox("Editor mode", &editorMode))
// 				chunk->SetEditorStatus(editorMode);

// 			if (ImGui::CollapsingHeader("Layers")) {
// 				for (auto* layer : chunk->GetLayers()) {
// 					if (ImGui::TreeNode(layer, layer->GetName())) {
// 						bool enabled = layer->IsEnable();

// 						if (ImGui::Checkbox("Enabled", &enabled))
// 							chunk->SetLayerEnabled(layer->GetName(), enabled);

// 						if (ImGui::Button("Edit resource"))
// 							ResObjectWorldEditor::Create(Desktop::instance->GetAllocator(), layer->GetResource());

// 						ImGui::TreePop();
// 					}
// 				}
// 			}

// 			if (ImGui::CollapsingHeader("Object statuses")) {
// 				for (auto& status : chunk->GetObjectStatuses()) {
// 					if (ImGui::TreeNode(&status, status.objectData->name)) {
// 						CheckboxFlags("Enabled", status.flags, WorldObjectStatus::Flag::ENABLED);
// 						CheckboxFlags("Is alive", status.flags, WorldObjectStatus::Flag::IS_ALIVE);
// 						CheckboxFlags("Started", status.flags, WorldObjectStatus::Flag::STARTED);
// 						CheckboxFlags("No restart", status.flags, WorldObjectStatus::Flag::NO_RESTART);

// 						Editor("Spawn priority", status.spawnPriority);

// 						ImGui::SeparatorText("Object Data");
// 						Editor("Object data", *status.objectData);

// 						ImGui::TreePop();
// 					}
// 				}
// 			}
// 			ImGui::TreePop();
// 		}
// 	}
// }

// void GameServiceInspector::RenderStageInfoInspector(app::level::StageInfo& service)
// {
// 	ImGui::SeparatorText("Stages");
// 	for (auto& stage : service.stages) {
// 		if (ImGui::TreeNodeEx(stage, ImGuiTreeNodeFlags_None, "%s", stage->name.c_str())) {
// 			Editor("Stage data", *stage);
// 			ImGui::TreePop();
// 		}
// 	}
// }

// void GameServiceInspector::RenderLevelInfoInspector(app::level::LevelInfo& service)
// {
// 	auto* stageData = service.GetStageData();
// 	ImGui::SeparatorText("Stage data");
// 	if (stageData)
// 		Editor("Stage data", *stageData);
// 	else
// 		ImGui::Text("No stage data loaded.");

// 	if (ImGui::TreeNode("PlayerInformation")) {
// 		for (size_t i = 0; i < 2; i++) {
// 			auto* playerInfo = service.GetPlayerInformation(i);
// 			char nodeName[50];
// 			snprintf(nodeName, sizeof(nodeName), "Player %zd", i);

// 			if (ImGui::TreeNode(nodeName)) {
// 				Editor(nodeName, *playerInfo);
// 				ImGui::TreePop();
// 			}
// 		}
// 		ImGui::TreePop();
// 	}
// }

// void GameServiceInspector::RenderGameModeResourceManagerInspector(app::game::GameModeResourceManager& service)
// {
// 	for (auto& module : service.modules) {
// 		if (ImGui::TreeNodeEx(module, ImGuiTreeNodeFlags_DefaultOpen, "%x", module->GetNameHash())) {
// 			ImGui::Text("Module flags: %x", module->flags);
// 			if (ImGui::TreeNode("Resources")) {
// 				for (auto& resource : module->resourceCollection->resources) {
// 					ImGui::Text("%s", resource.name);
// 				}
// 				ImGui::TreePop();
// 			}
// 			ImGui::TreePop();
// 		}
// 	}
// }

// void GameServiceInspector::RenderTerrainManagerInspector(app::trr::TerrainManager& service)
// {
// 	Viewer("Unk11", service.currentTerrain);
// 	Viewer("Unk10 count", service.unk10.size());
// 	//ImGui::SeparatorText("Unk13s:");
// 	//if (service.unk13.)
// 	//for (auto i = service.unk13.begin(); i != service.unk13.end(); i++) {
// 	//	ImGui::Text("%s", i.key());
// 	//}
// }

// void GameServiceInspector::RenderCameraManagerInspector(hh::game::CameraManager& service)
// {
// 	ImGui::SeparatorText("Components");
// 	for (auto* component : service.GetComponents()) {
// 		if (ImGui::TreeNode(component, component->name)) {
// 			ImGui::Text("Viewport ID: %d", component->viewportId);
// 			Editor("Viewport data", component->viewportData);
// 			ImGui::TreePop();
// 		}
// 	}

// 	ImGui::SeparatorText("Camera stacks");
// 	for (int i = 0; i < service.GetCameraCount(); i++) {
// 		ImGui::PushID(i);
// 		if (ImGui::TreeNode("Camera stack", "Camera stack %d", i)) {
// 			csl::ut::MoveArray<CameraComponent*> components{ hh::fnd::MemoryRouter::GetTempAllocator() };
// 			service.GetComponents(i, components);

// 			if (components.size() > 0) {
// 				ImGui::SeparatorText("Components");

// 				for (auto* component : components) {
// 					if (ImGui::TreeNode(component, component->name)) {
// 						ImGui::Text("Viewport ID: %d", component->viewportId);
// 						Editor("Viewport data", component->viewportData);
// 						ImGui::TreePop();
// 					}
// 				}
// 			}

// 			ImGui::TreePop();
// 		}
// 		ImGui::PopID();
// 	}
// }

// void GameServiceInspector::RenderCameraServiceInspector(app::camera::CameraService& service)
// {
// 	for (auto& bridgeUnit : service.cameraBridges) {
// 		if (bridgeUnit.cameraBridge.cameraFrameId != -1 && bridgeUnit.cameraComponent != nullptr && ImGui::TreeNodeEx(&bridgeUnit, ImGuiTreeNodeFlags_None, "%d", bridgeUnit.cameraBridge.cameraFrameId)) {
// 			ImGui::Text("Camera frame ID: %d - %s", bridgeUnit.cameraBridge.cameraFrameId, bridgeUnit.cameraBridge.cameraFrame->name.c_str());
// 			Editor("CameraFrame", bridgeUnit.cameraBridge.cameraFrame);
// 			if (ImGui::TreeNodeEx("Component", ImGuiTreeNodeFlags_None, "%s", bridgeUnit.cameraComponent->name.c_str())) {
// 				ImGui::Text("Viewport ID: %d", bridgeUnit.cameraComponent->viewportId);
// 				Editor("Viewport data", bridgeUnit.cameraComponent->viewportData);
// 				ImGui::TreePop();
// 			}
// 			ImGui::TreePop();
// 		}
// 	}
// }
