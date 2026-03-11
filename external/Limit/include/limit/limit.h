#pragma once

#include <cstdint>
#include <cstddef>

namespace lmt {

class Continuum {
    uint8_t* m_startptr;
    size_t   m_size;
    size_t   m_offset;

public:
    Continuum(void* buffer, size_t size)
        : m_startptr(static_cast<uint8_t*>(buffer))
        , m_size(size)
        , m_offset(0) {}

    template<typename T>
    T* alloc() {
        uintptr_t current = (uintptr_t)(m_startptr + m_offset);
        size_t align = alignof(T);

        // 정렬
        uintptr_t aligned = (current + align - 1) & ~(align - 1);

        m_offset += (aligned - current);

        // 용량 초과 검사
        if (m_offset + sizeof(T) > m_size)
            return nullptr;

        T* ptr = new (reinterpret_cast<void*>(aligned)) T{};
        m_offset += sizeof(T);
        return ptr;
    }

    template<typename T>
    T* alloc(size_t count) {
        if (count == 0)
            return nullptr;

        uintptr_t current = (uintptr_t)(m_startptr + m_offset);
        size_t align = alignof(T);

        // 정렬
        uintptr_t aligned = (current + align - 1) & ~(align - 1);

        m_offset += (aligned - current);

        // 용량 초과 검사
        if (m_offset + sizeof(T) * count > m_size)
            return nullptr;

        T* ptr = reinterpret_cast<T*>(aligned);
        for (size_t i = 0; i < count; ++i)
            new (ptr + i) T{};

        m_offset += sizeof(T) * count;
        return ptr;
    }

    void reset() { m_offset = 0; }
};

// a의 끝과 b의 시작이 연속인지 검증하고 경계 포인터 반환
template<typename T, typename U>
U* limit(T* from, U* to) {
    uintptr_t from_end = (uintptr_t)from + sizeof(T);
    uintptr_t to_start = (uintptr_t)to;

    // 정렬 고려: from 끝에서 to의 정렬 맞춘 위치가 to와 같아야 함
    uintptr_t aligned_end = (from_end + alignof(U) - 1) & ~(alignof(U) - 1);

    if (aligned_end != to_start)
        return nullptr;

    return to;
}

// 배열 -> 단일
template<typename T, typename U>
U* limit(T* from, size_t from_count, U* to) {
    uintptr_t from_end = (uintptr_t)from + sizeof(T) * from_count;
    uintptr_t to_start = (uintptr_t)to;

    uintptr_t aligned_end = (from_end + alignof(U) - 1) & ~(alignof(U) - 1);

    if (aligned_end != to_start)
        return nullptr;

    return to;
}

// 단일 -> 배열
template<typename T, typename U>
U* limit(T* from, U* to, size_t to_count) {
    uintptr_t from_end = (uintptr_t)from + sizeof(T);
    uintptr_t to_start = (uintptr_t)to;

    uintptr_t aligned_end = (from_end + alignof(U) - 1) & ~(alignof(U) - 1);

    if (aligned_end != to_start)
        return nullptr;

    if (to_count == 0)
        return nullptr;

    return to;
}

// 배열 -> 배열
template<typename T, typename U>
U* limit(T* from, size_t from_count, U* to, size_t to_count) {
    uintptr_t from_end = (uintptr_t)from + sizeof(T) * from_count;
    uintptr_t to_start = (uintptr_t)to;

    uintptr_t aligned_end = (from_end + alignof(U) - 1) & ~(alignof(U) - 1);

    if (aligned_end != to_start)
        return nullptr;

    if (to_count == 0)
        return nullptr;

    return to;
}

} // namespace lmt
