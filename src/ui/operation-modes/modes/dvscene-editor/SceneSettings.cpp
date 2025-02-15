#include "SceneSettings.h"
#include "Behaviors.h"
#include <ui/common/StandaloneOperationModeHost.h>
#include <ui/common/viewers/Basic.h>
#include <ui/common/editors/Basic.h>
#include <ui/common/inputs/Basic.h>
#include <ui/GlobalSettings.h>

namespace ui::operation_modes::modes::dvscene_editor {
	void SceneSettings::RenderPanel()
	{
		char sceneName[400];
		auto& context = GetContext();

		if (context.goDVSC != nullptr)
			snprintf(sceneName, sizeof(sceneName), "%s - %zx", context.goDVSC->cutsceneName.c_str(), reinterpret_cast<size_t>(&*context.goDVSC));
		else
			strcpy_s(sceneName, "none");

		if (ImGui::BeginCombo("DvSceneControl", sceneName)) {
			for (auto* obj : hh::game::GameManager::GetInstance()->objects)
				if (obj->objectClass == hh::dv::DvSceneControl::GetClass()) {
					auto* dvsc = reinterpret_cast<hh::dv::DvSceneControl*>(obj);
					snprintf(sceneName, sizeof(sceneName), "%s - %zx", dvsc->cutsceneName.c_str(), reinterpret_cast<size_t>(obj));
					if (ImGui::Selectable(sceneName, obj == context.goDVSC)) {
						GetBehavior<SelectionBehavior<Context>>()->DeselectAll();
						context.goDVSC = dvsc;
					}
					if (context.goDVSC == obj)
						ImGui::SetItemDefaultFocus();
				}
			ImGui::EndCombo();
		}

		if (context.goDVSC == nullptr) {
            ImGui::Text("No DvSceneControl selected");
			return;
		}

		/*if (ImGui::Button("Export")) {
			IGFD::FileDialogConfig cfg{};
			cfg.path = GlobalSettings::defaultFileDialogDirectory;
			cfg.flags = ImGuiFileDialogFlags_Modal | ImGuiFileDialogFlags_ConfirmOverwrite;
			cfg.userDatas = project->projectData;
			ImGuiFileDialog::Instance()->OpenDialog("ResSurfRideProjectExportDialog", "Choose File", ".swif", cfg);
		}*/

		if (ImGuiFileDialog::Instance()->Display("ResSurfRideProjectExportDialog", ImGuiWindowFlags_NoCollapse, ImVec2(800, 500))) {
			if (ImGuiFileDialog::Instance()->IsOk()) {
				//auto* exportProjectData = static_cast<ucsl::resources::swif::v6::SRS_PROJECT*>(ImGuiFileDialog::Instance()->GetUserDatas());

				//std::ofstream ofs{ ImGuiFileDialog::Instance()->GetFilePathName(), std::ios::trunc | std::ios::binary };
				//rip::binary::containers::swif::v6::SWIFSerializer serializer{ ofs };
				//serializer.serialize<he2sdk::ucsl::GameInterface>(*exportProjectData);
			}
			ImGuiFileDialog::Instance()->Close();
		}
        
		auto* timeline = context.goDVSC->timeline;
        ImGui::SeparatorText("DvCommon Settings");
        Editor("Start", timeline->frameStart);
        Editor("End", timeline->frameEnd);
		if(timeline->cuts.size() > 0){
			if(ImGui::TreeNode("Scene Cuts")){
				for(auto& x : timeline->cuts)
					DragScalar("", x);
				ImGui::TreePop();
			}
		}
		if(timeline->authPages.size() > 0){
			if(ImGui::TreeNode("Pages")){
				for(auto* x : timeline->authPages)
					if(ImGui::TreeNode(x->binaryData.pageName)){
						Editor("Page Name", x->binaryData.pageName);
						Editor("Start", x->binaryData.start);
						Editor("End", x->binaryData.end);
						Editor("Skip Frame", x->binaryData.skipFrame);
						Editor("Page Index", x->binaryData.pageIndex);
						ImGui::TreePop();
					}
				ImGui::TreePop();
			}
		}
	}

	PanelTraits SceneSettings::GetPanelTraits() const
	{
		return { "Scene Settings", ImVec2(ImGui::GetMainViewport()->WorkSize.x-250, ImGui::GetMainViewport()->WorkSize.y - 200), ImVec2(250, 200) };
	}
}
