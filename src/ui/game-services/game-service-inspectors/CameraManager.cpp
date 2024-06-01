#include "CameraManager.h"
#include <ui/common/editors/GraphicsContext.h>

using namespace hh::game;
using namespace hh::gfnd;

void RenderGameServiceInspector(hh::game::CameraManager& service)
{
	ImGui::SeparatorText("Components");
	for (auto* component : service.GetComponents()) {
		if (ImGui::TreeNode(component, component->name)) {
			ImGui::Text("Viewport ID: %d", component->viewportId);
			Editor("Viewport data", component->viewportData);
			ImGui::TreePop();
		}
	}

	ImGui::SeparatorText("Camera stacks");
	for (int i = 0; i < service.GetCameraCount(); i++) {
		ImGui::PushID(i);
		if (ImGui::TreeNode("Camera stack", "Camera stack %d", i)) {
			csl::ut::MoveArray<CameraComponent*> components{ hh::fnd::MemoryRouter::GetTempAllocator() };
			service.GetComponents(i, components);

			if (components.size() > 0) {
				ImGui::SeparatorText("Components");

				for (auto* component : components) {
					if (ImGui::TreeNode(component, component->name)) {
						ImGui::Text("Viewport ID: %d", component->viewportId);
						Editor("Viewport data", component->viewportData);
						ImGui::TreePop();
					}
				}
			}

			ImGui::TreePop();
		}
		ImGui::PopID();
	}
}
