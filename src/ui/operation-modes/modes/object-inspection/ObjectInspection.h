#pragma once
#include <ui/operation-modes/OperationMode.h>
#include "Context.h"

namespace ui::operation_modes::modes::object_inspection {
    class ObjectInspection
        : public OperationMode<Context>
        , public hh::game::GameManagerListener
    {
    public:
        ObjectInspection(csl::fnd::IAllocator* allocator);
        ~ObjectInspection();

        virtual void ProcessAction(const ActionBase& action) override;
        virtual void GameObjectRemovedCallback(hh::game::GameManager* gameManager, hh::game::GameObject* gameObject) override;
    };
}
