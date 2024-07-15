#pragma once
#include <ui/operation-modes/OperationMode.h>
#include "Context.h"

namespace ui::operation_modes::modes::level_editor {
    class LevelEditor
        : public OperationMode<Context>
        , public hh::game::GameManagerListener
        , public hh::game::ObjectWorldListener
    {
    public:
        LevelEditor(csl::fnd::IAllocator* allocator, OperationModeHost& host);
        virtual ~LevelEditor();

        virtual void ProcessAction(const ActionBase& action) override;
        virtual void GameServiceAddedCallback(hh::game::GameService* gameService) override;
        virtual void GameServiceRemovedCallback(hh::game::GameService* gameService) override;
        virtual void WorldChunkRemovedCallback(hh::game::ObjectWorldChunk* chunk) override;

        void ClearChunkReferences();
    };
}
