#pragma once

#include "../Common/common.hpp"

namespace ION::Memory {
    void zero(void* data, byte_t data_size_in_bytes);
    void copy(void* destination, byte_t destination_size, const void* source, byte_t source_size);
    bool equal(const void* buffer_one, byte_t b1_size, const void* buffer_two, byte_t b2_size);

    typedef struct Allocator Allocator;
    typedef void*(T_Alloc)(const Allocator* allocator, byte_t allocation_size);
    typedef void(T_Free)(const Allocator* allocator, void* data);

    typedef struct Allocator {
        void* m_ctx = nullptr;

        Allocator() = delete;
        Allocator(T_Alloc* alloc, T_Free* free, void* ctx = nullptr);
        void* malloc(byte_t allocation_size) const;
        void free(void* data) const;
        void* realloc(void* data, byte_t old_allocation_size, byte_t new_allocation_size) const;

        static Allocator invalid();
        static Allocator libc();
    private:
        T_Alloc* m_alloc = nullptr;
        T_Free* m_free = nullptr;
    } Allocator;
}