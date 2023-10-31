#pragma once
#include "../common/ReflectionEditor.h"
#include "../serialization/ReflectionSerializer.h"
#include "../imgui/ImGuiFileDialog.h"

class ResReflectionEditor {
public:
	template<typename T>
	static void Render(hh::fnd::ResReflection<T>& resource) {
		const ImGuiWindowFlags windowFlags
			= ImGuiWindowFlags_MenuBar;

		char namebuf[500];
		snprintf(namebuf, sizeof(namebuf), "%s - %s @ 0x%zx (file mapped @ 0x%zx)", resource.GetName(), resource.GetClass().pName, (size_t)&resource, (size_t)&resource.reflectionData);

		bool clicked_export = false;

		ImGui::Indent();
		if (ImGui::CollapsingHeader(namebuf)) {
			if (ImGui::BeginChild(namebuf, ImVec2(0, 800), true, windowFlags)) {
				if (ImGui::BeginMenuBar()) {
					ImGui::MenuItem("Reload from disk");
					ImGui::MenuItem("Export", nullptr, &clicked_export);
					ImGui::EndMenuBar();
				}

				ReflectionEditor::Render(*resource.reflectionData);
			}
			ImGui::EndChild();
		}
		ImGui::Unindent();

		if (clicked_export) {
			ImGuiFileDialog::Instance()->OpenDialog("ResReflectionExportDialog", "Choose File", ".rfl", ".", ImGuiFileDialogFlags_Modal | ImGuiFileDialogFlags_ConfirmOverwrite);
		}

		if (ImGuiFileDialog::Instance()->Display("ResReflectionExportDialog", ImGuiWindowFlags_NoCollapse, ImVec2(800, 500))) {
			if (ImGuiFileDialog::Instance()->IsOk()) {
				std::string filePath = ImGuiFileDialog::Instance()->GetFilePathName();
				std::wstring wFilePath(filePath.begin(), filePath.end());

				auto rflClass = rangerssdk::bootstrap::GetAddress(&T::rflClass);

				ReflectionSerializer::SerializeToFile(wFilePath.c_str(), resource.reflectionData, *rflClass);
			}
			ImGuiFileDialog::Instance()->Close();
		}
	}
};
