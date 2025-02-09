#include "ShapeList.h"
#include "Behaviors.h"
#include <ui/GlobalSettings.h>
#include <ucsl-reflection/reflections/resources/svcol/v1.h>
#include <rip/binary/containers/binary-file/v2.h>
#include <span>

namespace ui::operation_modes::modes::svcol_editor {
	using namespace ucsl::resources::svcol::v1;

	void ShapeList::RenderPanel() {
		auto& context = GetContext();
		
		if (ImGui::BeginCombo("Resources", context.resource == nullptr ? "<none>" : context.resource->GetName())) {
			for (auto* resource : hh::fnd::ResourceManager::GetInstance()->GetResourcesByTypeInfo(app::ResSvCol2::GetTypeInfo())) {
				auto* svColResource = (app::ResSvCol2*)resource;
				if (ImGui::Selectable(resource->GetName(), context.resource == svColResource)) {
					GetBehavior<SelectionBehavior<Context>>()->DeselectAll();
					context.resource = svColResource;
				}
				if (context.resource == svColResource)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		if (context.resource == nullptr) {
			ImGui::Text("No resource loaded.");
			return;
		}

		auto& svColData = *context.resource->svColData;

		if (ImGui::Button("Export")) {
			IGFD::FileDialogConfig cfg{};
			cfg.path = GlobalSettings::defaultFileDialogDirectory;
			cfg.flags = ImGuiFileDialogFlags_Modal | ImGuiFileDialogFlags_ConfirmOverwrite;
			cfg.userDatas = &svColData;
			ImGuiFileDialog::Instance()->OpenDialog("ResSvColFile2ExportDialog", "Choose File", ".svcol", cfg);
		}

		if (ImGuiFileDialog::Instance()->Display("ResSvColFile2ExportDialog", ImGuiWindowFlags_NoCollapse, ImVec2(800, 500))) {
			if (ImGuiFileDialog::Instance()->IsOk()) {
				auto* exportData = static_cast<ucsl::resources::svcol::v1::SvColData*>(ImGuiFileDialog::Instance()->GetUserDatas());

				std::ofstream ofs{ ImGuiFileDialog::Instance()->GetFilePathName(), std::ios::trunc | std::ios::binary };
				rip::binary::containers::binary_file::v2::BinaryFileSerializer<size_t> serializer{ ofs };
				serializer.serialize<he2sdk::ucsl::GameInterface>(*exportData);
			}
			ImGuiFileDialog::Instance()->Close();
		}

		for (auto& shape : std::span(svColData.shapes, svColData.shapeCount))
			RenderShapeItem(shape);
	}

	PanelTraits ShapeList::GetPanelTraits() const {
		return PanelTraits{ "Shape List", ImVec2(0, 0), ImVec2(250, ImGui::GetMainViewport()->WorkSize.y - 100) };
	}

	void ShapeList::RenderShapeItem(ShapeData& shape)
	{
		auto* selectionBehavior = GetBehavior<SelectionBehavior<Context>>();

		ImGui::PushID(&shape);
		if (ImGui::Selectable(shape.name, selectionBehavior->GetSelection().find(&shape) != -1))
			selectionBehavior->Select(&shape);
		ImGui::PopID();
	}
}
