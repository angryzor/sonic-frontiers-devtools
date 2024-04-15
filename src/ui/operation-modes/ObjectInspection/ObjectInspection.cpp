#include "ObjectInspection.h"
#include <ui/common/SimpleWidgets.h>
#include <ui/Desktop.h>
#include <utilities/math/MathUtils.h>
#include <utilities/BoundingBoxes.h>

using namespace hh::fnd;
using namespace hh::game;
using namespace hh::physics;

ObjectInspection::ObjectInspection(csl::fnd::IAllocator* allocator) : OperationMode{ allocator }
{
}

void ObjectInspection::Render() {
	objectList.Render();
	objectInspector.Render();

	if (focusedObjects.size() > 0) {
		if (focusedObjects.size() > 1 && gizmoOperation & ImGuizmo::SCALE)
			gizmoOperation = ImGuizmo::TRANSLATE;

		haveSelectionAabb = CalcApproxAabb(reinterpret_cast<csl::ut::MoveArray<GameObject*>&>(focusedObjects), selectionAabb);
		HandleObjectManipulation();
	} else
		haveSelectionAabb = false;

	HandleObjectSelection();
}

void ObjectInspection::RenderDebugVisuals(hh::gfnd::DrawContext& ctx)
{
	if (haveSelectionAabb) {
		csl::geom::Aabb aabb{ selectionAabb.m_Min, selectionAabb.m_Max };
		ctx.DrawAABB(aabb.m_Min, aabb.m_Max, { 140, 255, 255, 255 });
	}
}

void ObjectInspection::HandleObjectSelection() {
	if (Desktop::instance->IsPickerMouseReleased()) {
		auto& picked = Desktop::instance->GetPickedObjects();
		focusedObjects.clear();
		for (auto* object : picked)
			focusedObjects.push_back(object);
	}

	CheckSelectionHotkeys();
}

void ObjectInspection::HandleObjectManipulation() {
	auto* gameManager = GameManager::GetInstance();
	auto* firstObjGocTransform = focusedObjects[0]->GetComponent<GOCTransform>();
	auto* camera = gameManager->GetService<hh::game::CameraManager>()->GetTopComponent(0);

	// If the first has a transform, they all do (currently...), because that only happens with multiselect which selects by GOCTransform.
	if (firstObjGocTransform && camera) {
		hh::gfnd::GraphicsContext* gctx = hh::gfnd::GraphicsContext::GetInstance();

		ImGuiIO& io = ImGui::GetIO();

		Eigen::Affine3f pivotTransform{};
		if (focusedObjects.size() > 1 && haveSelectionAabb)
			pivotTransform.fromPositionOrientationScale((selectionAabb.m_Min + selectionAabb.m_Max) / 2.0f, Eigen::Quaternionf::Identity(), Eigen::Vector3f{ 1.0f, 1.0f, 1.0f });
		else
			pivotTransform = TransformToAffine3f(firstObjGocTransform->frame->fullTransform);

		csl::ut::MoveArray<GameObject*> objsToMove{ hh::fnd::MemoryRouter::GetTempAllocator() };
		for (auto& object : focusedObjects) {
			GameObject* obj = object;
			while (true) {
				auto* gocTransform = obj->GetComponent<GOCTransform>();
				if (auto* parentGoc = gocTransform->GetParent()) {
					auto* parentObj = parentGoc->GetOwnerGameObject();
					if (focusedObjects.find(parentObj) != -1)
						break;
					obj = parentObj;
				}
				else {
					objsToMove.push_back(object);
					break;
				}
			}
		}

		csl::ut::MoveArray<Eigen::Affine3f> preTransforms{ hh::fnd::MemoryRouter::GetTempAllocator() };
		csl::ut::MoveArray<Eigen::Affine3f> postTransforms{ hh::fnd::MemoryRouter::GetTempAllocator() };

		for (auto& object : objsToMove) {
			auto* gocTransform = object->GetComponent<GOCTransform>();
			auto absoluteTransform = TransformToAffine3f(gocTransform->frame->fullTransform);
			auto localTransform = TransformToAffine3f(gocTransform->transform);
			postTransforms.push_back(localTransform * absoluteTransform.inverse());
			preTransforms.push_back(pivotTransform.inverse() * absoluteTransform);
		}

		ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);

		if (ImGuizmo::Manipulate(camera->viewportData.viewMatrix.data(), gctx->viewportDatas[0].projMatrix.data(), gizmoOperation, gizmoMode, pivotTransform.data(), NULL, NULL)) {
			size_t i{ 0 };
			for (auto& object : objsToMove) {
				object->GetComponent<GOCTransform>()->SetLocalTransform(Affine3fToTransform(postTransforms[i] * pivotTransform * preTransforms[i]));
				i++;
			}
		}
	}

	CheckGizmoHotkeys();
}

void ObjectInspection::CheckGizmoHotkeys() {
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

void ObjectInspection::CheckSelectionHotkeys() {
	if (ImGui::IsKeyPressed(ImGuiKey_Delete)) {
		DeleteFocusedObject();
	}

	if (ImGui::IsKeyPressed(ImGuiKey_Escape)) {
		focusedObjects.clear();
	}
}

void ObjectInspection::DeleteFocusedObject() {
	for (auto& focusedObject : focusedObjects)
		focusedObject->Kill();
	focusedObjects.clear();
}
