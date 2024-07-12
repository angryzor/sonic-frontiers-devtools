#include "ObjectInspection.h"
#include "ObjectList.h"
#include "ObjectInspector.h"
#include "Behaviors.h"

namespace ui::operation_modes::modes::object_inspection {
	using namespace hh::fnd;
	using namespace hh::game;

	ObjectInspection::ObjectInspection(csl::fnd::IAllocator* allocator) : OperationMode{ allocator } {
		AddPanel<ObjectList>();
		AddPanel<ObjectInspector>();
		AddBehavior<SelectionBehavior>();
		AddBehavior<SelectionAabbBehavior>();
		AddBehavior<SelectionTransformationBehavior>();
		AddBehavior<SelectionVisual3DBehavior>();
		AddBehavior<GizmoBehavior>();
		AddBehavior<MousePickingBehavior>();
		AddBehavior<SelectionMousePickingBehavior>();
		AddBehavior<DeleteBehavior>();
		AddBehavior<GroundContextMenuBehavior>();
		AddBehavior<DebugCommentsVisualBehavior>();

		hh::game::GameManager::GetInstance()->AddListener(this);
	}

	ObjectInspection::~ObjectInspection()
	{
		hh::game::GameManager::GetInstance()->RemoveListener(this);
	}

	void ObjectInspection::ProcessAction(const ActionBase& action)
	{
		OperationMode::ProcessAction(action);

		switch (action.id)
		{
		case FocusGameObjectAction::id:
			GetBehavior<SelectionBehavior<Context>>()->Select(static_cast<const FocusGameObjectAction&>(action).payload);
		}
	}

	void ObjectInspection::GameObjectRemovedCallback(hh::game::GameManager* gameManager, hh::game::GameObject* gameObject)
	{
		auto* selectionBehavior = GetBehavior<SelectionBehavior<Context>>();

		if (selectionBehavior->IsSelected(gameObject))
			selectionBehavior->Deselect(gameObject);
	}
}
