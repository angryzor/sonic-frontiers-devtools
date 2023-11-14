#include "../Pch.h"
#include "ResReflectionEditor.h"

using namespace hh::fnd;

ResReflectionEditor::ResReflectionEditor(csl::fnd::IAllocator* allocator, ResReflection<void>* resource, const RflClass* rflClass) : StandaloneWindow{ allocator }, forcedRflClass{ true }, resource{ resource }, rflClass { rflClass } {
	char namebuf[500];
	snprintf(namebuf, sizeof(namebuf), "%s - %s @ 0x%zx (file mapped @ 0x%zx) - using RflClass %s", resource->GetName(), resource->GetClass().pName, (size_t)resource, (size_t)&resource->reflectionData, rflClass->GetName());
	SetTitle(namebuf);
}

ResReflectionEditor::ResReflectionEditor(csl::fnd::IAllocator* allocator, ResReflection<void>* resource) : StandaloneWindow{ allocator }, forcedRflClass{ false }, resource{ resource }, rflClass{ nullptr } {
	char namebuf[500];
	snprintf(namebuf, sizeof(namebuf), "%s - %s @ 0x%zx (file mapped @ 0x%zx)", resource->GetName(), resource->GetClass().pName, (size_t)resource, (size_t)&resource->reflectionData);
	SetTitle(namebuf);
}

ResReflectionEditor* ResReflectionEditor::Create(csl::fnd::IAllocator* allocator, hh::fnd::ResReflection<void>* resource, const hh::fnd::RflClass* rflClass) {
	return new (allocator) ResReflectionEditor(allocator, resource, rflClass);
}

ResReflectionEditor* ResReflectionEditor::Create(csl::fnd::IAllocator* allocator, hh::fnd::ResReflection<void>* resource) {
	return new (allocator) ResReflectionEditor(allocator, resource);
}

void ResReflectionEditor::RenderContents() {
	bool clicked_export = false;

	if (ImGui::BeginMenuBar()) {
		ImGui::MenuItem("Reload from disk");
		ImGui::MenuItem("Export", nullptr, &clicked_export);
		ImGui::EndMenuBar();
	}

	if (!forcedRflClass) {
		const char* previewValue = rflClass == nullptr ? "<none>" : rflClass->GetName();

		if (ImGui::BeginCombo("RflClass", previewValue)) {
			for (auto* rflc : RflClassNameRegistry::GetInstance()->GetItems()) {
				if (resource->GetSize() == rflc->GetSizeInBytes()) {
					bool is_selected = rflClass == rflc;

					if (ImGui::Selectable(rflc->GetName(), is_selected))
						rflClass = rflc;

					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
			}

			ImGui::EndCombo();
		}
	}

	if (rflClass != nullptr) {
		ReflectionEditor::Render(resource->reflectionData, rflClass);

		if (clicked_export) {
			ImGuiFileDialog::Instance()->OpenDialog("ResReflectionExportDialog", "Choose File", ".rfl", ".", ImGuiFileDialogFlags_Modal | ImGuiFileDialogFlags_ConfirmOverwrite);
		}

		if (ImGuiFileDialog::Instance()->Display("ResReflectionExportDialog", ImGuiWindowFlags_NoCollapse, ImVec2(800, 500))) {
			if (ImGuiFileDialog::Instance()->IsOk()) {
				std::string filePath = ImGuiFileDialog::Instance()->GetFilePathName();
				std::wstring wFilePath(filePath.begin(), filePath.end());

				ReflectionSerializer::SerializeToFile(wFilePath.c_str(), resource->reflectionData, *rflClass);
			}
			ImGuiFileDialog::Instance()->Close();
		}
	}
}
