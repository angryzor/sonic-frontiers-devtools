#pragma once
#include "operation-modes/OperationMode.h"
#include "StandaloneWindow.h"

class Desktop : public hh::fnd::BaseObject {
    hh::fnd::Reference<OperationMode> operationMode;
    hh::fnd::Reference<hh::gfnd::ResTexture> iconTexture;
    csl::ut::MoveArray<hh::fnd::Reference<StandaloneWindow>> windows{ GetAllocator() };
    hh::dbg::ViewerManager viewerManagerEmulator{ GetAllocator() };
    hh::fnd::Reference<hh::game::GameViewerContext> gameViewerCtx;
    hh::fnd::Reference<hh::game::ObjectViewerContext> objViewerCtx;
    hh::fnd::Reference<hh::physics::PhysicsViewerContext> physicsViewerCtx;
    bool prevPhysicsPickerMouseDown;
public:
    //hh::fnd::Reference<hh::game::MousePickingViewer> picker;
    hh::fnd::Reference<hh::physics::PhysicsMousePickingViewer> physicsPicker;
    //hh::fnd::Reference<hh::physics::PhysicsPickedObjectViewer> physicsObjPicker;

    hh::fnd::Reference<hh::fnd::ResourceLoader> resourceLoader;
    static Desktop* instance;

    ID3D11ShaderResourceView* iconView;
    Desktop(csl::fnd::IAllocator* allocator);
    ~Desktop();
    void Render();
    void AddStandaloneWindow(StandaloneWindow* window);
    void RemoveStandaloneWindow(StandaloneWindow* window);
    bool IsPickerMouseDown() const;
    bool IsPickerMouseClicked() const;
    bool IsPickerMouseReleased() const;
    hh::game::GameObject* GetPickedObject() const;
    csl::math::Vector3& GetPickedLocation() const;
    void SwitchToObjectInspectionMode();
    void SwitchToLevelEditorMode();
};
