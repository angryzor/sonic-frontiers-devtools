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
	static constexpr unsigned int id = 0x50484f54;
	static bool enabled;

	PhotoMode(csl::fnd::IAllocator* allocator);
	~PhotoMode();

	virtual unsigned int GetId() const override;
	virtual void Update() override;
	virtual void GameServiceAddedCallback(hh::game::GameService* gameService) override;
	virtual void GameServiceRemovedCallback(hh::game::GameService* gameService) override;
};
