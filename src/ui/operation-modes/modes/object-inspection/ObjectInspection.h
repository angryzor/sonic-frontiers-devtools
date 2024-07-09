#pragma once
#include <ui/operation-modes/OperationMode.h>
#include <ui/operation-modes/behaviors/SelectionAabb.h>
#include <ui/operation-modes/behaviors/SelectionTransformation.h>
#include <ui/operation-modes/behaviors/MousePicking.h>
#include <ui/operation-modes/behaviors/Delete.h>
#include "Context.h"

namespace ui::operation_modes::modes::object_inspection {
    class ObjectInspection
        : public OperationMode<Context>
        , public hh::game::GameManagerListener
    {
    public:
        ObjectInspection(csl::fnd::IAllocator* allocator);
        ~ObjectInspection();

        virtual void GameObjectRemovedCallback(hh::game::GameManager* gameManager, hh::game::GameObject* gameObject) override;
    };
}
