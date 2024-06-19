#pragma once
#include <ui/operation-modes/OperationMode.h>
#include <ui/common/StandaloneWindow.h>
#include <utilities/CompatibleObject.h>

class Desktop : public CompatibleObject {
    hh::fnd::Reference<hh::gfnd::ResTexture> iconTexture{};
    csl::ut::MoveArray<hh::fnd::Reference<StandaloneWindow>> windows{ GetAllocator() };
    csl::ut::MoveArray<hh::fnd::Reference<StandaloneWindow>> windowsThatOpened{ GetAllocator() };
    csl::ut::MoveArray<hh::physics::PhysicsQueryResult> pickerResults{ GetAllocator() };
    bool pickerClicked{ false };
    bool locationPicked{ false };
    csl::ut::MoveArray<hh::game::GameObject*> pickedObjects{ GetAllocator() };
    csl::math::Vector3 pickedLocation;
    void AddStandaloneWindow(StandaloneWindow* window);
    void RemoveStandaloneWindow(StandaloneWindow* window);
public:
    static bool selectionColliderFilters[32][32];
    hh::fnd::Reference<OperationMode> operationMode{};
    hh::fnd::Reference<hh::fnd::ResourceLoader> resourceLoader{};
    static Desktop* instance;

    ID3D11ShaderResourceView* iconView;
    Desktop(csl::fnd::IAllocator* allocator);
    ~Desktop();
    void Render();
    void RenderOverlayWindow();
    void OpenStandaloneWindow(StandaloneWindow* window);
    void HandleMousePicking();
    bool IsPickerMouseReleased() const;
    const csl::ut::MoveArray<hh::game::GameObject*>& GetPickedObjects() const;
    const csl::math::Vector3* GetPickedLocation() const;
    void SwitchToObjectInspectionMode();
    void SwitchToLevelEditorMode();
    void SwitchToSurfRideEditorMode();
};
