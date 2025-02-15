#include "DvSceneEditor.h"
#include "Tree.h"
#include "SceneSettings.h"
#include "node-inspector/NodeInspector.h"
#include "timeline/Timeline.h"

#include "Behaviors.h"
#include <ui/common/editors/Reflection.h>

namespace ui::operation_modes::modes::dvscene_editor
{
	DvSceneEditor::DvSceneEditor(csl::fnd::IAllocator* allocator, OperationModeHost& host) : OperationMode{ allocator, host }
	{
		GetContext().timelineFalloff = 3.0;
		AddPanel<Tree>();
        AddPanel<SceneSettings>();
        AddPanel<NodeInspector>();
        AddPanel<Timeline>();
		AddBehavior<SelectionBehavior>();
		AddBehavior<SelectionAabbBehavior>();
		AddBehavior<SelectionTransformationBehavior>();
		AddBehavior<GizmoBehavior>();
		AddBehavior<DeleteBehavior>();
		auto* gameManager = hh::game::GameManager::GetInstance();
		gameManager->AddListener(this);
#ifdef DEVTOOLS_TARGET_SDK_rangers
		if(auto* diEvtMgr = gameManager->GetService<hh::dv::DiEventManager>()) {
			diEvtMgr->AddListener(this);
			listenerAdded = true;
		}
#endif
	}

	DvSceneEditor::~DvSceneEditor(){
		auto* gameManager = hh::game::GameManager::GetInstance();
		gameManager->RemoveListener(this);
#ifdef DEVTOOLS_TARGET_SDK_rangers
		if(auto* diEvtMgr = gameManager->GetService<hh::dv::DiEventManager>()) {
			diEvtMgr->RemoveListener(this);
			listenerAdded = false;
		}
#endif
	}

	void DvSceneEditor::GameServiceAddedCallback(hh::game::GameService* gameService) {
#ifdef DEVTOOLS_TARGET_SDK_rangers
		if(!listenerAdded)
			if(gameService->staticClass == hh::dv::DiEventManager::GetClass()){
				auto* diEvtMgr = reinterpret_cast<hh::dv::DiEventManager*>(gameService);
				diEvtMgr->AddListener(this);
				listenerAdded = true;
			}
#endif
	}
	void DvSceneEditor::GameServiceRemovedCallback(hh::game::GameService* gameService) {
#ifdef DEVTOOLS_TARGET_SDK_rangers
		if(listenerAdded)
			if(gameService->staticClass == hh::dv::DiEventManager::GetClass()){
				auto* diEvtMgr = reinterpret_cast<hh::dv::DiEventManager*>(gameService);
				diEvtMgr->RemoveListener(this);
				listenerAdded = false;
			}
#endif
	}

	void DvSceneEditor::DSCL_UnkFunc7() {
		GetContext().goDVSC = nullptr;
	}
}
