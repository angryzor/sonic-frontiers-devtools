#pragma once
#include <optional>
#include <ui/Desktop.h>
#include <ui/operation-modes/OperationModeBehavior.h>
#include "ForwardDeclarations.h"
#include "MousePicking.h"

template<typename OpModeContext>
class PlacementBehavior : public OperationModeBehavior {
public:
	using Traits = PlacementBehaviorTraits<OpModeContext>;

private:
	Traits traits;
	bool placing{};

public:
	static constexpr unsigned int id = 13;
	virtual unsigned int GetId() override { return id; }

	using ObjectPlacedAction = Action<ActionId::OBJECT_PLACED, typename MousePickingBehavior<OpModeContext>::ObjectType>;
	using TogglePlaceModeAction = Action<ActionId::TOGGLE_PLACE_MODE>;

	PlacementBehavior(csl::fnd::IAllocator* allocator, OperationMode<OpModeContext>& operationMode) : OperationModeBehavior{ allocator, operationMode }, traits { operationMode.GetContext() } {}

	void Init() {
		Desktop::instance->BindShortcut<TogglePlaceModeAction>(ShortcutId::TOGGLE_PLACE_MODE);
	}

	void Deinit() {
		Desktop::instance->UnbindShortcut(ShortcutId::TOGGLE_PLACE_MODE);
	}
		
	void SetPlaceMode(bool placing) {
		this->placing = placing;
	}

	bool CanPlace() const {
		return traits.CanPlace();
	}

	bool IsInPlaceMode() const {
		return traits.CanPlace() && placing;
	}

	void Render() override {
		auto* mousePicking = operationMode.GetBehavior<MousePickingBehavior<OpModeContext>>();

		if (CanPlace() && (placing || ImGui::IsKeyDown(ImGuiKey_LeftShift) || ImGui::IsKeyDown(ImGuiKey_RightShift)) && mousePicking->locationPicked)
			Dispatch(ObjectPlacedAction{ traits.PlaceObject(mousePicking->pickedLocation) });
	}
};
