#pragma once
#include <ui/operation-modes/OperationMode.h>
#include "Context.h"

namespace ui::operation_modes::modes::dvscene_editor
{
	class DvSceneEditor : 
		public OperationMode<Context>,
		public hh::game::GameManagerListener,
		public hh::dv::DvSceneControlListener
	{
	public:
		DvSceneEditor(csl::fnd::IAllocator* allocator, OperationModeHost& host);
		~DvSceneEditor();
		
		bool listenerAdded = false;

		virtual void GameServiceAddedCallback(hh::game::GameService* gameService) override;
		virtual void GameServiceRemovedCallback(hh::game::GameService* gameService) override;
		virtual void DSCL_UnkFunc7() override;
	};
}
