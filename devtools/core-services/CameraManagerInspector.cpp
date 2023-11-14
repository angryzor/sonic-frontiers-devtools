#include "../Pch.h"
#include "CameraManagerInspector.h"
#include "../common/SimpleWidgets.h"

using namespace hh::game;

CameraManagerInspector::CameraManagerInspector(csl::fnd::IAllocator* allocator) : StandaloneWindow{ allocator }
{
	SetTitle("CameraManager");
}

void CameraManagerInspector::RenderContents()
{
	auto* gameManager = GameManager::GetInstance();
	auto* cameraManager = gameManager->GetService<CameraManager>();

	if (ImGui::CollapsingHeader("Components")) {
		for (auto* component : cameraManager->GetComponents()) {
			if (ImGui::TreeNode(component, component->name)) {
				ImGui::Text("Viewport ID: %d", component->viewportId);
				ViewportDataInfo(component->viewportData);
				ImGui::TreePop();
			}
		}
	}

	if (ImGui::CollapsingHeader("Camera stacks")) {
		for (int i = 0; i < cameraManager->GetCameraCount(); i++) {
			ImGui::PushID(i);
			if (ImGui::TreeNode("Camera stack", "Camera stack %d", i)) {
				csl::ut::MoveArray<CameraComponent*> components{ hh::fnd::GetTempAllocator(hh::fnd::GetAllocatorSystem()) };
				cameraManager->GetComponents(i, components);

				if (components.size() > 0) {
					ImGui::SeparatorText("Components");

					for (auto* component : components) {
						if (ImGui::TreeNode(component, component->name)) {
							ImGui::Text("Viewport ID: %d", component->viewportId);
							ViewportDataInfo(component->viewportData);
							ImGui::TreePop();
						}
					}
				}

				ImGui::TreePop();
			}
			ImGui::PopID();
		}
	}
}
