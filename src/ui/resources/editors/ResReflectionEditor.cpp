#include "ResReflectionEditor.h"
#include <reflection/serialization/ReflectionSerializer.h>
#include <reflection/ReflectiveOperations.h>
#include <ui/common/editors/Reflection.h>
#include <ui/common/viewers/RflDiff.h>

using namespace hh::fnd;

ResReflectionEditor::ResReflectionEditor(csl::fnd::IAllocator* allocator, ResReflection* resource, const RflClass* rflClass) : StandaloneWindow{ allocator }, forcedRflClass{ true }, resource{ resource }, rflClass{ rflClass }, diffResult{ allocator } {
	char namebuf[500];
	snprintf(namebuf, sizeof(namebuf), "%s - %s @ 0x%zx (file mapped @ 0x%zx) - using RflClass %s", resource->GetName(), resource->GetClass().pName, (size_t)resource, (size_t)&resource->reflectionData, rflClass->GetName());
	SetTitle(namebuf);
}

ResReflectionEditor::ResReflectionEditor(csl::fnd::IAllocator* allocator, ResReflection* resource) : StandaloneWindow{ allocator }, forcedRflClass{ false }, resource{ resource }, rflClass{ nullptr }, diffResult{ allocator } {
	char namebuf[500];
	snprintf(namebuf, sizeof(namebuf), "%s - %s @ 0x%zx (file mapped @ 0x%zx)", resource->GetName(), resource->GetClass().pName, (size_t)resource, (size_t)&resource->reflectionData);
	SetTitle(namebuf);
}

ResReflectionEditor::~ResReflectionEditor()
{
	FreeOriginalCopy();
}

ResReflectionEditor* ResReflectionEditor::Create(csl::fnd::IAllocator* allocator, hh::fnd::ResReflection* resource, const hh::fnd::RflClass* rflClass) {
	return new (allocator) ResReflectionEditor(allocator, resource, rflClass);
}

void ResReflectionEditor::MakeOriginalCopy()
{
	origData = new (std::align_val_t(16), GetAllocator()) char[rflClass->GetSizeInBytes()];
	hh::fnd::RflTypeInfoRegistry::GetInstance()->ConstructObject(GetAllocator(), origData, rflClass->GetName());
	rflops::Copy::Apply(origData, resource->reflectionData, rflClass);
}

void ResReflectionEditor::FreeOriginalCopy()
{
	if (origData) {
		hh::fnd::RflTypeInfoRegistry::GetInstance()->CleanupLoadedObject(origData, rflClass->GetName());
		GetAllocator()->Free(origData);
	}
}

ResReflectionEditor* ResReflectionEditor::Create(csl::fnd::IAllocator* allocator, hh::fnd::ResReflection* resource) {
	return new (allocator) ResReflectionEditor(allocator, resource);
}

void ResReflectionEditor::RenderContents() {
	bool clicked_export = false;
	bool clicked_showdiff = false;

	if (ImGui::BeginMenuBar()) {
		ImGui::MenuItem("Export", nullptr, &clicked_export);
		ImGui::MenuItem("Show diff", nullptr, &clicked_showdiff);
		ImGui::EndMenuBar();
	}

	if (clicked_showdiff)
		showdiff = !showdiff;

	if (!forcedRflClass) {
		const char* previewValue = rflClass == nullptr ? "<none>" : rflClass->GetName();
		csl::ut::MoveArray<const RflClass*> matchingRflClasses{ hh::fnd::MemoryRouter::GetTempAllocator() };

		for (auto* rflc : RflClassNameRegistry::GetInstance()->GetItems()) {
			auto resSize = resource->GetSize();
			auto rflSize = rflc->GetSizeInBytes();

			if (resSize == rflSize || resSize == ((rflSize + 0xFF) & ~0xFF))
				matchingRflClasses.push_back(rflc);
		}

		if (ImGui::BeginCombo("RflClass", previewValue)) {
			if (matchingRflClasses.size() > 0) {
				for (auto* rflc : matchingRflClasses) {
					bool is_selected = rflClass == rflc;

					if (ImGui::Selectable(rflc->GetName(), is_selected)) {
						FreeOriginalCopy();
						rflClass = rflc;
						MakeOriginalCopy();
					}

					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
			}
			else {
				for (auto* rflc : RflClassNameRegistry::GetInstance()->GetItems()) {
					bool is_selected = rflClass == rflc;

					if (ImGui::Selectable(rflc->GetName(), is_selected)) {
						FreeOriginalCopy();
						rflClass = rflc;
						MakeOriginalCopy();
					}

					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
			}

			ImGui::EndCombo();
		}
	}

	if (rflClass != nullptr) {
		if (showdiff) {
			if (ImGui::BeginChild("Properties", ImVec2(ImGui::GetWindowSize().x / 2, 0))) {
				if (ResettableReflectionEditor("Properties", resource->reflectionData, origData, rflClass))
					diffResult = std::move(RflDiffStruct(GetAllocator(), origData, resource->reflectionData, rflClass));
			}
			ImGui::EndChild();
			ImGui::SameLine();
			if (ImGui::BeginChild("Diff", ImVec2(ImGui::GetWindowSize().x / 2, 0))) {
				if (ImGui::BeginTabBar("Diff Views")) {
					if (ImGui::BeginTabItem("Tree view")) {
						Viewer("Diff", diffResult, origData, rflClass);
						ImGui::EndTabItem();
					}
					//if (ImGui::BeginTabItem("RFL2HMM")) {
					//	Rfl2HmmViewer("Diff", diffResult, origData, rflClass);
					//	ImGui::EndTabItem();
					//}
					ImGui::EndTabBar();
				}
			}
			ImGui::EndChild();
		}
		else {
			if (ImGui::BeginChild("Properties")) {
				ResettableReflectionEditor("Properties", resource->reflectionData, origData, rflClass);
			}
			ImGui::EndChild();
		}

		if (clicked_export) {
			IGFD::FileDialogConfig cfg{};
			cfg.path = ".";
			cfg.flags = ImGuiFileDialogFlags_Modal | ImGuiFileDialogFlags_ConfirmOverwrite;
			cfg.userDatas = this;
			ImGuiFileDialog::Instance()->OpenDialog("ResReflectionExportDialog", "Choose File", ".rfl", cfg);
		}

		RenderExportDialog();
	}
}

void ResReflectionEditor::RenderExportDialog()
{
	if (ImGuiFileDialog::Instance()->Display("ResReflectionExportDialog", ImGuiWindowFlags_NoCollapse, ImVec2(800, 500))) {
		if (ImGuiFileDialog::Instance()->IsOk()) {
			auto* self = static_cast<ResReflectionEditor*>(ImGuiFileDialog::Instance()->GetUserDatas());

			std::string filePath = ImGuiFileDialog::Instance()->GetFilePathName();
			std::wstring wFilePath(filePath.begin(), filePath.end());

			ReflectionSerializer::SerializeToFile(wFilePath.c_str(), self->resource->reflectionData, *self->rflClass);
		}
		ImGuiFileDialog::Instance()->Close();
	}
}