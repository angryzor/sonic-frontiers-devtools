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
	auto* gameManager = GameManager::GetInstance();
	if (!gameManager)
		return;

	gameManager->AddListener(this);

	auto* objWorld = gameManager->GetService<ObjectWorld>();

	if (!objWorld)
		return;

	objWorld->AddWorldListener(this);
}

LevelEditor::~LevelEditor() {
	auto* gameManager = GameManager::GetInstance();
	if (!gameManager)
		return;

	gameManager->RemoveListener(this);

	auto* objWorld = gameManager->GetService<ObjectWorld>();

	if (!objWorld)
		return;

	objWorld->RemoveWorldListener(this);
}

void LevelEditor::Render() {
	auto* gameManager = GameManager::GetInstance();
	if (!gameManager)
		return;

	setObjectList.Render();
	objectDataInspector.Render();
	objectLibrary.Render();

	if (!focusedChunk)
		return;

	auto* objWorld = GameManager::GetInstance()->GetService<ObjectWorld>();

	if (Desktop::instance->IsPickerMouseReleased()) {
		auto* picked = Desktop::instance->GetPickedObject();

		if (picked == nullptr)
			focusedObject = nullptr;
		else {
			auto* status = picked->GetWorldObjectStatus();

			if (status == nullptr || focusedChunk->GetObjectIndexById(status->objectData->id) == -1)
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

			// We want ImGuizmo to operate on the absolute transform so it can position the gizmo properly, yet also apply the changes on the local transform,
			// so we do some matrix juggling here to convert the changed absolute transform back to a local transform.
			Eigen::Transform<float, 3, Eigen::Affine> parentTransform = absoluteTransform * localTransform.inverse();

			ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
			ImGuizmo::Manipulate(camera->viewportData.viewMatrix.data(), camera->viewportData.projMatrix.data(), gizmoOperation, gizmoMode, absoluteTransform.data(), NULL, NULL);

			Eigen::Transform<float, 3, Eigen::Affine> updatedLocalTransform = parentTransform.inverse() * absoluteTransform;

			Eigen::Matrix3f absoluteRotation;
			Eigen::Matrix3f absoluteScaling;

			absoluteTransform.computeRotationScaling(&absoluteRotation, &absoluteScaling);

			focusedObject->transform.position = { absoluteTransform.translation() };
			focusedObject->transform.rotation = absoluteRotation.eulerAngles(0, 1, 2);

			Eigen::Matrix3f localRotation;
			Eigen::Matrix3f localScaling;

			updatedLocalTransform.computeRotationScaling(&localRotation, &localScaling);

			focusedObject->localTransform.position = { updatedLocalTransform.translation() };
			focusedObject->localTransform.rotation = localRotation.eulerAngles(0, 1, 2);

			int idx = focusedChunk->GetObjectIndexById(focusedObject->id);

			if (idx != -1) {
				auto* obj = focusedChunk->GetObjectByIndex(idx);

				if (obj) {
					auto* gocTransform = obj->GetComponent<GOCTransform>();

					if (gocTransform) {
						// Depending on whether the parent was able to be spawned, the object uses the local or the absolute transform as the GOC transform, so we have to replicate that here.
						if (gocTransform->IsExistParent())
							gocTransform->SetLocalTransform({ { updatedLocalTransform.translation() }, { Eigen::Quaternionf{ localRotation } }, { Eigen::Vector3f{ 1.0f, 1.0f, 1.0f } } });
						else
							gocTransform->SetLocalTransform({ { absoluteTransform.translation() }, { Eigen::Quaternionf{ localRotation } }, { Eigen::Vector3f{ 1.0f, 1.0f, 1.0f } } });
					}
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

	if (Desktop::instance->IsPickerMouseReleased() && objectClassToPlace && Desktop::instance->GetPickedLocation()) {
		auto* resource = focusedChunk->GetLayers()[1]->GetResource();
		auto* alloc = resource->GetAllocator();

		char name[200];
		snprintf(name, sizeof(name), "%s_%zd", objectClassToPlace->pName, resource->GetObjects().size());

		auto* objData = new (alloc) ObjectData{
			alloc,
			objectClassToPlace,
			name,
			nullptr,
			{ csl::math::Position{ *Desktop::instance->GetPickedLocation() }, csl::math::Position{ 0, 0, 0 } }
		};
		
		resource->AddObject(objData);
		focusedChunk->AddWorldObjectStatus(*objData, true, 0);
		auto idx = focusedChunk->GetObjectIndexById(objData->id);
		focusedChunk->SpawnByIndex(idx, nullptr);
	}
}

void LevelEditor::GameServiceAddedCallback(GameService* service) {
	if (service->pStaticClass == ObjectWorld::GetClass()) {
		auto* objWorld = static_cast<ObjectWorld*>(service);
		objWorld->AddWorldListener(this);
	}
}

void LevelEditor::GameServiceRemovedCallback(GameService* service) {
	if (service->pStaticClass == ObjectWorld::GetClass()) {
		auto* objWorld = static_cast<ObjectWorld*>(service);
		objWorld->RemoveWorldListener(this);
	}
}

void LevelEditor::WorldChunkRemovedCallback(ObjectWorldChunk* chunk)
{
	if (focusedChunk == chunk) {
		SetFocusedChunk(nullptr);
	}
}

void LevelEditor::SetFocusedChunk(ObjectWorldChunk* chunk)
{
	if (focusedChunk == chunk)
		return;

	focusedObject = nullptr;
	if (focusedChunk) {
		focusedChunk->DespawnAll();
		focusedChunk->ShutdownPendingObjects();
		focusedChunk->SetEditorStatus(false);
	}
	focusedChunk = chunk;
	if (focusedChunk) {
		focusedChunk->SetEditorStatus(true);
		focusedChunk->DespawnAll();
		focusedChunk->ShutdownPendingObjects();
		focusedChunk->Restart(true);
	}
}
