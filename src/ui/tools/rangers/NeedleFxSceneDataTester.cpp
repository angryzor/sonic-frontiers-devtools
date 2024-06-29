#include "NeedleFxSceneDataTester.h"

using namespace hh::fnd;

NeedleFxSceneDataTester::NeedleFxSceneDataTester(csl::fnd::IAllocator* allocator) : StandaloneWindow(allocator)
{
	SetTitle("NeedleFxSceneData testing tool");

	auto* gameManager = hh::game::GameManager::GetInstance();

	if (auto* fxColMgr = gameManager->GetService<app::gfx::FxColManager>()) {
		fxColManager = fxColMgr;
		gameManager->UnregisterService(fxColMgr);
	}

	if (auto* fxParamMgr = gameManager->GetService<app::gfx::FxParamManager>()) {
		fxParamManager = fxParamMgr;
		gameManager->UnregisterService(fxParamMgr);
	}
}

NeedleFxSceneDataTester::~NeedleFxSceneDataTester() {
	auto* gameManager = hh::game::GameManager::GetInstance();

	if (fxParamManager != nullptr)
		gameManager->RegisterService(fxParamManager);
	if (fxColManager != nullptr)
		gameManager->RegisterService(fxColManager);
}

void NeedleFxSceneDataTester::PreRender() {
	ImGui::SetNextWindowSize(ImVec2(600, 300), ImGuiCond_Always);
}

void NeedleFxSceneDataTester::RenderContents()
{
	if (ImGui::BeginChild("DropRegion", ImVec2(600, 200))) {
		if (resource == nullptr) {
			ImGui::Text("Drag a NeedleFxSceneData RFL resource into this window to test it out.");
			ImGui::Text("Do not drag non-NeedleFxSceneData RFL resources into this window!");
			ImGui::Text("");
			ImGui::Text("The FxColManager and FxParamManager services are removed as long as this window is open.");
			ImGui::Text("Save your work! Changing levels now will crash your game due to a bug in this tool!");
		}
		else {
			ImGui::Text("Linked to resource \"%s\" (ResReflection @ 0x%zx)", resource->GetName(), resource);

			ImGui::Checkbox("Set Scene Config", &setConfig);
			ImGui::Checkbox("Set NeedleFX Parameter", &setParam);

			if (setParam) {
				ImGui::Indent();
				ImGui::SliderInt("Item ID", &itemId, 0, 15);
				ImGui::Unindent();
			}

			auto* renderingEngine = static_cast<hh::gfx::RenderManager*>(hh::gfx::RenderManager::GetInstance())->GetNeedleResourceDevice();
			if (setConfig)
				renderingEngine->SetSceneConfig(&resource->GetData()->config);
			if (setParam)
				renderingEngine->SetFXParameter(&resource->GetData()->items[itemId], 0);
		}
	}
	ImGui::EndChild();

	if (ImGui::BeginDragDropTarget()) {
		if (auto* payload = ImGui::AcceptDragDropPayload("Resource")) {
			auto* res = *static_cast<ManagedResource**>(payload->Data);
			const ResourceTypeInfo* typeInfo = &res->GetClass();

			if (typeInfo == ResReflection::GetTypeInfo())
				resource = static_cast<ResReflectionT<hh::needle::NeedleFxSceneData>*>(res);
		}
		ImGui::EndDragDropTarget();
	}
}
