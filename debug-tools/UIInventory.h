#pragma once

namespace mods::inventory::ui {
    using namespace hh::game;
    using namespace hh::ui;

    struct InventoryItem {
        const char* id;
        const char* localizationTag;
        unsigned int quantity;
    };

    class UIInventory : public GameObject, public hh::hid::ActiveDeviceManagerListener
    {
        typedef app_cmn::fsm::GOCTinyFsm2<UIInventory> GOCFsm;

        LayerController* layerController{};
        LayerController* refBtn{};
        LayerController* refPcBtnT{};
        LayerController* refPcBtnB{};
        GOCInput* gocInput{};
        csl::ut::MoveArray<InventoryItem> inventoryItems{ GetAllocator() };

        bool waitingBtnUp{ false };
        bool firstOpen{ true };

        static GameObject* Instantiate(csl::fnd::IAllocator* pAllocator);
        static const GameObjectClass gameObjectClass;
    public:
        static const GameObjectClass& GetClass();

        UIInventory(csl::fnd::IAllocator* pAllocator);

        //bool fUnk2(hh::fnd::Message& message) override;
        virtual void Initialize(GameManager* gameManager) override;
        virtual void Dispose() override;
        virtual void Update(uint64_t unkParam, uint64_t unkParam2) override;

        GOCFsm::State_t State_Closed(const GOCFsm::Event_t& event);
        GOCFsm::State_t State_Open(const GOCFsm::Event_t& event);

        virtual void OnDeviceChange() override;
    };
}
