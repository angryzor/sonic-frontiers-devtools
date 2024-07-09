#include "LevelEditor.h"
#include <utilities/ObjectDataUtils.h>
#include <utilities/BoundingBoxes.h>
#include <ui/operation-modes/behaviors/Clipboard.h>
#include <ui/operation-modes/behaviors/Selection.h>
#include <ui/operation-modes/behaviors/SelectionAabb.h>
#include <ui/operation-modes/behaviors/SelectionMousePicking.h>
#include <ui/operation-modes/behaviors/SelectionTransformation.h>
#include <ui/operation-modes/behaviors/SelectionVisual.h>
#include <ui/operation-modes/behaviors/Gizmo.h>
#include <ui/operation-modes/behaviors/MousePicking.h>
#include <ui/operation-modes/behaviors/Delete.h>
#include <ui/operation-modes/behaviors/GroundContextMenu.h>
#include <ui/operation-modes/behaviors/DebugCommentsVisual.h>
#include <ui/operation-modes/BehaviorContext.h>
#include "SetObjectList.h"
#include "ObjectDataInspector.h"
#include "ObjectLibrary.h"
#include "Actions.h"

namespace app::gfx {
	class GOCVisualGeometryInstance : public GOComponent {
#ifdef DEVTOOLS_TARGET_SDK_wars
		GOCOMPONENT_CLASS_DECLARATION_INLINE_GETCLASS(GOCVisualGeometryInstance)
#else
		GOCOMPONENT_CLASS_DECLARATION(GOCVisualGeometryInstance)
#endif
	};
}

namespace ui::operation_modes::modes::level_editor {
	using namespace hh::fnd;
	using namespace hh::game;

	template<> struct ClipboardEntry<hh::game::ObjectData*> {
		Context& context;
		hh::game::ObjectData* proto;

		ClipboardEntry(Context& context, hh::game::ObjectData* proto) : context{ context }, proto{ context.CopyObjectForClipboard(proto) } {}
		ClipboardEntry(const ClipboardEntry& other) : context{ other.context }, proto{ context.CopyObjectForClipboard(other.proto) } {}
		ClipboardEntry(ClipboardEntry&& other) noexcept : context{ other.context }, proto{ other.proto } { other.proto = nullptr; }
		~ClipboardEntry() { context.TerminateClipboardObject(proto); }
	};

	class SelectionAabbBehaviorContext : public BehaviorContext<Context, SelectionAabbBehavior<ObjectData*>> {
		using BehaviorContext::BehaviorContext;
		virtual bool CalculateAabb(const csl::ut::MoveArray<hh::game::ObjectData*>& objects, csl::geom::Aabb& aabb) override {
			return CalcApproxAabb(context.GetFocusedChunk(), objects, aabb);
		}
	};

	class SelectionTransformationBehaviorContext : public BehaviorContext<Context, SelectionTransformationBehavior<ObjectData*>> {
		using BehaviorContext::BehaviorContext;
		virtual bool HasTransform(ObjectData* obj) override { return true; }
		virtual bool IsRoot(ObjectData* obj) override { return !obj->parentID.IsNonNull(); }
		virtual ObjectData* GetParent(ObjectData* obj) override { return context.GetFocusedChunk()->GetWorldObjectStatusByObjectId(obj->parentID).objectData; }
		virtual Eigen::Affine3f GetSelectionSpaceTransform(ObjectData* obj) override { return ObjectTransformDataToAffine3f(obj->transform); }
		virtual void SetSelectionSpaceTransform(ObjectData* obj, const Eigen::Affine3f& transform) override {
			UpdateAbsoluteTransform(transform, *obj);
			context.RecalculateDependentTransforms(obj);
		}
	};

	class MousePickingPhysicsBehaviorContext : public BehaviorContext<Context, MousePickingPhysicsBehavior<ObjectData*>> {
		using BehaviorContext::BehaviorContext;
		virtual const char* GetObjectName(ObjectData* obj) override { return obj->name; }
		virtual ObjectData* GetObjectForGameObject(GameObject* obj) override { return obj->GetWorldObjectStatus()->objectData; }
		virtual bool IsSelectable(hh::game::GameObject* obj) override {
			auto* focusedChunk = context.GetFocusedChunk();

			if (!focusedChunk)
				return false;

			auto* status = obj->GetWorldObjectStatus();

			return status != nullptr && focusedChunk->GetObjectIndexByObjectData(status->objectData) != -1;
		}
		virtual void GetFrustumResults(const Frustum& frustum, csl::ut::MoveArray<ObjectData*>& results) override {
			auto* focusedChunk = context.GetFocusedChunk();

			if (!focusedChunk)
				return;

			for (auto* layer : focusedChunk->GetLayers())
				for (auto* object : layer->GetResource()->GetObjects())
					if (frustum.Test(object->transform.position))
						results.push_back(object);
		}
	};
	
	class DeleteBehaviorContext : public BehaviorContext<Context, DeleteBehavior<ObjectData*>> {
		using BehaviorContext::BehaviorContext;
		virtual void DeleteObjects(const csl::ut::MoveArray<ObjectData*>& objects) override {
			auto* focusedChunk = context.GetFocusedChunk();

			for (auto* obj : objects)
				focusedChunk->Despawn(obj);

			focusedChunk->ShutdownPendingObjects();

			for (auto* obj : objects)
				for (auto* layer : focusedChunk->GetLayers())
					for (auto* object : layer->GetResource()->GetObjects())
						if (object == obj)
							layer->RemoveObjectData(obj);
		}
	};

