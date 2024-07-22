#pragma once

template<template<typename> typename A>
class RflArrayAccessor {
protected:
	template<template<typename> typename A2>
	class RflArray : public A<void*> {
	public:
		void change_allocator(csl::fnd::IAllocator* new_allocator, const hh::fnd::RflClassMember* member) {
			if (!new_allocator)
			{
				return;
			}

			if (this->isUninitialized() || !this->m_pBuffer)
			{
				this->m_pAllocator = new_allocator;
				return;
			}

			if (this->m_pAllocator == new_allocator)
			{
				return;
			}

			size_t subTypeSize = member->GetSubTypeSizeInBytes();

			void* new_buffer = new_allocator->Alloc(this->capacity() * subTypeSize, 16);

			memcpy(new_buffer, this->m_pBuffer, subTypeSize * this->m_length);

			if (this->m_pAllocator && !this->isUninitialized())
			{
				this->m_pAllocator->Free(this->m_pBuffer);
			}

			this->m_pAllocator = new_allocator;
			this->m_pBuffer = static_cast<void**>(new_buffer);
		}

		void reserve(size_t len, const hh::fnd::RflClassMember* member) {
			if (len <= this->capacity())
				return;

			size_t subTypeSize = member->GetSubTypeSizeInBytes();

			auto* allocator = !this->m_pAllocator ? hh::fnd::MemoryRouter::GetModuleAllocator() : this->m_pAllocator;

			void* new_buffer = allocator->Alloc(subTypeSize * len, 16);

			if (this->m_pBuffer)
			{
				memcpy(new_buffer, this->m_pBuffer, subTypeSize * this->m_length);
			}

			if (this->m_pAllocator && !this->isUninitialized())
			{
				this->m_pAllocator->Free(this->m_pBuffer);
			}

			this->m_pAllocator = allocator;
			this->m_capacity = len;
			this->m_pBuffer = static_cast<void**>(new_buffer);
		}

		void emplace_back(const hh::fnd::RflClassMember* member) {
			size_t subTypeSize = member->GetSubTypeSizeInBytes();

			this->m_length++;

			if (this->m_length > this->capacity())
			{
				reserve(this->m_length * 2, member);
			}

			void* placement = reinterpret_cast<void*>(reinterpret_cast<size_t>(this->m_pBuffer) + subTypeSize * (this->m_length - 1));

			if (member->GetSubType() == hh::fnd::RflClassMember::TYPE_STRUCT)
				hh::fnd::RflTypeInfoRegistry::GetInstance()->ConstructObject(this->m_pAllocator, placement, member->GetStructClass()->GetName());
			else
				memset(placement, 0, subTypeSize);
		}

		void remove(size_t i, const hh::fnd::RflClassMember* member) {
			size_t subTypeSize = member->GetSubTypeSizeInBytes();
			if (i >= this->m_length)
				return;

			void* placement = reinterpret_cast<void*>(reinterpret_cast<size_t>(this->m_pBuffer) + subTypeSize * i);

			if (member->GetSubType() == hh::fnd::RflClassMember::TYPE_STRUCT)
				hh::fnd::RflTypeInfoRegistry::GetInstance()->CleanupLoadedObject(placement, member->GetName());

			memmove(placement, reinterpret_cast<void*>(reinterpret_cast<size_t>(this->m_pBuffer) + subTypeSize * (i + 1)), subTypeSize * (this->m_length - i - 1));

			this->m_length--;
		}

		void clear(const hh::fnd::RflClassMember* member) {
			if (!this->empty()) {
				for (size_t i = 0; i < this->m_length; i++)
					if (member->GetSubType() == hh::fnd::RflClassMember::TYPE_STRUCT)
						hh::fnd::RflTypeInfoRegistry::GetInstance()->CleanupLoadedObject(get(i, member), member->GetName());

				this->m_length = 0;
			}
		}

		void** get(size_t i, const hh::fnd::RflClassMember* member) const {
			if (!this->m_pBuffer)
				return nullptr;

			return reinterpret_cast<void**>(reinterpret_cast<size_t>(this->m_pBuffer) + member->GetSubTypeSizeInBytes() * i);
		}
	};

#ifdef DEVTOOLS_TARGET_SDK_wars
	template<>
	class RflArray<hh::TArray> : public A<void*> {
	public:
		void reserve(size_t len, const hh::fnd::RflClassMember* member) {
			if (len <= this->capacity())
				return;

			size_t subTypeSize = member->GetSubTypeSizeInBytes();

			void* new_buffer = hh::fnd::MemoryRouter::GetDebugAllocator()->Alloc(subTypeSize * len, 16);

			if (this->m_pBuffer)
			{
				memcpy(new_buffer, this->m_pBuffer, subTypeSize * this->m_length);
			}

			if (!this->isUninitialized())
			{
				hh::fnd::MemoryRouter::GetDebugAllocator()->Free(this->m_pBuffer);
			}

			this->m_capacity = len;
			this->m_pBuffer = static_cast<void**>(new_buffer);
		}

