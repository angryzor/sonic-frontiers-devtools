#pragma once
#include <utilities/CompatibleObject.h>

namespace resources {
	class ManagedAllocator : public csl::fnd::IAllocator {
		hh::fnd::ManagedResource* resource;
	public:
		ManagedAllocator(hh::fnd::ManagedResource* resource);
		ManagedAllocator(const ManagedAllocator& other);
		void* Alloc(size_t in_size, size_t in_alignment) override;
		void Free(void* in_pMemory) override;
	};

	class ManagedMemoryRegistry : public CompatibleObject, public hh::fnd::ResourceManager::Listener {
		friend class ManagedAllocator;
		csl::ut::PointerMap<hh::fnd::ManagedResource*, csl::ut::MoveArray<void*>> allocationsByResource{ GetAllocator() };

		class ResourceListener : public hh::fnd::ResourceManager::Listener {
		public:
			ResourceListener();
			virtual ~ResourceListener();
			virtual void ResourceRemovedCallback(hh::fnd::ManagedResource* resource) override;
		};
		ResourceListener resourceListener{};

	public:
		static ManagedMemoryRegistry* instance;

		ManagedMemoryRegistry(csl::fnd::IAllocator* allocator);
		~ManagedMemoryRegistry();
		static void Init(csl::fnd::IAllocator* allocator);
		static void Deinit();
		ManagedAllocator GetManagedAllocator(hh::fnd::ManagedResource* resource);
		void FreeManagedMemory(hh::fnd::ManagedResource* resource);
		virtual void ResourceRemovedCallback(hh::fnd::ManagedResource* resource) override;
		void RenderDebugInfo();
	};
}
