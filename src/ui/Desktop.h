#pragma once
#include <ui/operation-modes/OperationMode.h>
#include <ui/common/StandaloneWindow.h>
#include <utilities/CompatibleObject.h>
#include "Action.h"
#include "Shortcuts.h"

class Desktop : public CompatibleObject, OperationModeHost {
    struct ShortcutBinding {
        ShortcutId shortcutId;
        ActionId actionId;
    };

    hh::fnd::Reference<hh::gfnd::ResTexture> iconTexture{};
    csl::ut::MoveArray<hh::fnd::Reference<StandaloneWindow>> windows{ GetAllocator() };
    csl::ut::MoveArray<hh::fnd::Reference<StandaloneWindow>> windowsThatOpened{ GetAllocator() };
    csl::ut::MoveArray<hh::physics::PhysicsQueryResult> pickerResults{ GetAllocator() };
    bool pickerClicked{ false };
    bool locationPicked{ false };
    csl::ut::MoveArray<hh::game::GameObject*> pickedObjects{ GetAllocator() };
    csl::ut::MoveArray<ShortcutBinding> boundShortcuts{ GetAllocator() };
    csl::math::Vector3 pickedLocation;
    void AddStandaloneWindow(StandaloneWindow* window);
    void RemoveStandaloneWindow(StandaloneWindow* window);
public:
    static bool selectionColliderFilters[32][32];
    hh::fnd::Reference<OperationModeBase> operationMode{};
    hh::fnd::Reference<hh::fnd::ResourceLoader> resourceLoader{};
    static Desktop* instance;

    ID3D11ShaderResourceView* iconView;
    Desktop(csl::fnd::IAllocator* allocator);
    ~Desktop();
    void Render();
    void RenderOverlayWindow();
    void OpenStandaloneWindow(StandaloneWindow* window);
    
    template<typename T>
    void SwitchToOperationMode() {
        if (operationMode != nullptr)
            operationMode->DeinitBehaviors();

        operationMode = new (GetAllocator()) T{ GetAllocator(), *this };

        operationMode->InitBehaviors();
    }
    void Dispatch(const ActionBase& action);

    template<typename T, typename = std::enable_if_t<std::is_same_v<typename T::PayloadType, EmptyActionPayload>>> void BindShortcut(ShortcutId shortcutId) {
        boundShortcuts.push_back({ shortcutId, T::id });
    }

    void UnbindShortcut(ShortcutId shortcutId);
    void HandleShortcuts();

    // Inherited via OperationModeHost
    void RenderPanel(PanelBase& panel) override;
    bool BeginSceneWindow(PanelBase& panel) override;
    void EndSceneWindow() override;
    bool IsMouseOverSceneWindow() override;
};
