#pragma once
#include <ui/operation-modes/OperationModeBehavior.h>
#include "MousePicking.h"

template<typename OpModeContext>
class GroundContextMenuBehavior : public OperationModeBehavior {
public:
	typename MousePickingBehaviorTraits<OpModeContext>::LocationType pickedLocation{ 0.0f, 0.0f, 0.0f };

	static constexpr unsigned int id = 12;
	virtual unsigned int GetId() override { return id; }

	using OperationModeBehavior::OperationModeBehavior;

	virtual void Render() override {
#ifdef DEVTOOLS_TARGET_SDK_rangers
		auto* mousePicking = operationMode.GetBehavior<MousePickingBehavior<OpModeContext>>();

		if (mousePicking->picked && mousePicking->mouseButton == ImGuiMouseButton_Right)
			if (auto* levelInfo = hh::game::GameManager::GetInstance()->GetService<app::level::LevelInfo>())
				if (auto* player = static_cast<app::player::Player*>(hh::fnd::MessageManager::GetInstance()->GetMessengerByHandle(levelInfo->GetPlayerObject(0))))
					if (auto* playerKine = player->GetComponent<app::player::GOCPlayerKinematicParams>()) {
						pickedLocation = mousePicking->pickedLocation;
						ImGui::OpenPopup("WorldContext");
					}


		if (ImGui::BeginPopup("WorldContext")) {
			if (auto* levelInfo = hh::game::GameManager::GetInstance()->GetService<app::level::LevelInfo>())
				if (auto* player = static_cast<app::player::Player*>(hh::fnd::MessageManager::GetInstance()->GetMessengerByHandle(levelInfo->GetPlayerObject(0))))
					if (auto* playerKine = player->GetComponent<app::player::GOCPlayerKinematicParams>())
						if (ImGui::Selectable("Teleport player"))
							playerKine->SetPosition({ pickedLocation.x(), pickedLocation.y(), pickedLocation.z(), 0.0f });
			ImGui::EndPopup();
		};
#endif
	}
};
