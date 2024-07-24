#include <ui/game-services/game-service-inspectors/FxColManager.h>
#include <ui/common/viewers/Basic.h>
#include <ui/common/editors/Resources.h>
#include <ui/common/editors/FxCol.h>

void RenderGameServiceInspector(app::gfx::FxColManager& service) {
	Viewer("Stage name", service.stageName);
#ifdef DEVTOOLS_TARGET_SDK_wars
	if (service.terrainPackfile != nullptr)
		Viewer("Terrain packfile name", service.terrainPackfile->GetName());
#endif
#ifdef DEVTOOLS_TARGET_SDK_rangers
	Viewer("Terrain packfile name", service.terrainPackfileName);
#endif
	if (service.resource == nullptr)
		ImGui::Text("No resource loaded.");
	else {
		Editor("Resource", *service.resource);

		ImGui::SeparatorText("Active collision shapes");
		for (auto shapeIdx : service.activeCollisionShapes) {
			ImGui::PushID(shapeIdx);
			char desc[20];
			snprintf(desc, sizeof(desc), "Collision shape %d", shapeIdx);
			Editor(desc, service.resource->fxColData->collisionShapes[shapeIdx]);
			ImGui::PopID();
		}
	}
}
