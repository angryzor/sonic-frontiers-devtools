#include "ObjectDataInspector.h"
#include "LevelEditor.h"
#include <ui/common/editors/Basic.h>
#include <ui/common/editors/ObjectData.h>
#include <utilities/ObjectDataUtils.h>
#include <imgui_internal.h>

using namespace hh::fnd;
using namespace hh::game;

ObjectDataInspector::ObjectDataInspector(csl::fnd::IAllocator* allocator, LevelEditor& levelEditor) : CompatibleObject{ allocator }, levelEditor{ levelEditor }
{
}

void ObjectDataInspector::Render() {
	const ImGuiWindowFlags windowFlags
		= 0;

	ImGui::SetNextWindowPos(ImVec2(ImGui::GetMainViewport()->WorkSize.x, 100), ImGuiCond_FirstUseEver, ImVec2(1, 0));
	ImGui::SetNextWindowSize(ImVec2(800, ImGui::GetMainViewport()->WorkSize.y - 140), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Object data inspector", NULL, windowFlags)) {
		if (levelEditor.focusedObjects.size() == 0) {
			ImGui::Text("Select an object in the left pane.");
		}
		else if (levelEditor.focusedObjects.size() > 1) {
			ImGui::Text("%zd objects selected", levelEditor.focusedObjects.size());
			ImGui::SeparatorText("Align / Distribute");
			ImGui::PushID("Align / Distribute");
			Editor("##AlignX", alignX);
			ImGui::SameLine();
			Editor("SpacingX", distributeSpacing.x());
			Editor("##AlignY", alignY);
			ImGui::SameLine();
			Editor("SpacingY", distributeSpacing.y());
			Editor("##AlignZ", alignZ);
			ImGui::SameLine();
			Editor("SpacingZ", distributeSpacing.z());
			if (ImGui::Button("Align / Distribute"))
				Distribute();
			ImGui::PopID();
		}
		else {
			auto focusedObject = levelEditor.focusedObjects[0];
			bool edited = Editor("Focused object", *focusedObject);

			if (edited || ImGui::IsItemDeactivatedAfterEdit()) {
				levelEditor.RecalculateDependentTransforms();

				hh::dbg::MsgParamChangedInEditor msg{};

				levelEditor.NotifyActiveObject(msg);
				// The only reason I do this is because apparently sending MsgParamChanged sets the debug visual visibility to false on volumes??
				levelEditor.NotifySelectedObject();
				levelEditor.NotifyUpdatedObject();

				hh::dbg::MsgUpdateSetEditor msg3{};
			}

			if (ImGui::IsItemDeactivatedAfterEdit()) {
				auto status = levelEditor.focusedChunk->GetWorldObjectStatusByObjectId(focusedObject->id);
				auto idx = levelEditor.focusedChunk->GetObjectIndexById(focusedObject->id);

				if (status.objectData && idx != -1) {
					levelEditor.focusedChunk->DespawnByIndex(idx);
					levelEditor.focusedChunk->ShutdownPendingObjects();
					levelEditor.focusedChunk->SpawnByIndex(idx, nullptr);
					levelEditor.NotifySelectedObject();

					//for (auto* obj : GameManager::GetInstance()->objects) {
					//	hh::dbg::MsgUpdateActiveObjectInEditor msg{};
					//	obj->SendMessageImm(msg);
					//}
				}
			}
		}
	}
	ImGui::End();
}

void ObjectDataInspector::Distribute()
{
	if (alignX) DistributeAlongBasis(Eigen::Vector3f::UnitX(), distributeSpacing.x());
	if (alignY) DistributeAlongBasis(Eigen::Vector3f::UnitY(), distributeSpacing.y());
	if (alignZ) DistributeAlongBasis(Eigen::Vector3f::UnitZ(), distributeSpacing.z());

	levelEditor.RecalculateDependentTransforms();
}

void ObjectDataInspector::DistributeAlongBasis(const Eigen::Vector3f& basis, float spacing)
{
	Eigen::Vector3f avg{ 0.0f, 0.0f, 0.0f };

	if (levelEditor.focusedObjects.size() == 0)
		return;

	for (auto* obj : levelEditor.focusedObjects)
		avg += obj->transform.position.cwiseProduct(basis);
	avg /= levelEditor.focusedObjects.size();

	size_t idx{};
	float startOffset{ -static_cast<float>(levelEditor.focusedObjects.size() - 1) / 2.0f };
	for (auto* obj : levelEditor.focusedObjects) {
		auto transform = obj->transform;
		transform.position = { transform.position - transform.position.cwiseProduct(basis) + avg + basis * spacing * (startOffset + static_cast<float>(idx)) };
		UpdateAbsoluteTransform(ObjectTransformDataToAffine3f(transform), *obj);
		idx++;
	}
}

