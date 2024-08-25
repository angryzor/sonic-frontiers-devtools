#include "ManagedMemoryRegistry.h"

namespace resources {
	ManagedMemoryRegistry* ManagedMemoryRegistry::instance{};

	ManagedAllocator::ManagedAllocator(hh::fnd::ManagedResource* resource) : resource{ resource } {}

	ManagedAllocator::ManagedAllocator(const ManagedAllocator& other) : resource{ other.resource } {}

	void* ManagedAllocator::Alloc(size_t in_size, size_t in_alignment)
	{
		auto* ptr = ManagedMemoryRegistry::instance->GetAllocator()->Alloc(in_size, in_alignment);
		if (ptr == nullptr)
			return nullptr;

		ManagedMemoryRegistry::instance->allocationsByResource.GetValueOrFallback(resource, nullptr).push_back(ptr);

		return ptr;
	}

	void ManagedAllocator::Free(void* in_pMemory)
	{
		auto& allocations = ManagedMemoryRegistry::instance->allocationsByResource.GetValueOrFallback(resource, nullptr);
		size_t idx = allocations.find(in_pMemory);
		
		if (idx == -1)
			return;

		ManagedMemoryRegistry::instance->GetAllocator()->Free(in_pMemory);
		allocations.remove(idx);
	}

	ManagedMemoryRegistry::ManagedMemoryRegistry(csl::fnd::IAllocator* allocator) : CompatibleObject{ allocator }
	{
	}

	ManagedMemoryRegistry::~ManagedMemoryRegistry()
	{
		for (auto it = allocationsByResource.begin(); it != allocationsByResource.end(); it++)
			for (auto* ptr : allocationsByResource.GetValue(it))
				GetAllocator()->Free(ptr);

		allocationsByResource.clear();
	}

	void ManagedMemoryRegistry::Init(csl::fnd::IAllocator* allocator)
	{
		instance = new (allocator) ManagedMemoryRegistry{ allocator };
	}

	void ManagedMemoryRegistry::Deinit()
	{
		instance->GetAllocator()->Free(instance);
	}

	ManagedAllocator ManagedMemoryRegistry::GetManagedAllocator(hh::fnd::ManagedResource* resource)
	{
		if (allocationsByResource.Find(resource) == allocationsByResource.end())
			allocationsByResource.Insert(resource, { GetAllocator() });

		return { resource };
	}

	void ManagedMemoryRegistry::FreeManagedMemory(hh::fnd::ManagedResource* resource)
	{
		auto it = allocationsByResource.Find(resource);

		if (it == allocationsByResource.end())
			return;

		for (auto* ptr : allocationsByResource.GetValue(it))
			GetAllocator()->Free(ptr);

		allocationsByResource.Erase(it);
	}

	void ManagedMemoryRegistry::ResourceRemovedCallback(hh::fnd::ManagedResource* resource)
	{
		FreeManagedMemory(resource);
	}
	void ManagedMemoryRegistry::RenderDebugInfo()
	{
		ImGui::Begin("Managed Memory Registry");

		for (auto it = allocationsByResource.begin(); it != allocationsByResource.end(); it++)
		{
			if (ImGui::TreeNode("Resource: %p", allocationsByResource.GetKey(it)->GetName())) {
				for (auto* ptr : allocationsByResource.GetValue(it))
					ImGui::Text("%p", ptr);
				ImGui::TreePop();
			}
		}

		ImGui::End();
	}

	ManagedMemoryRegistry::ResourceListener::ResourceListener()
	{
		hh::fnd::ResourceManager::GetInstance()->AddListener(this);
	}

	void ManagedMemoryRegistry::ResourceListener::ResourceRemovedCallback(hh::fnd::ManagedResource* resource)
	{
		if (&resource->GetClass() == hh::fnd::Packfile::GetTypeInfo())
			for (auto* container : static_cast<hh::fnd::Packfile*>(resource)->GetResourceContainers())
				for (int i = 0; i < container->GetNumResources(); i++)
					ManagedMemoryRegistry::instance->FreeManagedMemory(container->GetResourceByIndex(i));

		ManagedMemoryRegistry::instance->FreeManagedMemory(resource);
	}
}
