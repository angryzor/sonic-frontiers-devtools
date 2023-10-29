#include "Pch.h"
#include "UIInventory.h"
#include <rangers-api/bootstrap.h>

namespace mods::inventory::ui {
	using namespace hh::ut;
	using namespace hh::game;
	using namespace hh::ui;

	const GameObjectClass UIInventory::gameObjectClass{
		"UIInventory",
		"UIInventory",
		0,
		sizeof(UIInventory),
		&UIInventory::Instantiate,
		0,
		0,
		0,
		0,
		nullptr,
		nullptr
	};

	UIInventory::UIInventory(csl::fnd::IAllocator* pAllocator) : GameObject{ pAllocator } {
		SetLayer(19);
		SetForceComponentsFlag(ComponentType::VISIBLE, 1);
	}

	const GameObjectClass& UIInventory::GetClass() {
		return gameObjectClass;
	}

	GameObject* UIInventory::Instantiate(csl::fnd::IAllocator* pAllocator) {
		return new (pAllocator) UIInventory{ pAllocator };
	}

	void UIInventory::Initialize(GameManager* gameManager) {
		inventoryItems.push_back({ "inventory_shield", "inventory_shield", 2 });
		inventoryItems.push_back({ "inventory_thunder_shield", "inventory_thunder_shield", 3 });
		inventoryItems.push_back({ "inventory_flame_shield", "inventory_flame_shield", 23 });
		inventoryItems.push_back({ "inventory_aqua_shield", "inventory_aqua_shield", 5 });
		inventoryItems.push_back({ "inventory_triple_damage", "inventory_triple_damage", 6 });
		inventoryItems.push_back({ "inventory_speed_boost", "inventory_speed_boost", 4 });
		inventoryItems.push_back({ "inventory_invulnerability", "inventory_invulnerability", 12 });
		inventoryItems.push_back({ "inventory_super_form", "inventory_super_form", 1 });

		auto gocSprite = InstantiateComponent<GOCSprite>();
		GOCSprite::Config gocSpriteConfig;
		gocSpriteConfig.name = "UIInventory";
		gocSpriteConfig.unk1 = 7;
		gocSpriteConfig.unk2 = 10;
		gocSpriteConfig.unk3 = 2;
		gocSpriteConfig.unk4 = 0;
		gocSpriteConfig.unk5 = 2;
		gocSpriteConfig.unk6 = 0;
		gocSpriteConfig.unk7 = 0;
		gocSpriteConfig.projectResource = ResSurfRideProject::Load("ui_cockpitindicator_bgm");
		gocSprite->Initialize(gocSpriteConfig);
		AttachComponent(*gocSprite);

		gocInput = InstantiateComponent<GOCInput>();

		GOCInput::Config gocInputConfig;
		gocInputConfig.unk1 = true;
		gocInputConfig.inputComponentInternalPlayerInputIndex = 0;
		gocInputConfig.inputComponentPriority = 5;
		gocInputConfig.inputComponentActionMonitorCount = 3;
		gocInputConfig.inputComponentAxisMonitorCount = 0;
		gocInputConfig.inputComponentUnknownMonitorCount = 0;
		gocInputConfig.inputComponentUnk7 = 0;
		gocInput->Initialize(gocInputConfig);
		AttachComponent(*gocInput);

		gocInput->GetInputComponent()->MonitorActionMapping("StopSlot", 0, false);
		gocInput->GetInputComponent()->MonitorActionMapping("GotoPractice", 1, false);
		gocInput->GetInputComponent()->MonitorActionMapping("UIReopenWindow", 2, false);

		auto gocUIComposition = InstantiateComponent<GOCUIComposition>();

		GOCUIComposition::Config gocUICompositionConfig;
		gocUICompositionConfig.unk1 = 0;
		gocUICompositionConfig.unk2 = 0;
		gocUICompositionConfig.unk3 = 0;
		gocUIComposition->Initialize(gocUICompositionConfig);
		AttachComponent(*gocUIComposition);

		gocUIComposition->SetProject(gocSprite->GetProject());
		gocUIComposition->DoSomething();

		layerController = gocSprite->GetLayerController("ui_cockpitindicator_bgm", "info_bgm", 0);

		refBtn = layerController->GetChildLayer("ref_btn", 1);
		LinkActionToUIKey(refBtn, "info_bgm/ref_btn/cnt_key/sysf_key", "UIReopenWindow", nullptr);
		refBtn->SetVisibility(true);

		refPcBtnT = layerController->GetChildLayer("ref_pc_btn_t", 2);
		LinkActionToUIKey(refPcBtnT, "info_bgm/ref_pc_btn_t/cnt_key/sysf_key", "StopSlot", nullptr);
		refPcBtnT->SetVisibility(true);

		refPcBtnB = layerController->GetChildLayer("ref_pc_btn_b", 3);
		LinkActionToUIKey(refPcBtnB, "info_bgm/ref_pc_btn_b/cnt_key/sysf_key", "GotoPractice", nullptr);
		refPcBtnB->SetVisibility(true);

		layerController->UnkFunc1("obj_pc_btn", true);

		auto listView = static_cast<UIListViewElement*>(GetComponent<GOCUIComposition>()->FindElement("lv_bgm"));
		listView->Reset();
		listView->NUnkFunc1(-1, 0, 1);

		int i = 0;
		for (auto& invItem : inventoryItems) {
			// Should use hh::ut::TempString instead
			char qtyStr[128]{};
			snprintf(qtyStr, sizeof(qtyStr), "%d", invItem.quantity);

			auto* item = new (GetAllocator()) UIListViewItem(GetAllocator(), 0);
			item->SetTextElementLocalizationVariable("quantity", qtyStr);
			item->SetTextElementLocalizationTag(invItem.localizationTag);
			listView->AddItem(item);

			listView->NUnkFunc1(i, 0, 1);

			listView->GetItemLayerController(item)->StartAnimation("switch", static_cast<float>(i++));
		}

		GOCFsm::Config gocTinyFsmConfig;
		gocTinyFsmConfig.initialState = GOCFsm::State_t::CreateDelegate(&UIInventory::State_Closed);
		gocTinyFsmConfig.setFlag3 = false;
		gocTinyFsmConfig.targetObject = nullptr;
		gocTinyFsmConfig.unk1 = 0;
		gocTinyFsmConfig.unk2 = 0;
		gocTinyFsmConfig.setGOCFlag0 = true;
		gocTinyFsmConfig.setGOCFlag1 = false;
		gocTinyFsmConfig.setGOCFlag2 = false;
		gocTinyFsmConfig.setFlag4 = false;

		auto gocTinyFsm = InstantiateComponent<GOCFsm>();
		gocTinyFsm->Initialize(gocTinyFsmConfig);
		AttachComponent(*gocTinyFsm);
		(*rangersapi::bootstrap::GetAddress(&hh::hid::DeviceManagerWin32::instance))->GetActiveDeviceManager()->RegisterActiveDeviceManagerListener(*this);
	}

