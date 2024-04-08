#include "GraphicsContextInspector.h"
#include <ui/common/SimpleWidgets.h>

GraphicsContextInspector::GraphicsContextInspector(csl::fnd::IAllocator* allocator) : StandaloneWindow{ allocator }
{
	SetTitle("GraphicsContext");
}

void GraphicsContextInspector::RenderContents()
{
	auto* gfxContext = hh::gfnd::GraphicsContext::GetInstance();

	if (ImGui::TreeNode("Renderable containers")) {
		int i = 0;
		for (auto& container : gfxContext->renderableContainers) {
			if (ImGui::TreeNode(container, "Container %d", i++)) {
				for (auto& renderable : container->renderables) {
					ImGui::TreeNodeEx(&renderable, ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen, renderable.name == nullptr ? "<anonymous>" : renderable.name);
				}
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Viewports")) {
		int i = 0;

		if (ImGui::TreeNode("Default viewport")) {
			ViewportDataInfo(gfxContext->defaultViewportData);
			ImGui::TreePop();
		}

		for (auto& viewport : gfxContext->viewportDatas) {
			if (ImGui::TreeNode(&viewport, "Viewport %d", i++)) {
				ViewportDataInfo(viewport);
				ImGui::TreePop();
			}
		}

		ImGui::TreePop();
	}
}
