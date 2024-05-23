#include "RenderManagerInspector.h"
#include <ui/common/editors/Needle.h>
#include <ui/common/editors/Basic.h>
#include <ui/common/viewers/Basic.h>

using namespace hh::gfx;

RenderManagerInspector::RenderManagerInspector(csl::fnd::IAllocator* allocator) : StandaloneWindow{ allocator }
{
	SetTitle("RenderManager");
}

void RenderManagerInspector::RenderContents()
{
	if (auto* renderMgr = static_cast<RenderManager*>(RenderManager::GetInstance())) {
		Editor("Viewports", renderMgr->implementation->viewports);
		Viewer("Cameras", renderMgr->implementation->fxCameras);
		if (ImGui::TreeNode("Render parameters")) {
			Editor("Render parameters", renderMgr->implementation->renderParam);
			ImGui::TreePop();
		}
		Editor("unk2", renderMgr->implementation->unk2);
		Editor("numMainViewports", renderMgr->implementation->numMainViewports);
		Editor("unk3", renderMgr->implementation->unk3);
		Editor("unk4", renderMgr->implementation->unk4);
		Editor("unk5", renderMgr->implementation->unk5);
		Editor("unk6", renderMgr->implementation->unk6);
		Editor("maybeSupportFXWidth", renderMgr->implementation->maybeSupportFXWidth);
		Editor("maybeSupportFXHeight", renderMgr->implementation->maybeSupportFXHeight);
		Editor("unk12", renderMgr->implementation->unk12);
		Editor("unk13", renderMgr->implementation->unk13);
		Editor("unk14", renderMgr->implementation->unk14);
		Editor("unk15", renderMgr->implementation->unk15);
		Editor("width", renderMgr->implementation->width);
		Editor("height", renderMgr->implementation->height);
		Editor("maybeRenderWidth", renderMgr->implementation->maybeRenderWidth);
		Editor("maybeRenderHeight", renderMgr->implementation->maybeRenderHeight);
		Editor("unk18", renderMgr->implementation->unk18);
		Editor("unk19", renderMgr->implementation->unk19);
		Editor("unk20", renderMgr->implementation->unk20);
		Editor("unk21", renderMgr->implementation->unk21);
		Editor("unk22", renderMgr->implementation->unk22);
		Editor("unk25", renderMgr->implementation->unk25);

		Editor("unk29suppfx", renderMgr->implementation->renderingEngine->GetSupportFX()->unk29);
	}
}