	void UIInventory::Dispose() {
		(*rangersapi::bootstrap::GetAddress(&hh::hid::DeviceManagerWin32::instance))->GetActiveDeviceManager()->UnregisterActiveDeviceManagerListener(*this);
	}

	void UIInventory::Update(uint64_t unkParam1, uint64_t unkParam2) {
	}

	void UIInventory::OnDeviceChange()
	{
		LinkActionToUIKey(refBtn, "info_bgm/ref_btn/cnt_key/sysf_key", "UIReopenWindow", nullptr);
		LinkActionToUIKey(refPcBtnT, "info_bgm/ref_pc_btn_t/cnt_key/sysf_key", "StopSlot", nullptr);
		LinkActionToUIKey(refPcBtnB, "info_bgm/ref_pc_btn_b/cnt_key/sysf_key", "GotoPractice", nullptr);
	}

	UIInventory::GOCFsm::State_t UIInventory::State_Closed(const UIInventory::GOCFsm::Event_t& event) {
		switch (event.signal) {
		case TinyFsmSignal::ENTER: {
			if (firstOpen) {
				firstOpen = false;
				const char* animations[]{ "in", "loop" };
				layerController->QueueAnimations({ animations, animations + 2 });
			}
			else {
				const char* animations[]{ "loop" };
				layerController->QueueAnimations({ animations, animations + 1 });
			}

			return UIInventory::GOCFsm::State_t::CreateHandled();
		}
		case TinyFsmSignal::LEAVE: {
			const char* animations[]{ "out" };
			layerController->QueueAnimations({ animations, animations + 1 });

			return UIInventory::GOCFsm::State_t::CreateHandled();
		}
		case TinyFsmSignal::UPDATE:
			if (gocInput->GetInputComponent()->actionMonitors[2].state & 0x200) {
				waitingBtnUp = true;
			} else if (waitingBtnUp && (gocInput->GetInputComponent()->actionMonitors[2].state & 0x400)) {
				waitingBtnUp = false;

				auto gocFsm = GetComponent<GOCFsm>();

				gocFsm->Transition(GOCFsm::State_t::CreateDelegate(&UIInventory::State_Open));
			}
			return UIInventory::GOCFsm::State_t::CreateHandled();
		default:
			return UIInventory::GOCFsm::State_t::CreateUnhandled();
		}
	}
	UIInventory::GOCFsm::State_t UIInventory::State_Open(const UIInventory::GOCFsm::Event_t& event) {
		switch (event.signal) {
		case TinyFsmSignal::ENTER: {
			const char* animations[]{ "act_in", "act_loop" };
			layerController->QueueAnimations({ animations, animations + 2 });

			GetComponent<GOCUIComposition>()->SetCurrentGroup("cnt_list");

			return UIInventory::GOCFsm::State_t::CreateHandled();
		}
		case TinyFsmSignal::LEAVE: {
			GetComponent<GOCUIComposition>()->SetCurrentGroup(nullptr);

			const char* animations[]{ "act_out" };
			layerController->QueueAnimations({ animations, animations + 1 });

			return UIInventory::GOCFsm::State_t::CreateHandled();
		}
		case TinyFsmSignal::UPDATE:
			if (gocInput->GetInputComponent()->actionMonitors[2].state & 0x200) {
				waitingBtnUp = true;
			}
			else if (waitingBtnUp && (gocInput->GetInputComponent()->actionMonitors[2].state & 0x400)) {
				waitingBtnUp = false;

				auto gocFsm = GetComponent<GOCFsm>();

				gocFsm->Transition(GOCFsm::State_t::CreateDelegate(&UIInventory::State_Closed));
			}
			return UIInventory::GOCFsm::State_t::CreateHandled();
		default:
			return UIInventory::GOCFsm::State_t::CreateUnhandled();
		}
	}
}