	class ClipboardBehaviorContext : public BehaviorContext<Context, ClipboardBehavior<ObjectData*>> {
		using BehaviorContext::BehaviorContext;
		virtual ClipboardEntry<ObjectData*> CreateClipboardEntry(ObjectData* objData) override { return { context, objData }; }
		virtual ObjectData* CreateObject(const ClipboardEntry<ObjectData*>& entry) override { return context.CopyObjectForPlacement(entry.proto); }
	};

	class ObjectLocationVisual3DBehaviorContext : public BehaviorContext<Context, ObjectLocationVisual3DBehavior<ObjectData*>> {
		using BehaviorContext::BehaviorContext;
		virtual Eigen::Affine3f GetWorldTransform(ObjectData* object) const override { return ObjectTransformDataToAffine3f(object->transform); }
		virtual void GetInvisibleObjects(csl::ut::MoveArray<ObjectData*>& objects) const override {
			auto* focusedChunk = context.GetFocusedChunk();

			if (!focusedChunk)
				return;

			for (auto& status : focusedChunk->GetObjectStatuses()) {
				auto* gameObject = focusedChunk->GetGameObjectByObjectId(status.objectData->id);

				if (!gameObject || ((!gameObject->GetComponent<hh::gfx::GOCVisual>() || gameObject->GetComponent<hh::gfx::GOCVisual>() == gameObject->GetComponent<hh::gfx::GOCVisualDebugDraw>()) && !gameObject->GetComponent<app::gfx::GOCVisualGeometryInstance>()))
					objects.push_back(status.objectData);
			}
		}
	};

	LevelEditor::LevelEditor(csl::fnd::IAllocator* allocator) : OperationMode{ allocator } {
		AddPanel<SetObjectList>();
		AddPanel<ObjectDataInspector>();
		AddPanel<ObjectLibrary>();
		AddBehavior<SelectionBehavior<ObjectData*>>();
		AddBehavior<SelectionAabbBehavior<ObjectData*>, SelectionAabbBehaviorContext>();
		AddBehavior<SelectionTransformationBehavior<ObjectData*>, SelectionTransformationBehaviorContext>();
		AddBehavior<SelectionVisual3DBehavior>();
		AddBehavior<GizmoBehavior>();
		AddBehavior<MousePickingPhysicsBehavior<ObjectData*>, MousePickingPhysicsBehaviorContext>();
		AddBehavior<SelectionMousePickingBehavior<ObjectData*>>();
		AddBehavior<DeleteBehavior<ObjectData*>, DeleteBehaviorContext>();
		AddBehavior<ClipboardBehavior<ObjectData*>, ClipboardBehaviorContext>();
		AddBehavior<ObjectLocationVisual3DBehavior<ObjectData*>, ObjectLocationVisual3DBehaviorContext>();
		AddBehavior<GroundContextMenuBehavior<ObjectData*>>();
		AddBehavior<DebugCommentsVisualBehavior>();

		auto* gameManager = GameManager::GetInstance();

		gameManager->AddListener(this);

		if (auto* objWorld = gameManager->GetService<ObjectWorld>())
			objWorld->AddWorldListener(this);
	}

	LevelEditor::~LevelEditor() {

		auto* gameManager = GameManager::GetInstance();

		gameManager->RemoveListener(this);

		if (auto* objWorld = gameManager->GetService<ObjectWorld>())
			objWorld->RemoveWorldListener(this);
	} 

	void LevelEditor::ProcessAction(const ActionBase& action) {
		OperationMode::ProcessAction(action);

		switch (action.id) {
		case SelectionTransformationBehavior<ObjectData*>::SelectionTransformChangedAction::id: {
			auto& selection = GetBehavior<SelectionBehavior<ObjectData*>>()->GetSelection();

			GetContext().RecalculateDependentTransforms(selection);

			if (selection.size() == 1)
				GetContext().NotifyUpdatedObject(selection[0]);

			break;
		}
		case SelectionBehavior<ObjectData*>::SelectionChangedAction::id: {
			auto& changes = static_cast<const SelectionBehavior<ObjectData*>::SelectionChangedAction&>(action).payload;

			if (changes.previousSelection.size() == 1 && (changes.currentSelection.size() != 1 || changes.previousSelection[0] != changes.currentSelection[0]))
				GetContext().NotifyDeselectedObject(changes.previousSelection[0]);
			if (changes.currentSelection.size() == 1 && (changes.previousSelection.size() != 1 || changes.previousSelection[0] != changes.currentSelection[0]))
				GetContext().NotifySelectedObject(changes.currentSelection[0]);

			break;
		}
		case SetFocusedChunkAction::id: {
			auto* chunk = static_cast<const SetFocusedChunkAction&>(action).payload;

			if (GetContext().GetFocusedChunk() == chunk)
				return;

			GetContext().SetFocusedChunk(chunk);
			Dispatch(FocusedChunkChangedAction{});
			break;
		}
		case FocusedChunkChangedAction::id:
			GetBehavior<ClipboardBehavior<ObjectData*>>()->Clear();
			GetBehavior<SelectionBehavior<ObjectData*>>()->DeselectAll();
			break;
		case ChangingParametersAction::id:
			GetContext().ReloadActiveObjectParameters(GetBehavior<SelectionBehavior<ObjectData*>>()->GetSelection()[0]);
			break;
		case StopChangingParametersAction::id:
			GetContext().RespawnActiveObject(GetBehavior<SelectionBehavior<ObjectData*>>()->GetSelection()[0]);
			break;
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
		if (GetContext().GetFocusedChunk() != chunk)
			return;

		GetContext().ReleaseChunk();
		Dispatch(FocusedChunkChangedAction{});
	}
}
