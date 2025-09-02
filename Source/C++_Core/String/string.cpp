#include "string.hpp"
#include "../Container/view.hpp"

namespace ION::String {
    char* allocate(ION::Memory::Allocator& allocator, const char* s1, u64 length) {
        char* ret = (char*)allocator.malloc(length + 1);
        ION::Memory::copy(ret, length + 1, s1, length + 1);

        return ret;
    }

    u64 length(const char* c_string) {
        u64 ret = 0;
        while (*c_string++ != '\0') {
            ret += 1;
        } 

        return ret;
    }

    bool equal(const char* s1, u64 s1_length, const char* s2, u64 s2_length) {
        return Memory::equal(s1, s1_length, s2, s2_length);
    }

    s64 index_of(const char* str, u64 str_length, const char* substring, u64 substring_length) {
        RUNTIME_ASSERT(str);
        RUNTIME_ASSERT(substring);

        if (str_length == 0 && substring_length == 0) {
            return 0;
        } else if (substring_length == 0) {
            return -1;
        } else if (str_length == 0) {
            return -1;
        }

        if (substring_length > str_length) {
            return -1;
        }
        
        s64 ret_index = -1;
        for (u64 i = 0; i <= str_length - substring_length; i++) {
            if (ret_index != -1) {
                break;
            }
            
            if (str[i] != substring[0]) {
                continue;
            }

            ION::Container::View<char> current_view = ION::Container::View(str + i, substring_length);
            if (ION::String::equal(substring, substring_length, current_view.data, current_view.length)) {
                ret_index = (s64)i;
                break;
            }
        }

        return ret_index; // returns -1 if not found
    }

    bool contains(const char* str, u64 str_length, const char* contains, u64 contains_length) {
        RUNTIME_ASSERT(str);
        RUNTIME_ASSERT(contains);

        return ION::String::index_of(str, str_length, contains, contains_length) != -1;  
    }

    s64 last_index_of(const char* str, u64 str_length, const char* substring, u64 substring_length) {
        RUNTIME_ASSERT(str);
        RUNTIME_ASSERT(substring);

        if (str_length == 0 && substring_length == 0) {
            return 0;
        } else if (substring_length == 0) {
            return -1;
        } else if (str_length == 0) {
            return -1;
        }

        if (substring_length > str_length) {
            return -1;
        }
        
        s64 ret_index = -1;
        for (u64 i = 0; i <= (str_length - substring_length); i++) {
            if (str[i] != substring[0]) {
                continue;
            }

            ION::Container::View<char> current_view = ION::Container::View(str + i, substring_length);
            if (ION::String::equal(current_view.data, current_view.length, substring, substring_length)) {
                ret_index = (s64)i;
            }
        }

        return ret_index; // returns -1 if not found
    }

    bool starts_with(const char* str, u64 str_length, const char* starts_with, u64 starts_with_length) {
        RUNTIME_ASSERT(str);
        RUNTIME_ASSERT(starts_with);

        if (str_length == 0 && starts_with_length == 0) {
            return true;
        } else if (str[0] != starts_with[0] || str_length < starts_with_length) {
            return false;
        }
        
        if (ION::String::equal(str, starts_with_length, starts_with, starts_with_length)) {
            return true;
        }

        return false;
    }

    bool ends_with(const char* str, u64 str_length, const char* ends_with, u64 ends_with_length) {
        RUNTIME_ASSERT(str);
        RUNTIME_ASSERT(ends_with);

        if ((str_length == 0) || (ends_with_length == 0)) {
            return true;
        } else if (str_length < ends_with_length) {
            return false;
        }

        u64 start_index = MAX(str_length - 1, 0) - MAX(ends_with_length - 1, 0);
        if (str[start_index] != ends_with[0]) {
            return false;
        }

        if (ION::String::equal(str + start_index, ends_with_length, ends_with, ends_with_length)) {
            return true;
        }

        return false;
    }

    void copy(char* s1, byte_t s1_capacity, const char* s2, u64 s2_length) {
        ION::Memory::zero(s1, s1_capacity);
        ION::Memory::copy(s1, s1_capacity, s2, s2_length);
    }

    void insert(char* str, u64 str_length, byte_t str_capacity, const char* to_insert, u64 to_insert_length, u64 index) {
        RUNTIME_ASSERT(str);
        RUNTIME_ASSERT(to_insert);

        u64 new_length = str_length + to_insert_length;
        RUNTIME_ASSERT_MSG(new_length < str_capacity, "ckg_str_insert: str_capacity is %lld but new valid cstring length is %d + %d + 1(null_term)= %d\n", str_capacity, str_length, to_insert_length, new_length + 1);
        
        u8* move_source_ptr = (u8*)(str + index);
        u8* move_dest_ptr = (u8*)(move_source_ptr + to_insert_length);

        ION::Memory::copy(move_dest_ptr, str_capacity - (index + to_insert_length), move_source_ptr, str_length - index);
        u8* copy_dest_ptr = (u8*)(str + index);
        ION::Memory::copy(copy_dest_ptr, str_capacity, to_insert, to_insert_length);
    }

    void insert(char* str, u64 str_length, byte_t str_capacity, char to_insert, u64 index) {
        RUNTIME_ASSERT(str);
        RUNTIME_ASSERT(to_insert);

        u64 to_insert_length = 1;
        bool expression = (str_length + to_insert_length) < str_capacity;
        RUNTIME_ASSERT_MSG(expression, "ckg_str_insert_char: str overflow new_capacity_required: %d >= current_capacity: %lld\n",  str_length + to_insert_length, str_capacity);

        char* source_ptr = str + index;
        ION::Memory::copy(source_ptr + 1, str_capacity - (index + 1), source_ptr, str_length - index);
        str[index] = to_insert;
    }

    void append(char* str, u64 str_length, byte_t str_capacity, const char* to_append, u64 to_append_length) {
        ION::String::insert(str, str_length, str_capacity, to_append, to_append_length, str_length);
    }

    void append(char* str, u64 str_length, byte_t str_capacity, char to_append) {
        ION::String::insert(str, str_length, str_capacity, to_append, str_length);
    }
}