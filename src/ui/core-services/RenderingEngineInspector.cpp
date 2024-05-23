#include "RenderingEngineInspector.h"
#include <ui/common/editors/Basic.h>
#include <ui/common/editors/Needle.h>
#include <ui/common/viewers/Basic.h>

using namespace hh::gfx;

RenderingEngineInspector::RenderingEngineInspector(csl::fnd::IAllocator* allocator) : StandaloneWindow{ allocator }
{
	SetTitle("RenderingEngine");
}

void RenderingEngineInspector::RenderContents()
{
	auto* renderingEngine = static_cast<RenderingEngineRangers*>(static_cast<RenderManager*>(RenderManager::GetInstance())->GetNeedleResourceDevice());

	Editor("Weird toggle", *reinterpret_cast<uint32_t*>(0x143CFB388));

	ImGui::SeparatorText("Main rendering pipeline");
	Editor("Main rendering pipeline", *renderingEngine->mainRenderingPipeline);

	ImGui::SeparatorText("Main render unit");
	Editor("Main render unit", *renderingEngine->mainRenderUnit);

	ImGui::SeparatorText("SupportFX render units");
	for (auto& renderUnit : renderingEngine->supportFX.renderUnits) {
		if (ImGui::TreeNode(renderUnit.nameId, "%s", renderUnit.nameId->name)) {
			Editor("Pipeline", *static_cast<hh::needle::RenderTexturePipeline*>(renderUnit.pipeline));
			Editor("RenderUnit", renderUnit);
			ImGui::TreePop();
		}
	}
}
