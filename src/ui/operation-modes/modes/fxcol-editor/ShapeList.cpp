#include "ShapeList.h"
#include "Behaviors.h"
#include <ui/GlobalSettings.h>
#include <ucsl-reflection/reflections/resources/fxcol/v1.h>
#include <rip/binary/containers/binary-file/BinaryFile.h>

namespace ui::operation_modes::modes::fxcol_editor {
	using namespace app::gfx;

	void ShapeList::RenderPanel() {
		auto* fxColManager = hh::game::GameManager::GetInstance()->GetService<FxColManager>();

		if (!fxColManager) {
			ImGui::Text("FxColManager service not active.");
			return;
		}

		if (!fxColManager->resource) {
			ImGui::Text("No resource loaded.");
			return;
		}

		auto* fxColData = fxColManager->resource->fxColData;

		if (ImGui::Button("Export")) {
			IGFD::FileDialogConfig cfg{};
			cfg.path = GlobalSettings::defaultFileDialogDirectory;
			cfg.flags = ImGuiFileDialogFlags_Modal | ImGuiFileDialogFlags_ConfirmOverwrite;
			cfg.userDatas = fxColData;
			ImGuiFileDialog::Instance()->OpenDialog("ResFxColFile2ExportDialog", "Choose File", ".fxcol", cfg);
		}

		if (ImGuiFileDialog::Instance()->Display("ResFxColFile2ExportDialog", ImGuiWindowFlags_NoCollapse, ImVec2(800, 500))) {
			if (ImGuiFileDialog::Instance()->IsOk()) {
				auto* exportData = static_cast<ucsl::resources::fxcol::v1::FxColData*>(ImGuiFileDialog::Instance()->GetUserDatas());

				std::ofstream ofs{ ImGuiFileDialog::Instance()->GetFilePathName(), std::ios::trunc | std::ios::binary };
				rip::binary::binary_ostream bofs{ ofs };
				rip::binary::containers::binary_file::v2::BinaryFileSerializer serializer{ bofs };
				serializer.serialize<he2sdk::ucsl::GameInterface>(*exportData);
			}
			ImGuiFileDialog::Instance()->Close();
		}

		auto& context = GetContext();
		const char* targetBoundingVolumePreview = "<none>";
		char targetBoundingVolumeName[100];

		for (size_t i = 0; i < fxColData->boundingVolumeCount; i++) {
			auto& boundingVolume = fxColData->boundingVolumes[i];

			if (context.placementVolume == &boundingVolume) {
				snprintf(targetBoundingVolumeName, sizeof(targetBoundingVolumeName), "Bounding Volume %zd", i);

				targetBoundingVolumePreview = targetBoundingVolumeName;
			}
		}

		if (ImGui::BeginCombo("Target placement volume", targetBoundingVolumePreview)) {
			for (size_t i = 0; i < fxColData->boundingVolumeCount; i++) {
				auto& boundingVolume = fxColData->boundingVolumes[i];
				char name[100];
				snprintf(name, sizeof(name), "Bounding Volume %zd", i);

				if (ImGui::Selectable(name, context.placementVolume == &boundingVolume))
					context.placementVolume = &boundingVolume;
				if (context.placementVolume == &boundingVolume)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		if (ImGui::BeginPopupContextItem()) {
			if (ImGui::MenuItem("Create bounding volume"))
				GetContext().AddBoundingVolume();
			ImGui::EndPopup();
		}

		for (size_t i = 0; i < fxColData->boundingVolumeCount; i++) {
			auto& boundingVolume = fxColData->boundingVolumes[i];
			char name[100];
			snprintf(name, sizeof(name), "Bounding Volume %zd", i);

			ImGui::PushID(&boundingVolume);

			bool isOpen = ImGui::TreeNode(&boundingVolume, name);

			if (ImGui::BeginPopupContextItem()) {
				if (ImGui::MenuItem("Remove bounding volume"))
					GetContext().RemoveBoundingVolume(&boundingVolume);
				ImGui::EndPopup();
			}

			if (isOpen) {
				for (size_t j = 0; j < boundingVolume.shapeCount; j++)
					RenderShapeItem(fxColData->collisionShapes[boundingVolume.shapeStartIdx + j]);

				ImGui::TreePop();
			}

			ImGui::PopID();
		}

		for (size_t i = 0; i < fxColData->collisionShapeCount; i++) {
			bool found{};

			for (size_t j = 0; j < fxColData->boundingVolumeCount; j++) {
				if (i >= fxColData->boundingVolumes[j].shapeStartIdx && i < fxColData->boundingVolumes[j].shapeStartIdx + fxColData->boundingVolumes[j].shapeCount) {
					found = true;
					break;
				}
			}

			if (found)
				break;

			RenderShapeItem(fxColData->collisionShapes[i]);
		}
	}

	PanelTraits ShapeList::GetPanelTraits() const {
		return PanelTraits{ "Shape List", ImVec2(0, 0), ImVec2(250, ImGui::GetMainViewport()->WorkSize.y - 100) };
	}

	void ShapeList::RenderShapeItem(FxColCollisionShapeData& shape)
	{
		auto* selectionBehavior = GetBehavior<SelectionBehavior<Context>>();

		ImGui::PushID(&shape);
		if (ImGui::Selectable(shape.name, selectionBehavior->GetSelection().find(&shape) != -1))
			selectionBehavior->Select(&shape);
		ImGui::PopID();
	}
}
