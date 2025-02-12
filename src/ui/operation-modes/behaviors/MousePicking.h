#pragma once
#include <utilities/math/Ray.h>
#include <utilities/math/Frustum.h>
#include <utilities/math/MathUtils.h>
#include <ui/operation-modes/OperationModeBehavior.h>
#include "Zoom.h"
#include "ForwardDeclarations.h"

template<typename OpModeContext>
class MousePickingBehavior : public OperationModeBehavior {
public:
	using Traits = MousePickingBehaviorTraits<OpModeContext>;
	using ObjectType = typename Traits::ObjectType;
	using LocationType = typename Traits::LocationType;

	csl::ut::MoveArray<ObjectType> pickedObjects{ GetAllocator() };
	ImGuiMouseButton mouseButton{};
	LocationType pickedLocation{};
	bool locationPicked{};
	bool picked{};

	static constexpr unsigned int id = 3;
	virtual unsigned int GetId() override { return id; }
	//struct Traits {
	//	virtual void GetAllClickResults(const ImVec2& position, csl::ut::MoveArray<ObjectType>& results) = 0;
	//	virtual void GetBestClickResult(const ImVec2& position, csl::ut::MoveArray<ObjectType>& results, V& location, bool& pickedLocation) = 0;
	//	virtual void GetDragResults(const ImVec2& start, const ImVec2 end, csl::ut::MoveArray<ObjectType>& results) = 0;
	//	virtual const char* GetObjectName(T object) = 0;
	//};

protected:
	Traits traits;
	csl::ut::MoveArray<ObjectType> intermediateResults{ this->GetAllocator() };
	ImGuiMouseButton intermediateMouseButton{};
	bool draggingPrevFrame{};

public:
	struct LocationPickInfo {
		LocationType location;
		ImGuiMouseButton button;
	};

	using ObjectsPickedAction = Action<ActionId::OBJECTS_PICKED, ImGuiMouseButton>;
	using LocationPickedAction = Action<ActionId::LOCATION_PICKED, LocationPickInfo>;

	MousePickingBehavior(csl::fnd::IAllocator* allocator, OperationMode<OpModeContext>& operationMode)
		: OperationModeBehavior{ allocator, operationMode }, traits{ operationMode.GetContext() } {}

	virtual void Render() override {
		this->picked = false;
		this->locationPicked = false;
		this->pickedObjects.clear();

		if (this->operationMode.CanTakeMouseControl(this)) {
			if (this->operationMode.IsDragging(this))
				HandleDragSelect();
			else if (!draggingPrevFrame && operationMode.IsMouseOverSceneWindow()) {
				if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
					this->operationMode.BeginSingleFrameExclusiveMouseControl(this);
					HandleClickSelect(ImGuiMouseButton_Left);
				}
				else if (ImGui::IsMouseReleased(ImGuiMouseButton_Right)) {
					this->operationMode.BeginSingleFrameExclusiveMouseControl(this);
					HandleClickSelect(ImGuiMouseButton_Right);
				}
			}

			this->operationMode.ToggleDragging(this);
		}

		draggingPrevFrame = ImGui::IsMouseDragging(ImGuiMouseButton_Left) || ImGui::IsMouseDragging(ImGuiMouseButton_Right);

		HandlePopupSelect();
	}

	void HandleDragSelect() {
		auto* zoomBehavior = operationMode.GetBehavior<ZoomBehavior>();
		auto zoom = zoomBehavior == nullptr ? 1.0f : zoomBehavior->zoomLevel;
		auto mouseScreenStartAbs = ImGui::GetMousePos() - ImGui::GetMouseDragDelta();
		auto mouseScreenEndAbs = ImGui::GetMousePos();
		auto mouseScreenMinAbs = ImVec2(std::min(mouseScreenStartAbs.x, mouseScreenEndAbs.x), std::min(mouseScreenStartAbs.y, mouseScreenEndAbs.y));
		auto mouseScreenMaxAbs = ImVec2(std::max(mouseScreenStartAbs.x, mouseScreenEndAbs.x), std::max(mouseScreenStartAbs.y, mouseScreenEndAbs.y));
		auto mouseMin = (mouseScreenMinAbs - ImGui::GetWindowContentRegionMin() - ImGui::GetWindowPos()) / zoom;
		auto mouseMax = (mouseScreenMaxAbs - ImGui::GetWindowContentRegionMin() - ImGui::GetWindowPos()) / zoom;

		if (operationMode.BeginOverlayWindow()) {
			ImGui::GetWindowDrawList()->AddRectFilled(mouseScreenStartAbs, mouseScreenEndAbs, 0x40FFFFFF);
		}
		operationMode.EndOverlayWindow();

		traits.GetDragResults(mouseMin, mouseMax, this->pickedObjects);

		this->mouseButton = ImGuiMouseButton_Left;
		this->picked = true;

		this->Dispatch(ObjectsPickedAction{ ImGuiMouseButton_Left });
	}

	void HandleClickSelect(ImGuiMouseButton button) {
		auto* zoomBehavior = operationMode.GetBehavior<ZoomBehavior>();
		auto zoom = zoomBehavior == nullptr ? 1.0f : zoomBehavior->zoomLevel;
		auto mousePos = (ImGui::GetMousePos() - ImGui::GetWindowContentRegionMin() - ImGui::GetWindowPos()) / zoom;

		if (ImGui::IsKeyDown(ImGuiKey_LeftAlt) || ImGui::IsKeyDown(ImGuiKey_RightAlt)) {
			this->intermediateResults.clear();

			traits.GetAllClickResults(mousePos, this->intermediateResults);

			if (intermediateResults.size() > 0) {
				intermediateMouseButton = button;
				ImGui::OpenPopup("Picker results");
				return;
			}
		}
		else {
			traits.GetBestClickResult(mousePos, this->pickedObjects, this->pickedLocation, this->locationPicked);
		}

		this->mouseButton = button;
		this->picked = true;

		this->Dispatch(ObjectsPickedAction{ button });

		if (this->locationPicked && this->pickedObjects.size() == 0)
			this->Dispatch(LocationPickedAction{ { this->pickedLocation, button } });
	}

	void HandlePopupSelect() {
		if (ImGui::BeginPopup("Picker results")) {
			for (auto& object : intermediateResults) {
				ImGui::PushID(&object);
				if (ImGui::Selectable(traits.GetObjectName(object))) {
					this->pickedObjects.push_back(object);
					this->mouseButton = intermediateMouseButton;
					this->picked = true;

					this->Dispatch(ObjectsPickedAction{ intermediateMouseButton });
				}
				ImGui::PopID();
			}
			ImGui::EndPopup();
		}
	}
};
