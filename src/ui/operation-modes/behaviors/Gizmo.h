#pragma once
#include <ui/Action.h>
#include <ui/Desktop.h>
#include <ui/Shortcuts.h>
#include <ui/operation-modes/OperationModeBehavior.h>
#include "SelectionTransformation.h"

class GizmoBehavior : public OperationModeBehavior {
    ImGuizmo::OPERATION gizmoOperation{ ImGuizmo::TRANSLATE };
    ImGuizmo::MODE gizmoMode{ ImGuizmo::LOCAL };
    bool allowTranslate;
    bool allowRotate;
    bool allowScale;

public:
	using ChangeCoordinateSystemAction = Action<ActionId::CHANGE_COORDINATE_SYSTEM>;
	using TranslateModeAction = Action<ActionId::TRANSLATE_MODE>;
	using RotateModeAction = Action<ActionId::ROTATE_MODE>;
	using ScaleModeAction = Action<ActionId::SCALE_MODE>;
	using OnlyXAxisAction = Action<ActionId::ONLY_X_AXIS>;
	using LockXAxisAction = Action<ActionId::LOCK_X_AXIS>;
	using OnlyYAxisAction = Action<ActionId::ONLY_Y_AXIS>;
	using LockYAxisAction = Action<ActionId::LOCK_Y_AXIS>;
	using OnlyZAxisAction = Action<ActionId::ONLY_Z_AXIS>;
	using LockZAxisAction = Action<ActionId::LOCK_Z_AXIS>;

	GizmoBehavior(csl::fnd::IAllocator* allocator, OperationMode& operationMode, bool allowTranslate = true, bool allowRotate = true, bool allowScale = true) : OperationModeBehavior{ allocator, operationMode }, allowTranslate{ allowTranslate }, allowRotate{ allowRotate }, allowScale{ allowScale } {}

	static constexpr unsigned int id = 2;
	virtual unsigned int GetId() override { return id; }

	virtual void Init() {
		Desktop::instance->BindShortcut<ChangeCoordinateSystemAction>(ShortcutId::CHANGE_COORDINATE_SYSTEM);
		Desktop::instance->BindShortcut<TranslateModeAction>(ShortcutId::TRANSLATE_MODE);
		Desktop::instance->BindShortcut<RotateModeAction>(ShortcutId::ROTATE_MODE);
		Desktop::instance->BindShortcut<ScaleModeAction>(ShortcutId::SCALE_MODE);
		Desktop::instance->BindShortcut<OnlyXAxisAction>(ShortcutId::ONLY_X_AXIS);
		Desktop::instance->BindShortcut<LockXAxisAction>(ShortcutId::LOCK_X_AXIS);
		Desktop::instance->BindShortcut<OnlyYAxisAction>(ShortcutId::ONLY_Y_AXIS);
		Desktop::instance->BindShortcut<LockYAxisAction>(ShortcutId::LOCK_Y_AXIS);
		Desktop::instance->BindShortcut<OnlyZAxisAction>(ShortcutId::ONLY_Z_AXIS);
		Desktop::instance->BindShortcut<LockZAxisAction>(ShortcutId::LOCK_Z_AXIS);
	}

	virtual void Deinit() {
		Desktop::instance->UnbindShortcut(ShortcutId::CHANGE_COORDINATE_SYSTEM);
		Desktop::instance->UnbindShortcut(ShortcutId::TRANSLATE_MODE);
		Desktop::instance->UnbindShortcut(ShortcutId::ROTATE_MODE);
		Desktop::instance->UnbindShortcut(ShortcutId::SCALE_MODE);
		Desktop::instance->UnbindShortcut(ShortcutId::ONLY_X_AXIS);
		Desktop::instance->UnbindShortcut(ShortcutId::LOCK_X_AXIS);
		Desktop::instance->UnbindShortcut(ShortcutId::ONLY_Y_AXIS);
		Desktop::instance->UnbindShortcut(ShortcutId::LOCK_Y_AXIS);
		Desktop::instance->UnbindShortcut(ShortcutId::ONLY_Z_AXIS);
		Desktop::instance->UnbindShortcut(ShortcutId::LOCK_Z_AXIS);
	}

