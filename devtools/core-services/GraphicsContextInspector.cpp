#include "../Pch.h"
#include "GraphicsContextInspector.h"

GraphicsContextInspector::GraphicsContextInspector(csl::fnd::IAllocator* allocator) : StandaloneWindow{ allocator }
{
	SetTitle("GraphicsContext");
}

void GraphicsContextInspector::RenderContents()
{
	auto* gfxContext = *rangerssdk::bootstrap::GetAddress(&hh::gfnd::GraphicsContext::instance);

	/*if (ImGui::TreeNode("Renderable containers")) {
		int i = 0;
		for (auto& container : gfxContext->renderableContainers) {
			ImGui::TreeNode("Container %d", i++);

			auto* renderable = container->renderables.m_pRoot;

			while (renderable != nullptr) {
				renderable->
			}*/

	//ImGui::Text("layers active during ingame pause: %x", gameUpdater.layersActiveDuringIngamePause);
	//ImGui::Text("layers active during debug pause: %x", gameUpdater.layersActiveDuringDebugPause);
	//ImGui::Text("layers active during object pause: %x", gameUpdater.layersActiveDuringObjectPause);
}
