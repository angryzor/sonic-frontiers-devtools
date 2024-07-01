#include "ObjectData.h"
#include <ui/common/inputs/Basic.h>
#include <ui/common/editors/Basic.h>
#include <ui/common/editors/Reflection.h>
#include <ui/common/viewers/Basic.h>
#include <ui/common/viewers/ObjectData.h>
#include <reflection/ReflectiveOperations.h>
#include <utilities/ObjectDataUtils.h>
#include "ObjectData.h"

using namespace hh::game;

#ifndef DEVTOOLS_TARGET_SDK_wars
//ComponentData* CreateComponentData(const GOComponentRegistry::GOComponentRegistryItem* gocRegItem) {
//	auto* allocator = hh::fnd::MemoryRouter::GetModuleAllocator();
//
//	size_t spawnerDataSize = gocRegItem->rflClass->GetSizeInBytes();
//	// We're placing both in 1 block here because TerminateObjectData only calls free on the ComponentData.
//	// If we had 2 allocations the spawner data would not be freed.
//	void* buf = new (std::align_val_t(16), allocator) char[sizeof(ComponentData) + spawnerDataSize];
//	void* spawnerData = reinterpret_cast<void*>(reinterpret_cast<size_t>(buf) + sizeof(ComponentData));
//
//	if (!strcmp(gocRegItem->name, "Model"))
//		new (spawnerData) GOCVisualModelSpawner{ allocator };
//	else if (!strcmp(gocRegItem->name, "SimpleAnimation"))
//		new (spawnerData) GOCSimpleAnimationSpawner{ allocator };
//	else if (!strcmp(gocRegItem->name, "RangeSpawning"))
//		new (spawnerData) GOCActivatorSpawner{};
//	else
//		assert(false);
//
//	return new (buf) ComponentData{ gocRegItem->name, spawnerData, spawnerDataSize };
//}
#endif

bool Editor(const char* label, hh::game::ObjectTransformData& obj)
{
	bool edited{};
	ImGui::PushID(label);
	ImGui::BeginGroup();
	edited |= Editor("Position", obj.position);
	edited |= DragScalar("Rotation", obj.rotation, 0.005f);
	ImGui::EndGroup();
	ImGui::PopID();
	return edited;
}

bool Editor(const char* label, hh::game::ObjectData& obj)
{
	bool edited{};
	auto* objSystem = GameObjectSystem::GetInstance();

	Viewer("Id", obj.id);
#ifdef DEVTOOLS_TARGET_SDK_wars
	Viewer("Name", obj.name);
#else
	edited |= Editor("Name", obj.name);
#endif
	ImGui::Text("Class: %s", obj.gameObjectClass);
	Viewer("Parent object id", obj.parentID);

	ImGui::SeparatorText("Transform");
	Viewer("Transform", obj.transform);

	ObjectTransformData localTransformData{ obj.localTransform };

	ImGui::SeparatorText("Local transform");
	bool editedTransform = Editor("Local transform", localTransformData);
	edited |= editedTransform;

	if (editedTransform)
		UpdateLocalTransform(ObjectTransformDataToAffine3f(localTransformData), obj);

#ifdef DEVTOOLS_TARGET_SDK_wars
	ImGui::SeparatorText("Component configuration");
	for (auto* componentConfig : obj.componentData) {
		if (ImGui::CollapsingHeader(componentConfig->type)) {
			ImGui::Text("Configuration:");
			if (!strcmp(componentConfig->type, "RangeSpawning"))
				edited |= ReflectionEditor("Component properties", componentConfig->data, &GOCActivatorSpawner::rflClass, true);
		}
	}
#else
	for (auto* componentConfig : obj.componentData) {
		auto* gocInfo = objSystem->goComponentRegistry->GetComponentInformationByName(componentConfig->type);
		if (ImGui::CollapsingHeader(gocInfo->name)) {
			ImGui::Text("Component type: %s", gocInfo->componentClass->category);
			ImGui::Text("Configuration:");
			edited |= ReflectionEditor("Component properties", componentConfig->data, gocInfo->rflClass, true);
		}
	}

	//if (ImGui::Button("Add component..."))
	//	ImGui::OpenPopup("ComponentConfigSelection");

	//if (ImGui::BeginPopup("ComponentConfigSelection")) {
	//	for (auto* gocRegItem : objSystem->goComponentRegistry->GetComponents()) {
	//		if (ImGui::Selectable(gocRegItem->name)) {
	//			auto* allocator = hh::fnd::MemoryRouter::GetModuleAllocator();

	//			if (!obj.flags.test(ObjectData::Flag::COMPONENT_DATA_NEEDS_TERMINATION)) {

	//				csl::ut::MoveArray<ComponentData*> componentDatas{ allocator };

	//				for (auto* origCData : obj.componentData) {
	//					auto* gocRegItem = GameObjectSystem::GetInstance()->goComponentRegistry->GetComponentInformationByName(origCData->type);
	//					auto* componentData = CreateComponentData(gocRegItem);
	//					rflops::Copy::Apply(componentData->data, origCData->data, gocRegItem->rflClass);
	//					componentDatas.push_back(componentData);
	//				}

	//				obj.componentData = std::move(componentDatas);
	//				obj.flags.set(ObjectData::Flag::COMPONENT_DATA_NEEDS_TERMINATION);
	//			}

	//			obj.componentData.push_back(CreateComponentData(gocRegItem));
	//		}
	//	}
	//	ImGui::EndPopup();
	//}
#endif

	if (obj.spawnerData) {
		auto* objClass = objSystem->gameObjectRegistry->GetGameObjectClassByName(obj.gameObjectClass);

		ImGui::SeparatorText("Object properties");
		edited |= ReflectionEditor("Object properties", obj.spawnerData, objClass->spawnerDataRflClass, true);
	}

	return edited;
}