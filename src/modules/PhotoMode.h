#pragma once
#include "Module.h"

class PhotoMode : public Module, public hh::game::GameManagerListener {
	hh::game::InputManager* inputMgr{};
	hh::fnd::Reference<hh::game::InputComponent> inputComponent{};
	bool active{};
	INT64 perfFreq{};
	INT64 debounce{};

	void Register();
	void Unregister();
	bool HasRequestedPhotoMode() const;
public:
	PhotoMode(csl::fnd::IAllocator* allocator);
	~PhotoMode();

	virtual void Update() override;
	virtual void GameServiceAddedCallback(hh::game::GameService* gameService) override;
	virtual void GameServiceRemovedCallback(hh::game::GameService* gameService) override;
};