		void emplace_back(const hh::fnd::RflClassMember* member) {
			size_t subTypeSize = member->GetSubTypeSizeInBytes();

			this->m_length++;

			if (this->m_length > this->capacity())
			{
				reserve(this->m_length * 2, member);
			}

			void* placement = reinterpret_cast<void*>(reinterpret_cast<size_t>(this->m_pBuffer) + subTypeSize * (this->m_length - 1));

			if (member->GetSubType() == hh::fnd::RflClassMember::TYPE_STRUCT)
				hh::fnd::RflTypeInfoRegistry::GetInstance()->ConstructObject(hh::fnd::MemoryRouter::GetModuleAllocator(), placement, member->GetStructClass()->GetName());
			else
				memset(placement, 0, subTypeSize);
		}

		void remove(size_t i, const hh::fnd::RflClassMember* member) {
			size_t subTypeSize = member->GetSubTypeSizeInBytes();
			if (i >= this->m_length)
				return;

			void* placement = reinterpret_cast<void*>(reinterpret_cast<size_t>(this->m_pBuffer) + subTypeSize * i);

			if (member->GetSubType() == hh::fnd::RflClassMember::TYPE_STRUCT)
				hh::fnd::RflTypeInfoRegistry::GetInstance()->CleanupLoadedObject(placement, member->GetName());

			memmove(placement, reinterpret_cast<void*>(reinterpret_cast<size_t>(this->m_pBuffer) + subTypeSize * (i + 1)), subTypeSize * (this->m_length - i - 1));

			this->m_length--;
		}

		void clear(const hh::fnd::RflClassMember* member) {
			if (!this->empty()) {
				for (size_t i = 0; i < this->m_length; i++)
					if (member->GetSubType() == hh::fnd::RflClassMember::TYPE_STRUCT)
						hh::fnd::RflTypeInfoRegistry::GetInstance()->CleanupLoadedObject(get(i, member), member->GetName());

				this->m_length = 0;
			}
		}

		// FIXME: why is this a double pointer
		void** get(size_t i, const hh::fnd::RflClassMember* member) const {
			if (!this->m_pBuffer)
				return nullptr;

			return reinterpret_cast<void**>(reinterpret_cast<size_t>(this->m_pBuffer) + member->GetSubTypeSizeInBytes() * i);
		}
	};
#endif

	RflArray<A>& underlying;
public:
	const hh::fnd::RflClassMember* member;

	class iterator {
		const RflArrayAccessor& accessor;
		size_t idx{};

	public:
		iterator(const RflArrayAccessor& accessor, size_t idx) : accessor{ accessor }, idx{ idx } {}
		iterator(const iterator& other) : accessor{ other.accessor }, idx{ other.idx } {}

		iterator& operator++() {
			idx++;
			return *this;
		}

		iterator operator++(int) {
			RflArrayAccessor::iterator result{ *this };
			idx++;
			return result;
		}

		iterator& operator--() {
			idx--;
			return *this;
		}

		iterator operator--(int) {
			RflArrayAccessor::iterator result{ *this };
			idx--;
			return result;
		}

		bool operator==(const iterator& other) const {
			return idx == other.idx;
		}

		bool operator!=(const iterator& other) const {
			return idx != other.idx;
		}

		bool operator<(const iterator& other) const {
			return idx < other.idx;
		}

		bool operator>(const iterator& other) const {
			return idx > other.idx;
		}

		bool operator<=(const iterator& other) const {
			return idx <= other.idx;
		}

		bool operator>=(const iterator& other) const {
			return idx >= other.idx;
		}

		void* operator*() const {
			return accessor[idx];
		}
	};

	RflArrayAccessor(A<void*>& underlying, const hh::fnd::RflClassMember* member) : underlying{ static_cast<RflArray<A>&>(underlying) }, member{ member } {}

	void change_allocator(csl::fnd::IAllocator* new_allocator) {
		underlying.change_allocator(new_allocator, member);
	}

	void reserve(size_t len) {
		underlying.reserve(len, member);
	}

	void emplace_back() {
		underlying.emplace_back(member);
	}

	void remove(size_t i) {
		underlying.remove(i, member);
	}

	void clear() {
		underlying.clear(member);
	}

	iterator begin() const {
		return RflArrayAccessor::iterator{ *this, 0 };
	}

	iterator end() const {
		return RflArrayAccessor::iterator{ *this, size() * member->GetSubTypeSizeInBytes() };
	}

	void* operator[](size_t i) const {
		return underlying.get(i, member);
	}

	size_t size() const {
		return underlying.size();
	}

	size_t capacity () const {
		return underlying.capacity();
	}
};
