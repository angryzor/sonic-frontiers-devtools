#include "TerrainManager.h"
#include <ui/common/viewers/Basic.h>

void RenderGameServiceInspector(app::trr::TerrainManager& service)
{
	Viewer("Current terrain ID", service.currentTerrain);
	Viewer("Terrain count", service.terrains.size());
	//ImGui::SeparatorText("Unk13s:");
	//if (service.unk13.)
	//for (auto i = service.unk13.begin(); i != service.unk13.end(); i++) {
	//	ImGui::Text("%s", i.key());
	//}
}
