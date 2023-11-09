//#include "Pch.h"
//#include "ResReflectionEditorWindow.h"
//#include "common/ReflectionEditor.h"
//#include "imgui/ImGuiFileDialog.h"
//
//using namespace hh::fnd;
//
//ResReflectionEditorWindow::ResReflectionEditorWindow(csl::fnd::IAllocator* allocator, ResReflection<void>* refl) : StandaloneWindow{ allocator } {}
//
//void ResReflectionEditorWindow::Render() {
//	const ImGuiWindowFlags windowFlags
//		= ImGuiWindowFlags_MenuBar;
//
//	char namebuf[500];
//	snprintf(namebuf, sizeof(namebuf), "%s - %s @ 0x%zx (file mapped @ 0x%zx)", resource->GetName(), resource->GetClass().pName, (size_t)&resource, (size_t)&resource->reflectionData);
//
//	bool clicked_export = false;
//
//	if (ImGui::Begin(namebuf, ImVec2(0, 800), true, windowFlags)) {
//		if (ImGui::BeginMenuBar()) {
//			ImGui::MenuItem("Reload from disk");
//			ImGui::MenuItem("Export", nullptr, &clicked_export);
//			ImGui::EndMenuBar();
//		}
//
//		static const char* none = "<none>";
//
//		if (ImGui::BeginCombo("Reflection class", rflClass == nullptr ? none : rflClass->GetName())) {
//			auto* values = (*rangerssdk::bootstrap::GetAddress(&RflClassNameRegistry::instance))->;
//
//			for (size_t i = 0; i < enumClass->GetValueCount(); i++) {
//				bool is_selected = currentValue == values[i].GetIndex();
//
//				if (ImGui::Selectable(values[i].GetEnglishName(), is_selected))
//					WritePrimitiveInt(obj, values[i].GetIndex(), member->GetSubType());
//
//				if (is_selected)
//					ImGui::SetItemDefaultFocus();
//			}
//
//			ImGui::EndCombo();
//		}
//
//		ReflectionEditor::Render(&resource.reflectionData, );
//	}
//	ImGui::End();
//
//	if (clicked_export) {
//		ImGuiFileDialog::Instance()->OpenDialog("ResReflectionExportDialog", "Choose File", ".rfl", ".", ImGuiFileDialogFlags_Modal | ImGuiFileDialogFlags_ConfirmOverwrite);
//	}
//
//	if (ImGuiFileDialog::Instance()->Display("ResReflectionExportDialog", ImGuiWindowFlags_NoCollapse, ImVec2(800, 500))) {
//		if (ImGuiFileDialog::Instance()->IsOk()) {
//			std::string filePath = ImGuiFileDialog::Instance()->GetFilePathName();
//			std::wstring wFilePath(filePath.begin(), filePath.end());
//
//			auto rflClass = rangerssdk::bootstrap::GetAddress(&T::rflClass);
//
//			ReflectionSerializer::SerializeToFile(wFilePath.c_str(), resource.reflectionData, *rflClass);
//		}
//		ImGuiFileDialog::Instance()->Close();
//	}
//}