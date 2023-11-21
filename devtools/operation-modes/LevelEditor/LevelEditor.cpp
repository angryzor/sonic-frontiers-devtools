#include "../../Pch.h"
#include "LevelEditor.h"
#include "../../GameServiceInspector.h"
#include "../../ResourceBrowser.h"
#include "../../common/SimpleWidgets.h"
#include "../../Desktop.h"

using namespace hh::fnd;
using namespace hh::game;
using namespace hh::physics;

LevelEditor::LevelEditor(csl::fnd::IAllocator* allocator) : OperationMode{ allocator }
{
}

void LevelEditor::Render() {
	auto* gameManager = GameManager::GetInstance();
	if (!gameManager)
		return;

	setObjectList.Render();
	objectDataInspector.Render();
	GameServiceInspector::Render();

	if (Desktop::instance->IsPickerMouseReleased()) {
		auto* picked = Desktop::instance->GetPickedObject();

		if (picked == nullptr)
			focusedObject = nullptr;
		else {
			auto* status = picked->GetWorldObjectStatus();

			if (status == nullptr)
				focusedObject = nullptr;
			else
				focusedObject = status->objectData;
		}
	}

	if (focusedObject) {
		auto* camera = gameManager->GetService<hh::game::CameraManager>()->GetTopComponent(0);

		if (camera) {
			hh::gfnd::GraphicsContext* gctx = hh::gfnd::GraphicsContext::GetInstance();

			ImGuiIO& io = ImGui::GetIO();

			Eigen::Transform<float, 3, Eigen::Affine> absoluteTransform{};
			Eigen::Transform<float, 3, Eigen::Affine> localTransform{};
			Eigen::Matrix4f delta;

			absoluteTransform.fromPositionOrientationScale(
				focusedObject->transform.position,
				Eigen::AngleAxisf(focusedObject->transform.rotation[0], Eigen::Vector3f::UnitX()) * Eigen::AngleAxisf(focusedObject->transform.rotation[1], Eigen::Vector3f::UnitY()) * Eigen::AngleAxisf(focusedObject->transform.rotation[2], Eigen::Vector3f::UnitZ()),
				csl::math::Vector3{ 1.0f, 1.0f, 1.0f }
			);

			localTransform.fromPositionOrientationScale(
				focusedObject->localTransform.position,
				Eigen::AngleAxisf(focusedObject->localTransform.rotation[0], Eigen::Vector3f::UnitX()) * Eigen::AngleAxisf(focusedObject->localTransform.rotation[1], Eigen::Vector3f::UnitY()) * Eigen::AngleAxisf(focusedObject->localTransform.rotation[2], Eigen::Vector3f::UnitZ()),
				csl::math::Vector3{ 1.0f, 1.0f, 1.0f }
			);

			ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
			ImGuizmo::Manipulate(camera->viewportData.viewMatrix.data(), gctx->defaultViewportData.projMatrix.data(), gizmoOperation, gizmoMode, reinterpret_cast<float*>(&absoluteTransform), delta.data(), NULL);

			Eigen::Matrix3f absoluteRotation;
			Eigen::Matrix3f absoluteScaling;

			absoluteTransform.computeRotationScaling(&absoluteRotation, &absoluteScaling);

			//focusedObject->transform.position = { absoluteTransform.translation() };
			//focusedObject->transform.rotation = absoluteRotation.eulerAngles(0, 1, 2);

			Eigen::Matrix3f localRotation;
			Eigen::Matrix3f localScaling;

			localTransform *= delta;
			localTransform.computeRotationScaling(&localRotation, &localScaling);

			//focusedObject->localTransform.position = { localTransform.translation() };
			//focusedObject->localTransform.rotation = localRotation.eulerAngles(0, 1, 2);

			auto* objWorld = GameManager::GetInstance()->GetService<ObjectWorld>();

			for (auto* chunk : objWorld->GetWorldChunks()) {
				int idx = chunk->GetObjectIndexById(focusedObject->id);

				if (idx != -1) {
					auto* obj = chunk->GetObjectByIndex(idx);

					if (obj == nullptr)
						break;

					auto* gocTransform = obj->GetComponent<GOCTransform>();

					if (gocTransform == nullptr)
						break;

					gocTransform->SetLocalTransform({ { localTransform.translation() }, { Eigen::Quaternionf{ localRotation } }, { Eigen::Vector3f{ 1.0f, 1.0f, 1.0f } } });
				}
			}

			if ((ImGui::IsKeyDown(ImGuiKey_LeftAlt) || ImGui::IsKeyDown(ImGuiKey_RightAlt)) && ImGui::IsKeyPressed(ImGuiKey_Space))
				gizmoMode = gizmoMode == ImGuizmo::LOCAL ? ImGuizmo::WORLD : ImGuizmo::LOCAL;

			if (ImGui::IsKeyPressed(ImGuiKey_G))
				gizmoOperation = ImGuizmo::TRANSLATE;

			if (ImGui::IsKeyPressed(ImGuiKey_R))
				gizmoOperation = ImGuizmo::ROTATE;

			if (gizmoOperation & ImGuizmo::TRANSLATE) {
				if (ImGui::IsKeyPressed(ImGuiKey_X))
					gizmoOperation = ImGui::IsKeyDown(ImGuiKey_LeftShift) || ImGui::IsKeyDown(ImGuiKey_RightShift) ? ImGuizmo::TRANSLATE_Y | ImGuizmo::TRANSLATE_Z : ImGuizmo::TRANSLATE_X;

				if (ImGui::IsKeyPressed(ImGuiKey_Y))
					gizmoOperation = ImGui::IsKeyDown(ImGuiKey_LeftShift) || ImGui::IsKeyDown(ImGuiKey_RightShift) ? ImGuizmo::TRANSLATE_X | ImGuizmo::TRANSLATE_Z : ImGuizmo::TRANSLATE_Y;

				if (ImGui::IsKeyPressed(ImGuiKey_Z))
					gizmoOperation = ImGui::IsKeyDown(ImGuiKey_LeftShift) || ImGui::IsKeyDown(ImGuiKey_RightShift) ? ImGuizmo::TRANSLATE_X | ImGuizmo::TRANSLATE_Y : ImGuizmo::TRANSLATE_Z;
			}

			if (gizmoOperation & ImGuizmo::ROTATE) {
				if (ImGui::IsKeyPressed(ImGuiKey_X))
					gizmoOperation = ImGui::IsKeyDown(ImGuiKey_LeftShift) || ImGui::IsKeyDown(ImGuiKey_RightShift) ? ImGuizmo::ROTATE_Y | ImGuizmo::ROTATE_Z : ImGuizmo::ROTATE_X;

				if (ImGui::IsKeyPressed(ImGuiKey_Y))
					gizmoOperation = ImGui::IsKeyDown(ImGuiKey_LeftShift) || ImGui::IsKeyDown(ImGuiKey_RightShift) ? ImGuizmo::ROTATE_X | ImGuizmo::ROTATE_Z : ImGuizmo::ROTATE_Y;

				if (ImGui::IsKeyPressed(ImGuiKey_Z))
					gizmoOperation = ImGui::IsKeyDown(ImGuiKey_LeftShift) || ImGui::IsKeyDown(ImGuiKey_RightShift) ? ImGuizmo::ROTATE_X | ImGuizmo::ROTATE_Y : ImGuizmo::ROTATE_Z;
			}
		}
	}
}
