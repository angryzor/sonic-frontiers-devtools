#include "PhotoMode.h"

using namespace hh::game;

bool PhotoMode::enabled{ true };

PhotoMode::PhotoMode(csl::fnd::IAllocator* allocator) : Module{ allocator } {
	::QueryPerformanceFrequency((LARGE_INTEGER*)&perfFreq);

	if (GameManager::GetInstance()->GetService<app::gfx::FxParamManager>()) {
		inputMgr = GameManager::GetInstance()->GetService<InputManager>();
		Register();
	}

	GameManager::GetInstance()->AddListener(this);
}

PhotoMode::~PhotoMode() {
	GameManager::GetInstance()->RemoveListener(this);

	if (inputMgr) {
		Unregister();
		inputMgr = nullptr;
	}
}

void PhotoMode::Register() {
	InputComponent::Config config{
		.internalPlayerInputIndex = 0,
		.objectLayer = (unsigned int)-1,
		.priority = 0,
		.actionMonitorCount = 1,
		.axisMonitorCount = 0,
		.unkMonitorCount = 0,
		.objectName = "DevToolsPhotoMode",
		.exclusive = false,
	};

	inputComponent = InputComponent::Create(GetAllocator(), config, inputMgr);
	inputComponent->MonitorActionMapping("DevToolsPhotoMode", 0, 0);
	inputComponent->SetListening(true);
	inputComponent->flags.set(static_cast<hh::game::InputComponent::Flags>(2));
	inputMgr->RegisterInputComponent(*inputComponent);
}

void PhotoMode::Unregister() {
	inputMgr->UnregisterInputComponent(*inputComponent);
	inputComponent = nullptr;
}

bool PhotoMode::HasRequestedPhotoMode() const {
	return inputComponent->actionMonitors[0].state & 512;
}

void PhotoMode::Update() {
	if (!enabled)
		return;

	if (!inputComponent)
		return;

	if (!HasRequestedPhotoMode())
		return;

	if (active) {
		DebugCameraManager::GetInstance()->DeactivateDebugCamera();
		GameApplication::GetInstance()->GetGameUpdater().flags.reset(GameUpdater::Flags::DEBUG_PAUSE);

		if (auto* cockpit = GameManager::GetInstance()->GetGameObject("UIGameCockpit"))
		if (auto* cockpitGocSprite = cockpit->GetComponent<hh::ui::GOCSprite>())
			cockpitGocSprite->flags.set(hh::ui::GOCSprite::Flag::ENABLED);
	}
	else {
		DebugCameraManager::GetInstance()->ActivateDebugCamera({ 1, 0, 1, 0 });
		GameApplication::GetInstance()->GetGameUpdater().flags.set(GameUpdater::Flags::DEBUG_PAUSE);

		if (auto* cockpit = GameManager::GetInstance()->GetGameObject("UIGameCockpit"))
		if (auto* cockpitGocSprite = cockpit->GetComponent<hh::ui::GOCSprite>())
			cockpitGocSprite->flags.reset(hh::ui::GOCSprite::Flag::ENABLED);
	}

	active = !active;
}

void PhotoMode::GameServiceAddedCallback(GameService* gameService) {
	if (gameService->staticClass == app::gfx::FxParamManager::GetClass()) {
		inputMgr = GameManager::GetInstance()->GetService<InputManager>();
		Register();
	}
}

void PhotoMode::GameServiceRemovedCallback(GameService* gameService) {
	if (gameService->staticClass == app::gfx::FxParamManager::GetClass()) {
		Unregister();
		inputMgr = nullptr;
	}
}

