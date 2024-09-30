#include "PathManager.h"
#include <ui/common/viewers/Basic.h>

void RenderGameServiceInspector(hh::path::PathManager& service) {
	for (auto* pathComponent : service.pathComponents) {
		if (ImGui::TreeNode(pathComponent, "%s", pathComponent->GetName())) {
			Viewer("Name", pathComponent->GetName());
			Viewer("Owner object", pathComponent->GetOwnerGameObject());

			ImGui::TreePop();
		}
	}
}
