#include "RflMoveArrayAccessor.h"

using namespace hh::fnd;

void RflMoveArrayAccessor::RflMoveArray::change_allocator(csl::fnd::IAllocator* new_allocator, const RflClassMember* member)
{
    if (!new_allocator)
    {
        return;
    }

    if (isUninitialized() || !m_pBuffer)
    {
        m_pAllocator = new_allocator;
        return;
    }

    if (m_pAllocator == new_allocator)
    {
        return;
    }

    size_t subTypeSize = member->GetSubTypeSizeInBytes();

    void* new_buffer = new_allocator->Alloc(capacity() * subTypeSize, 16);

    memcpy(new_buffer, m_pBuffer, subTypeSize * m_length);

    if (m_pAllocator && !isUninitialized())
    {
        m_pAllocator->Free(m_pBuffer);
    }

    m_pAllocator = new_allocator;
    m_pBuffer = static_cast<void**>(new_buffer);
}

void RflMoveArrayAccessor::RflMoveArray::reserve(size_t len, const RflClassMember* member) {
    if (len <= capacity())
        return;

    size_t subTypeSize = member->GetSubTypeSizeInBytes();
    
    if (!m_pAllocator && !isUninitialized())
        change_allocator(MemoryRouter::GetModuleAllocator(), member);

    void* new_buffer = m_pAllocator->Alloc(subTypeSize * len, 16);

    if (m_pBuffer)
    {
        memcpy(new_buffer, m_pBuffer, subTypeSize * m_length);
    }

    if (!isUninitialized())
    {
        m_pAllocator->Free(m_pBuffer);
    }

    m_capacity = len;
    m_pBuffer = static_cast<void**>(new_buffer);
}

void RflMoveArrayAccessor::RflMoveArray::emplace_back(const RflClassMember* member) {
    size_t subTypeSize = member->GetSubTypeSizeInBytes();

    m_length++;

    if (m_length > capacity())
    {
        reserve(m_length * 2, member);
    }

    void* placement = reinterpret_cast<void*>(reinterpret_cast<size_t>(m_pBuffer) + subTypeSize * (m_length - 1));

    if (member->GetSubType() == RflClassMember::TYPE_STRUCT)
        RflTypeInfoRegistry::GetInstance()->ConstructObject(m_pAllocator, placement, member->GetStructClass()->GetName());
    else
        memset(placement, 0, subTypeSize);
}

void RflMoveArrayAccessor::RflMoveArray::remove(size_t i, const RflClassMember* member) {
    size_t subTypeSize = member->GetSubTypeSizeInBytes();
    if (i >= m_length)
        return;

    void* placement = reinterpret_cast<void*>(reinterpret_cast<size_t>(m_pBuffer) + subTypeSize * i);

    if (member->GetSubType() == RflClassMember::TYPE_STRUCT)
        RflTypeInfoRegistry::GetInstance()->CleanupLoadedObject(placement, member->GetName());

    memmove(placement, reinterpret_cast<void*>(reinterpret_cast<size_t>(m_pBuffer) + subTypeSize * (i + 1)), subTypeSize * (m_length - i - 1));

    m_length--;
}

void RflMoveArrayAccessor::RflMoveArray::clear(const RflClassMember* member) {
    if (!empty()) {
        for (size_t i = 0; i < this->m_length; i++)
            if (member->GetSubType() == RflClassMember::TYPE_STRUCT)
                RflTypeInfoRegistry::GetInstance()->CleanupLoadedObject(get(i, member), member->GetName());

        this->m_length = 0;
    }
}

void** RflMoveArrayAccessor::RflMoveArray::get(size_t i, const RflClassMember* member) const
{
    if (!m_pBuffer)
        return nullptr;

    return reinterpret_cast<void**>(reinterpret_cast<size_t>(m_pBuffer) + member->GetSubTypeSizeInBytes() * i);
}

RflMoveArrayAccessor::RflMoveArrayAccessor(csl::ut::MoveArray<void*>& underlying, const RflClassMember* member) : underlying{ static_cast<RflMoveArray&>(underlying) }, member{ member }
{
}

void RflMoveArrayAccessor::change_allocator(csl::fnd::IAllocator* new_allocator)
{
    underlying.change_allocator(new_allocator, member);
}

void RflMoveArrayAccessor::reserve(size_t len)
{
    underlying.reserve(len, member);
}

void RflMoveArrayAccessor::emplace_back()
{
    underlying.emplace_back(member);
}

void RflMoveArrayAccessor::remove(size_t i)
{
    underlying.remove(i, member);
}

void RflMoveArrayAccessor::clear()
{
    underlying.clear(member);
}

size_t RflMoveArrayAccessor::size() const
{
    return underlying.size();
}

void* RflMoveArrayAccessor::operator[](size_t i) const
{
    return underlying.get(i, member);
}

RflMoveArrayAccessor::iterator RflMoveArrayAccessor::begin() const
{
    return RflMoveArrayAccessor::iterator{ *this, 0 };
}

RflMoveArrayAccessor::iterator RflMoveArrayAccessor::end() const
{
    return RflMoveArrayAccessor::iterator{ *this, size() * member->GetSubTypeSizeInBytes() };
}

RflMoveArrayAccessor::iterator::iterator(const RflMoveArrayAccessor& accessor, size_t idx) : accessor{ accessor }, idx{ idx }
{
}

RflMoveArrayAccessor::iterator::iterator(const iterator& other) : accessor{ other.accessor }, idx{ other.idx }
{
}

void* RflMoveArrayAccessor::iterator::operator*() const
{
    return accessor[idx];
}

RflMoveArrayAccessor::iterator& RflMoveArrayAccessor::iterator::operator++()
{
    idx++;
    return *this;
}

RflMoveArrayAccessor::iterator RflMoveArrayAccessor::iterator::operator++(int)
{
    RflMoveArrayAccessor::iterator result{ *this };
    idx++;
    return result;
}

RflMoveArrayAccessor::iterator& RflMoveArrayAccessor::iterator::operator--()
{
    idx--;
    return *this;
}

RflMoveArrayAccessor::iterator RflMoveArrayAccessor::iterator::operator--(int)
{
    RflMoveArrayAccessor::iterator result{ *this };
    idx--;
    return result;
}

bool RflMoveArrayAccessor::iterator::operator==(const RflMoveArrayAccessor::iterator& other) const
{
    return idx == other.idx;
}

bool RflMoveArrayAccessor::iterator::operator!=(const RflMoveArrayAccessor::iterator& other) const
{
    return idx != other.idx;
}

bool RflMoveArrayAccessor::iterator::operator<(const RflMoveArrayAccessor::iterator& other) const
{
    return idx < other.idx;
}

bool RflMoveArrayAccessor::iterator::operator>(const RflMoveArrayAccessor::iterator& other) const
{
    return idx > other.idx;
}

bool RflMoveArrayAccessor::iterator::operator<=(const RflMoveArrayAccessor::iterator& other) const
{
    return idx <= other.idx;
}

bool RflMoveArrayAccessor::iterator::operator>=(const RflMoveArrayAccessor::iterator& other) const
{
    return idx >= other.idx;
}