    virtual void ProcessAction(const ActionBase& action) {
		switch (action.id) {
		case ChangeCoordinateSystemAction::id:
			gizmoMode = gizmoMode == ImGuizmo::LOCAL ? ImGuizmo::WORLD : ImGuizmo::LOCAL;
			break;
		case TranslateModeAction::id:
			if (allowTranslate)
				gizmoOperation = ImGuizmo::TRANSLATE;
			break;
		case RotateModeAction::id:
			if (allowRotate)
				gizmoOperation = ImGuizmo::ROTATE;
			break;
		case ScaleModeAction::id:
			if (allowScale)
				gizmoOperation = ImGuizmo::SCALE;
			break;
		case OnlyXAxisAction::id:
			if (gizmoOperation & ImGuizmo::TRANSLATE)
				gizmoOperation = ImGuizmo::TRANSLATE_X;
			if (gizmoOperation & ImGuizmo::ROTATE)
				gizmoOperation = ImGuizmo::ROTATE_X;
			if (gizmoOperation & ImGuizmo::SCALE)
				gizmoOperation = ImGuizmo::SCALE_X;
			break;
		case LockXAxisAction::id:
			if (gizmoOperation & ImGuizmo::TRANSLATE)
				gizmoOperation = ImGuizmo::TRANSLATE_Y | ImGuizmo::TRANSLATE_Z;
			if (gizmoOperation & ImGuizmo::ROTATE)
				gizmoOperation = ImGuizmo::ROTATE_Y | ImGuizmo::ROTATE_Z;
			if (gizmoOperation & ImGuizmo::SCALE)
				gizmoOperation = ImGuizmo::SCALE_Y | ImGuizmo::SCALE_Z;
			break;
		case OnlyYAxisAction::id:
			if (gizmoOperation & ImGuizmo::TRANSLATE)
				gizmoOperation = ImGuizmo::TRANSLATE_Y;
			if (gizmoOperation & ImGuizmo::ROTATE)
				gizmoOperation = ImGuizmo::ROTATE_Y;
			if (gizmoOperation & ImGuizmo::SCALE)
				gizmoOperation = ImGuizmo::SCALE_Y;
			break;
		case LockYAxisAction::id:
			if (gizmoOperation & ImGuizmo::TRANSLATE)
				gizmoOperation = ImGuizmo::TRANSLATE_X | ImGuizmo::TRANSLATE_Z;
			if (gizmoOperation & ImGuizmo::ROTATE)
				gizmoOperation = ImGuizmo::ROTATE_X | ImGuizmo::ROTATE_Z;
			if (gizmoOperation & ImGuizmo::SCALE)
				gizmoOperation = ImGuizmo::SCALE_X | ImGuizmo::SCALE_Z;
			break;
		case OnlyZAxisAction::id:
			if (gizmoOperation & ImGuizmo::TRANSLATE)
				gizmoOperation = ImGuizmo::TRANSLATE_Z;
			if (gizmoOperation & ImGuizmo::ROTATE)
				gizmoOperation = ImGuizmo::ROTATE_Z;
			if (gizmoOperation & ImGuizmo::SCALE)
				gizmoOperation = ImGuizmo::SCALE_Z;
			break;
		case LockZAxisAction::id:
			if (gizmoOperation & ImGuizmo::TRANSLATE)
				gizmoOperation = ImGuizmo::TRANSLATE_X | ImGuizmo::TRANSLATE_Y;
			if (gizmoOperation & ImGuizmo::ROTATE)
				gizmoOperation = ImGuizmo::ROTATE_X | ImGuizmo::ROTATE_Y;
			if (gizmoOperation & ImGuizmo::SCALE)
				gizmoOperation = ImGuizmo::SCALE_X | ImGuizmo::SCALE_Y;
			break;
		}
    }

    virtual void Render() override {
		if (auto* gameManager = hh::game::GameManager::GetInstance())
		if (auto* cameraManager = gameManager->GetService<hh::game::CameraManager>())
		if (auto* camera = cameraManager->GetTopComponent(0)) {
			auto* selTransform = operationMode.GetBehavior<SelectionTransformationBehaviorBase<false>>();

			if (!selTransform->HasTransformableSelection())
				return;

			auto selectionTransform = selTransform->GetSelectionTransform();

			ImGuiIO& io = ImGui::GetIO();
			ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);

			if (ImGuizmo::Manipulate(camera->viewportData.viewMatrix.data(), camera->viewportData.projMatrix.data(), gizmoOperation, gizmoMode, selectionTransform.data(), NULL, NULL))
				selTransform->SetSelectionTransform(selectionTransform);
		}
    }
};
