#include "../../Pch.h"
#include "ObjectInspection.h"
#include "../../GameServiceInspector.h"
#include "../../ResourceBrowser.h"
#include "../../common/SimpleWidgets.h"
#include "../../Desktop.h"

using namespace hh::fnd;
using namespace hh::game;
using namespace hh::physics;

ObjectInspection::ObjectInspection(csl::fnd::IAllocator* allocator) : OperationMode{ allocator }
{
}

void ObjectInspection::Render() {
	auto* gameManager = GameManager::GetInstance();
	if (!gameManager)
		return;

	objectList.Render();
	objectInspector.Render();

	if (Desktop::instance->IsPickerMouseReleased())
		focusedObject = Desktop::instance->GetPickedObject();

	if (focusedObject) {
		auto* gocTransform = focusedObject->GetComponent<GOCTransform>();
		auto* camera = gameManager->GetService<hh::game::CameraManager>()->GetTopComponent(0);

		if (gocTransform && camera) {
			hh::gfnd::GraphicsContext* gctx = hh::gfnd::GraphicsContext::GetInstance();

			ImGuiIO& io = ImGui::GetIO();

			// We want ImGuizmo to operate on the absolute transform so it can position the gizmo properly, yet apply the changes on the local transform,
			// so we do some matrix juggling here to convert the changed absolute transform back to a local transform.
			Eigen::Transform<float, 3, Eigen::Affine> absoluteTransform{};
			absoluteTransform.fromPositionOrientationScale(gocTransform->frame->fullTransform.position, gocTransform->frame->fullTransform.rotation, gocTransform->frame->fullTransform.scale);

			Eigen::Transform<float, 3, Eigen::Affine> localTransform{};
			localTransform.fromPositionOrientationScale(gocTransform->transform.position, gocTransform->transform.rotation, gocTransform->transform.scale);

			Eigen::Transform<float, 3, Eigen::Affine> parentTransform = absoluteTransform * localTransform.inverse();

			ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
			ImGuizmo::Manipulate(camera->viewportData.viewMatrix.data(), gctx->viewportDatas[0].projMatrix.data(), gizmoOperation, gizmoMode, absoluteTransform.data(), NULL, NULL);

			Eigen::Transform<float, 3, Eigen::Affine> updatedLocalTransform = parentTransform.inverse() * absoluteTransform;

			Eigen::Matrix3f rotation;
			Eigen::Matrix3f scaling;

			updatedLocalTransform.computeRotationScaling(&rotation, &scaling);

			gocTransform->SetLocalTransform({ { updatedLocalTransform.translation() }, { Eigen::Quaternionf{ rotation } }, { Eigen::Vector3f{ scaling.diagonal() } } });

			if ((ImGui::IsKeyDown(ImGuiKey_LeftAlt) || ImGui::IsKeyDown(ImGuiKey_RightAlt)) && ImGui::IsKeyPressed(ImGuiKey_Space))
				gizmoMode = gizmoMode == ImGuizmo::LOCAL ? ImGuizmo::WORLD : ImGuizmo::LOCAL;

			if (ImGui::IsKeyPressed(ImGuiKey_G))
				gizmoOperation = ImGuizmo::TRANSLATE;

			if (ImGui::IsKeyPressed(ImGuiKey_R))
				gizmoOperation = ImGuizmo::ROTATE;

			if (ImGui::IsKeyPressed(ImGuiKey_S))
				gizmoOperation = ImGuizmo::SCALE;

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

			if (gizmoOperation & ImGuizmo::SCALE) {
				if (ImGui::IsKeyPressed(ImGuiKey_X))
					gizmoOperation = ImGui::IsKeyDown(ImGuiKey_LeftShift) || ImGui::IsKeyDown(ImGuiKey_RightShift) ? ImGuizmo::SCALE_Y | ImGuizmo::SCALE_Z : ImGuizmo::SCALE_X;

				if (ImGui::IsKeyPressed(ImGuiKey_Y))
					gizmoOperation = ImGui::IsKeyDown(ImGuiKey_LeftShift) || ImGui::IsKeyDown(ImGuiKey_RightShift) ? ImGuizmo::SCALE_X | ImGuizmo::SCALE_Z : ImGuizmo::SCALE_Y;

				if (ImGui::IsKeyPressed(ImGuiKey_Z))
					gizmoOperation = ImGui::IsKeyDown(ImGuiKey_LeftShift) || ImGui::IsKeyDown(ImGuiKey_RightShift) ? ImGuizmo::SCALE_X | ImGuizmo::SCALE_Y : ImGuizmo::SCALE_Z;
			}
		}
	}
}
