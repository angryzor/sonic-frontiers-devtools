#pragma once

class RflMoveArrayAccessor {
protected:
	class RflMoveArray : public csl::ut::MoveArray<void*> {
	public:
		void change_allocator(csl::fnd::IAllocator* new_allocator, const hh::fnd::RflClassMember* member);
		void reserve(size_t len, const hh::fnd::RflClassMember* member);
		void emplace_back(const hh::fnd::RflClassMember* member);
		void remove(size_t i, const hh::fnd::RflClassMember* member);
		void clear(const hh::fnd::RflClassMember* member);
		void** get(size_t i, const hh::fnd::RflClassMember* member) const;
	};

	RflMoveArray& underlying;
public:
	const hh::fnd::RflClassMember* member;

	class iterator {
		const RflMoveArrayAccessor& accessor;
		size_t idx{};

	public:

		iterator(const RflMoveArrayAccessor& accessor, size_t idx);
		iterator(const iterator& other);

		iterator& operator++();
		iterator operator++(int);
		iterator& operator--();
		iterator operator--(int);
		bool operator==(const iterator& other) const;
		bool operator!=(const iterator& other) const;
		bool operator<(const iterator& other) const;
		bool operator>(const iterator& other) const;
		bool operator<=(const iterator& other) const;
		bool operator>=(const iterator& other) const;
		void* operator*() const;
	};
	RflMoveArrayAccessor(csl::ut::MoveArray<void*>& underlying, const hh::fnd::RflClassMember* member);
	void change_allocator(csl::fnd::IAllocator* new_allocator);
	void reserve(size_t len);
	void emplace_back();
	void remove(size_t i);
	void clear();
	iterator begin() const;
	iterator end() const;
	void* operator[](size_t i) const;
	size_t size() const;


};
