#include <stdlib.h>

#include "memory.hpp"

namespace ION::Memory {
    void zero(void* data, byte_t data_size_in_bytes) {
        RUNTIME_ASSERT(data);
        
        for (byte_t i = 0; i < data_size_in_bytes; i++) {
            ((u8*)data)[i] = 0;
        }
    }

    void copy(void* destination, byte_t destination_size, const void* source, byte_t source_size) {
        RUNTIME_ASSERT(source);
        RUNTIME_ASSERT(destination);
        RUNTIME_ASSERT(source_size <= destination_size);
        if (source_size == 0) {
            return;
        }

        u8* src = (u8*)source;
        u8* dst = (u8*)destination;

        bool overlap = dst < src || dst >= src + source_size;
        if (overlap) {
            for (byte_t i = 0; i < source_size; i++) {
                dst[i] = src[i];
            }
        } else {
            for (byte_t i = source_size; i-- > 0;) {
                dst[i] = src[i];
            }
        }
    }

    bool equal(const void* buffer_one, byte_t b1_size, const void* buffer_two, byte_t b2_size) {
        RUNTIME_ASSERT(buffer_one);
        RUNTIME_ASSERT(buffer_two);

        if (b1_size != b2_size) {
            return false;
        }

        u8* buffer_one_data = (u8*)buffer_one;
        u8* buffer_two_data = (u8*)buffer_two;
        for (byte_t i = 0; i < b1_size; i++) {
            if (buffer_one_data[i] != buffer_two_data[i]) {
                return false;
            }
        }

        return true;
    }

    Allocator::Allocator(T_Alloc* alloc, T_Free* free, void* ctx) {
        this->m_alloc = alloc;
        this->m_free = free;
        this->m_ctx = ctx;
    }

    void* Allocator::malloc(byte_t allocation_size) const {
        RUNTIME_ASSERT(allocation_size != 0);
        void* ret = this->m_alloc(this, allocation_size);
        ION::Memory::zero(ret, allocation_size);

        return ret;
    }

    void Allocator::free(void* data) const {
        RUNTIME_ASSERT(data);
        this->m_free(this, data);
    }

    void* Allocator::realloc(void* data, byte_t old_allocation_size, byte_t new_allocation_size) const {
        RUNTIME_ASSERT(old_allocation_size != 0);
        RUNTIME_ASSERT(new_allocation_size != 0);

        void* ret = this->malloc(new_allocation_size);
        copy(ret, new_allocation_size, data, old_allocation_size);
        this->free(data);

        return ret;
    }

    Allocator Allocator::invalid() {
        return Allocator(nullptr, nullptr);
    }

    internal void* default_libc_malloc(const Allocator* allocator, byte_t allocation_size) {
        (void)allocator;
        return malloc(allocation_size);
    }

    internal void default_libc_free(const Allocator* allocator, void* data) {
        (void)allocator;
        free(data);
    }

    Allocator Allocator::libc() {
        return Allocator(default_libc_malloc, default_libc_free);
    }
}
