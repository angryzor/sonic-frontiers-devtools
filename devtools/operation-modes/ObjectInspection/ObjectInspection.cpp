#include "../../Pch.h"
#include "ObjectInspection.h"
#include "../../GameServiceInspector.h"
#include "../../ResourceBrowser.h"
#include "../../EulerTransform.h"

using namespace hh::fnd;
using namespace hh::game;

namespace heur::services {
	class CameraManager : public GameService {
	public:
		class SomeCameraClass {
		public:
			void* unk1[6];
			csl::math::Matrix44 viewMatrix;
		};

		SomeCameraClass* GetCamera(int id);
		
		static GameServiceClass* GetClass();
	};
}

ObjectInspection::ObjectInspection(csl::fnd::IAllocator* allocator, Desktop& desktop) : OperationMode{ allocator }, desktop{ desktop }
{
}

void ObjectInspection::Render() {
	auto* gameManager = GameManager::GetInstance();
	if (!gameManager)
		return;

	objectList.Render();
	objectInspector.Render();
	GameServiceInspector::Render();

	if (focusedObject) {
		auto* gocTransform = focusedObject->GetComponent<GOCTransform>();
		auto* camera = gameManager->GetService<heur::services::CameraManager>()->GetCamera(0);

		if (gocTransform && camera) {
			hh::gfnd::GraphicsContext* gctx = *rangerssdk::bootstrap::GetAddress(&hh::gfnd::GraphicsContext::instance);

			EulerTransform localTransform{ gocTransform->transform };

			csl::math::Matrix44 mat;
			ImGuiIO& io = ImGui::GetIO();

			ImGuizmo::RecomposeMatrixFromComponents(reinterpret_cast<float*>(&localTransform.position), reinterpret_cast<float*>(&localTransform.rotation), reinterpret_cast<float*>(&localTransform.scale), reinterpret_cast<float*>(&mat));
			ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
			ImGuizmo::Manipulate(camera->viewMatrix.data(), gctx->viewportData.projMatrix.data(), gizmoOperation, ImGuizmo::WORLD, reinterpret_cast<float*>(&mat), NULL, NULL);
			ImGuizmo::DecomposeMatrixToComponents(reinterpret_cast<float*>(&mat), reinterpret_cast<float*>(&localTransform.position), reinterpret_cast<float*>(&localTransform.rotation), reinterpret_cast<float*>(&localTransform.scale));

			gocTransform->SetLocalTransform(localTransform);

			if (ImGui::IsKeyPressed(ImGuiKey_G))
				gizmoOperation = ImGuizmo::TRANSLATE;

			if (ImGui::IsKeyPressed(ImGuiKey_R))
				gizmoOperation = ImGuizmo::ROTATE;

			if (ImGui::IsKeyPressed(ImGuiKey_S))
				gizmoOperation = ImGuizmo::SCALE;

			if (gizmoOperation == ImGuizmo::TRANSLATE) {
				if (ImGui::IsKeyPressed(ImGuiKey_X))
					gizmoOperation = ImGuizmo::TRANSLATE_X;

				if (ImGui::IsKeyPressed(ImGuiKey_Y))
					gizmoOperation = ImGuizmo::TRANSLATE_Y;

				if (ImGui::IsKeyPressed(ImGuiKey_Z))
					gizmoOperation = ImGuizmo::TRANSLATE_Z;
			}

			if (gizmoOperation == ImGuizmo::ROTATE) {
				if (ImGui::IsKeyPressed(ImGuiKey_X))
					gizmoOperation = ImGuizmo::ROTATE_X;

				if (ImGui::IsKeyPressed(ImGuiKey_Y))
					gizmoOperation = ImGuizmo::ROTATE_Y;

				if (ImGui::IsKeyPressed(ImGuiKey_Z))
					gizmoOperation = ImGuizmo::ROTATE_Z;
			}
		}
	}
}
