#include "GraphicsContextInspector.h"
#include <ui/common/editors/GraphicsContext.h>

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
			if (ImGui::TreeNode(container, "Render pass %d", i++)) {
				for (auto& renderable : container->renderables) {
					ImGui::TreeNodeEx(&renderable, ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen, "%s - priority %d", renderable.name == nullptr ? "<anonymous>" : renderable.name, renderable.priority);
				}
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Viewports")) {
		int i = 0;

		if (ImGui::TreeNode("Default viewport")) {
			Editor("Viewport data", gfxContext->defaultViewportData);
			ImGui::TreePop();
		}

		for (auto& viewport : gfxContext->viewportDatas) {
			if (ImGui::TreeNode(&viewport, "Viewport %d", i++)) {
				Editor("Viewport data", viewport);
				ImGui::TreePop();
			}
		}

		ImGui::TreePop();
	}
}
