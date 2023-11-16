#include "../../Pch.h"
#include "ObjectInspection.h"
#include "../../GameServiceInspector.h"
#include "../../ResourceBrowser.h"
#include "../../common/SimpleWidgets.h"

using namespace hh::fnd;
using namespace hh::game;

ObjectInspection::ViewerContextHolder::ViewerContextHolder(csl::fnd::IAllocator* allocator) : ReferencedObject{ allocator, true } {
}

ObjectInspection::DebuggerThingEmulator::DebuggerThingEmulator(csl::fnd::IAllocator* allocator) : ReferencedObject{ allocator, true } {
	ctxHolder = new (allocator) ViewerContextHolder(allocator);
}

ObjectInspection::ObjectInspection(csl::fnd::IAllocator* allocator, Desktop& desktop) : OperationMode{ allocator }, desktop{ desktop }
{
	dbgEmulator = new (allocator) DebuggerThingEmulator(allocator);
	gameViewerCtx = hh::dbg::ViewerContext::Create<GameViewerContext>(allocator);
	gameViewerCtx->gameManagers.push_back(GameManager::GetInstance());
	dbgEmulator->ctxHolder->viewerContext.push_back(gameViewerCtx);
	objViewerCtx = hh::dbg::ViewerContext::Create<ObjectViewerContext>(allocator);
	dbgEmulator->ctxHolder->viewerContext.push_back(objViewerCtx);
	*rangerssdk::GetAddress(&DebuggerThingEmulator::instance) = dbgEmulator;
	picker = MousePickingViewer::Create(allocator);
}

void ObjectInspection::Render() {
	auto* gameManager = GameManager::GetInstance();
	if (!gameManager)
		return;

	objectList.Render();
	objectInspector.Render();
	GameServiceInspector::Render();

	if (picker->objectViewerContext->selectedObject != nullptr)
		focusedObject = picker->objectViewerContext->selectedObject;

	if (focusedObject) {
		auto* gocTransform = focusedObject->GetComponent<GOCTransform>();
		auto* camera = gameManager->GetService<hh::game::CameraManager>()->GetTopComponent(0);

		if (gocTransform && camera) {
			hh::gfnd::GraphicsContext* gctx = hh::gfnd::GraphicsContext::GetInstance();

			ImGuiIO& io = ImGui::GetIO();

			Eigen::Transform<float, 3, Eigen::Affine> transform{};
			transform.fromPositionOrientationScale(gocTransform->transform.position, gocTransform->transform.rotation, gocTransform->transform.scale);

			ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
			ImGuizmo::Manipulate(camera->viewportData.viewMatrix.data(), gctx->defaultViewportData.projMatrix.data(), gizmoOperation, gizmoMode, reinterpret_cast<float*>(&transform), NULL, NULL);

			Eigen::Matrix3f rotation;
			Eigen::Matrix3f scaling;

			transform.computeRotationScaling(&rotation, &scaling);

			gocTransform->SetLocalTransform({ { transform.translation() }, { Eigen::Quaternionf{ rotation } }, { Eigen::Vector3f{ scaling.diagonal() } } });

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
