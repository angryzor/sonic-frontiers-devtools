#pragma once
#include <ui/operation-modes/OperationMode.h>
#include <ui/common/StandaloneWindow.h>

class Desktop : public hh::fnd::BaseObject {
    hh::fnd::Reference<hh::gfnd::ResTexture> iconTexture;
    csl::ut::MoveArray<hh::fnd::Reference<StandaloneWindow>> windows{ GetAllocator() };
    csl::ut::MoveArray<hh::physics::PhysicsQueryResult> pickerResults{ GetAllocator() };
    //hh::dbg::ViewerManager viewerManagerEmulator{ GetAllocator() };
    //hh::fnd::Reference<hh::game::GameViewerContext> gameViewerCtx;
    //hh::fnd::Reference<hh::game::ObjectViewerContext> objViewerCtx;
    //hh::fnd::Reference<hh::physics::PhysicsViewerContext> physicsViewerCtx;
    //hh::physics::PhysicsQueryResult lastRaycastResult;
    bool pickerClicked{ false };
    bool locationPicked{ false };
    csl::ut::MoveArray<hh::game::GameObject*> pickedObjects{ GetAllocator() };
    csl::math::Vector3 pickedLocation;
public:
    static bool selectionColliderFilters[32][32];
    hh::fnd::Reference<OperationMode> operationMode;
    //hh::fnd::Reference<hh::game::MousePickingViewer> picker;
    //hh::fnd::Reference<hh::physics::PhysicsMousePickingViewer> physicsPicker;
    //hh::fnd::Reference<hh::physics::PhysicsPickedObjectViewer> physicsObjPicker;

    hh::fnd::Reference<hh::fnd::ResourceLoader> resourceLoader;
    static Desktop* instance;

    ID3D11ShaderResourceView* iconView;
    Desktop(csl::fnd::IAllocator* allocator);
    ~Desktop();
    void Render();
    void AddStandaloneWindow(StandaloneWindow* window);
    void RemoveStandaloneWindow(StandaloneWindow* window);
    void HandleMousePicking();
    //bool IsPickerMouseDown() const;
    //bool IsPickerMouseClicked() const;
    bool IsPickerMouseReleased() const;
    const csl::ut::MoveArray<hh::game::GameObject*>& GetPickedObjects() const;
    const csl::math::Vector3* GetPickedLocation() const;
    void SwitchToObjectInspectionMode();
    void SwitchToLevelEditorMode();
};
