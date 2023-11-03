#include "Pch.h"
#include "GameServiceInspector.h"
#include "common/ReflectionEditor.h"

using namespace hh::game;

namespace app::gfx {
	class FxParamManager {
	public:
		static GameServiceClass* GetClass();
	};
}

void GameServiceInspector::Render() {
	auto* gameManager = GameManager::GetInstance();

	const ImGuiWindowFlags windowFlags
		= ImGuiWindowFlags_MenuBar;

	ImGui::SetNextWindowPos(ImVec2(ImGui::GetMainViewport()->WorkSize.x, ImGui::GetMainViewport()->WorkSize.y - 30), ImGuiCond_Once, ImVec2(1, 0));
	ImGui::SetNextWindowSize(ImVec2(800, ImGui::GetMainViewport()->WorkSize.y - 100), ImGuiCond_Once);
	ImGui::SetNextWindowCollapsed(true, ImGuiCond_Once);
	if (ImGui::Begin("GameService inspector", NULL, windowFlags)) {
		for (auto* service : gameManager->GetServices()) {
			if (ImGui::CollapsingHeader(service->pStaticClass->pName)) {
				RenderServiceInspector(*service);
			}
		}
	}
	ImGui::End();
}

void GameServiceInspector::RenderServiceInspector(hh::game::GameService& service) {
	if (service.pStaticClass == app::gfx::FxParamManager::GetClass()) {
		RenderFxParamManagerInspector(service);
	}
	else {
		RenderUnknownServiceInspector(service);
	}
}

namespace app::rfl {
	struct NeedleFxParameter {
		static const hh::fnd::RflClass rflClass;
	};
}

void GameServiceInspector::RenderFxParamManagerInspector(hh::game::GameService& service) {
	ImGui::SeparatorText("NeedleFXParameter");
	ReflectionEditor::Render(*reinterpret_cast<app::rfl::NeedleFxParameter*>(reinterpret_cast<size_t>(&service) + 128));
	ImGui::SeparatorText("FxLODParameter");
	ReflectionEditor::Render(*reinterpret_cast<app::rfl::FxLODParameter*>(reinterpret_cast<size_t>(&service) + 4872));
	ImGui::SeparatorText("StageCommonTimeProgressParameter");
	ReflectionEditor::Render(*reinterpret_cast<app::rfl::StageCommonTimeProgressParameter*>(reinterpret_cast<size_t>(&service) + 5052));
}

void GameServiceInspector::RenderUnknownServiceInspector(hh::game::GameService& service) {
	ImGui::Text("Inspector for this service not yet implemented");
}
