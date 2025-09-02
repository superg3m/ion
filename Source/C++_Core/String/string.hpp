#pragma once

#include "../Common/common.hpp"
#include "../Memory/memory.hpp"

namespace ION::String {
    char* allocate(ION::Memory::Allocator& allocator, const char* s1, u64 length);
    u64 length(const char* c_string);
    bool equal(const char* s1, u64 s1_length, const char* s2, u64 s2_length);
    bool contains(const char* s1, u64 s1_length, const char* contains, u64 contains_length);

    s64 index_of(const char* str, u64 str_length, const char* substring, u64 substring_length);
    s64 last_index_of(const char* str, u64 str_length, const char* substring, u64 substring_length);
    bool starts_with(const char* str, u64 str_length, const char* starts_with, u64 starts_with_length);
    bool ends_with(const char* str, u64 str_length, const char* ends_with, u64 ends_with_length);

    void copy(char* s1, byte_t s1_capacity, const char* s2, u64 s2_length);
    void append(char* str, u64 str_length, byte_t str_capacity, const char* to_append, u64 to_append_length);
    void append(char* str, u64 str_length, byte_t str_capacity, char to_append);
    void insert(char* str, u64 str_length, byte_t str_capacity, const char* to_insert, u64 to_insert_length, u64 index);
    void insert(char* str, u64 str_length, byte_t str_capacity, char to_insert, u64 index);
}