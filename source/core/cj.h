#pragma once

// #define CJ_IMPL 
// for the implementation details

#ifdef __cplusplus
    #define CJ_API extern "C"
#else 
    #define CJ_API
#endif 

#if defined(CJ_IMPL)
    #define CJ_IMPL_MEMORY
    #define CJ_IMPL_OS
    #define CJ_IMPL_COLLECTIONS
    #define CJ_IMPL_CSTRING
    #define CJ_IMPL_ARENA

    #define CJ_IMPL_CREATION
    #define CJ_IMPL_FORMATTED_BUFFER

    #define CJ_IMPL_CJ_TOKEN
    #define CJ_IMPL_LEXER
    #define CJ_IMPL_PARSING
#endif

#define CJ_INCLUDE_TYPES
#define CJ_INCLUDE_OS
#define CJ_INCLUDE_ARENA
#define CJ_INCLUDE_CSTRING
#define CJ_INCLUDE_CREATION
#define CJ_INCLUDE_FORMATTED_BUFFER
#define CJ_INCLUDE_PARSING

#if defined(CJ_INCLUDE_TYPES)
    #undef NULLPTR
    #undef PI
    #undef stringify
    #undef glue
    #undef KiloBytes
    #undef MegaBytes
    #undef GigaBytes
    #undef MIN
    #undef MAX
    #undef CLAMP
    #undef FIRST_DIGIT
    #undef GET_BIT
    #undef SET_BIT
    #undef UNSET_BIT
    #undef ArrayCount
    #undef PLATFORM_WINDOWS
    #undef PLATFORM_APPLE
    #undef PLATFORM_LINUX
    #undef OS_DELIMITER
    #undef CRASH
    #undef UNUSED_FUNCTION

    #include <stdio.h>
    #include <stdbool.h>
    #include <stdint.h>
    #include <stdarg.h>
    #include <stdlib.h>
    #include <stddef.h>
    #include <ctype.h>

    typedef int8_t  s8;
    typedef int16_t s16;
    typedef int64_t s64;

    typedef uint8_t  u8;
    typedef uint16_t u16;
    typedef uint64_t u64;

    #define NULLPTR 0
    #define PI 3.14159265359
    #define DEGREES_TO_RAD(degrees) ((degrees)*(PI/180))
    #define RAD_TO_DEGREES(rad) ((rad)*(180/PI))

    #define stringify(entry) #entry
    #define glue(a, b) a##b

    #define KiloBytes(value) ((size_t)(value) * 1024L)
    #define MegaBytes(value) ((size_t)KiloBytes(value) * 1024L)
    #define GigaBytes(value) ((size_t)MegaBytes(value) * 1024L)

    #define MIN(a, b) (((a) < (b)) ? (a) : (b))
    #define MAX(a, b) (((a) > (b)) ? (a) : (b))
    #define CLAMP(value, min_value, max_value) (MIN(MAX(value, min_value), max_value))

    #define FIRST_DIGIT(number) ((int)number % 10);
    #define GET_BIT(number, bit_to_check) ((number & (1 << bit_to_check)) >> bit_to_check)
    #define SET_BIT(number, bit_to_set) number |= (1 << bit_to_set);
    #define UNSET_BIT(number, bit_to_unset) number &= (~(1 << bit_to_unset));

    #define ArrayCount(array) (int)(sizeof(array) / sizeof(array[0]))

    #if defined(_WIN32)
        #define NOMINMAX
        #define WIN32_LEAN_AND_MEAN
        #include <windows.h>
        #define PLATFORM_WINDOWS
        #define OS_DELIMITER '\\'
        #define CRASH __debugbreak()
    #elif defined(__APPLE__)
        #include <dlfcn.h>
        #define PLATFORM_APPLE
        #define OS_DELIMITER '/'
        #define CRASH __builtin_trap()
    #elif defined(__linux__) || defined(__unix__) || defined(__POSIX__)
        #include <dlfcn.h>
        #define PLATFORM_LINUX
        #define OS_DELIMITER '/'
        #define CRASH __builtin_trap()
    #else
        #error "Unknown Platform???"
    #endif

    #if defined(__clang__)
        #define UNUSED_FUNCTION __attribute__((used))
    #elif defined(__GNUC__) || defined(__GNUG__)
        #define UNUSED_FUNCTION __attribute__((used))
    #elif defined(_MSC_VER)
        #define UNUSED_FUNCTION
    #endif
#endif

#if defined (CJ_INCLUDE_OS)
    bool cj_os_path_exists(const char* path);
    u8* cj_os_read_entire_file(const char* path, u64* returned_file_size);
#endif

#if defined(CJ_INCLUDE_ARENA)
    #define CJ_ARENA_FLAG_FIXED 0
    #define CJ_ARENA_FLAG_CIRCULAR 1
    #define CJ_ARENA_FLAG_EXTENDABLE_PAGES 2
    #define CJ_ARENA_FLAG_COUNT 3

    typedef u8 CJ_ArenaFlag;
    typedef struct CJ_Arena CJ_Arena;

    CJ_API void* MACRO_cj_arena_push(CJ_Arena* arena, size_t element_size);
    CJ_API CJ_Arena* MACRO_cj_arena_create(size_t allocation_size, CJ_ArenaFlag flag, u8 alignment);
    CJ_API CJ_Arena* MACRO_cj_arena_free(CJ_Arena* arena);

    #define cj_arena_create(allocation_size) MACRO_cj_arena_create(allocation_size, CJ_ARENA_FLAG_EXTENDABLE_PAGES, 0)
    #define cj_arena_create_custom(allocation_size, flags, alignment) MACRO_cj_arena_create(allocation_size, flags, alignment)
    #define cj_arena_free(arena) arena = MACRO_cj_arena_free(arena)

    #define cj_arena_push(arena, type) ((type*)MACRO_cj_arena_push(arena, sizeof(type)))
    #define cj_arena_push_array(arena, type, element_count) ((type*)MACRO_cj_arena_push(arena, sizeof(type) * element_count))
#endif

#if defined(CJ_INCLUDE_CSTRING)
    typedef struct CJ_StringView {
        const char* ptr;
        u64 start;
        u64 end;
    } CJ_StringView;
#endif

#if defined(CJ_INCLUDE_CREATION)
    typedef struct JSON JSON;

    typedef struct CJ_Array {
        JSON** jsonVector;
    } CJ_Array;

    typedef struct JSON_KEY_VALUE {
        char* key;
        JSON* value;
    } JSON_KEY_VALUE;

    typedef struct CJ_JSON {
        JSON_KEY_VALUE* key_value_pair_vector;
    } CJ_JSON;

    typedef enum CJ_Type {
        CJ_TYPE_BOOL,
        CJ_TYPE_INT,
        CJ_TYPE_FLOAT,
        CJ_TYPE_STRING,
        CJ_TYPE_ARRAY,
        CJ_TYPE_JSON,
        CJ_TYPE_NULL,
    } CJ_Type;

    typedef struct JSON {
        CJ_Type type;
        union {
            bool cj_bool;
            CJ_StringView cj_string;
            double cj_float;
            int cj_int; 
            char* cj_null; // "null\0"
            CJ_Array cj_array;
            CJ_JSON cj_json;
        };
        CJ_Arena* arena;
    } JSON;

    JSON* cj_create(CJ_Arena* arena);
    void MACRO_cj_push(JSON* root, char* key, JSON* value);

    JSON* cj_array_create(CJ_Arena* arena);
    void MACRO_cj_array_push(JSON* array, JSON* value);

    JSON* JSON_INT(CJ_Arena* arena, int value);
    JSON* JSON_FLOAT(CJ_Arena* arena, double value);
    JSON* JSON_STRING(CJ_Arena* arena, char* value);
    JSON* JSON_STRING_VIEW(CJ_Arena* arena, CJ_StringView value); 
    JSON* JSON_BOOL(CJ_Arena* arena, bool value);
    JSON* JSON_JSON(CJ_Arena* arena, JSON* json);
    JSON* JSON_NULL(CJ_Arena* arena);

    #define cj_push(root, key, value) MACRO_cj_push(root, key, _Generic((value),  \
        bool: JSON_BOOL,                        \
        char[sizeof(value)]: JSON_STRING,       \
        const char[sizeof(value)]: JSON_STRING, \
        char*: JSON_STRING,                     \
        const char*: JSON_STRING,               \
        CJ_StringView: JSON_STRING_VIEW,        \
        double: JSON_FLOAT,                     \
        int: JSON_INT,                          \
        JSON*: JSON_JSON                        \
    )(root->arena, value))


    #define cj_array_push(root, value) MACRO_cj_array_push(root, _Generic((value),  \
        bool: JSON_BOOL,                        \
        char[sizeof(value)]: JSON_STRING,       \
        const char[sizeof(value)]: JSON_STRING, \
        char*: JSON_STRING,                     \
        const char*: JSON_STRING,               \
        float: JSON_FLOAT,                      \
        int: JSON_INT,                          \
        JSON*: JSON_JSON                        \
    )(root->arena, value))
#endif

#if defined(CJ_INCLUDE_FORMATTED_BUFFER)
    CJ_API char* cj_to_string(JSON* root);
    CJ_API void cj_set_context_indent(char* new_indent);
#endif

#if defined(CJ_INCLUDE_PARSING)
    JSON* cj_parse(CJ_Arena* arena, char* json_buffer);
#endif

//
// ===================================================== CJ_IMPL =====================================================
//

#if defined(CJ_IMPL_MEMORY)
    #define cj_assert(expression)                          \
    do {                                                   \
        if (!(expression)) {                               \
            char msg_art[] = "Func: %s, File: %s:%d\n";    \
            printf(msg_art, __func__, __FILE__, __LINE__); \
            CRASH;                                         \
        }                                                  \
    } while (false)                                        \

    #define cj_assert_msg(expression, message, ...)	       \
    do {                                                   \
        if (!(expression)) {                               \
            char msg_art[] = "Func: %s, File: %s:%d\n";    \
            printf(msg_art, __func__, __FILE__, __LINE__); \
            printf(message, ##__VA_ARGS__);                \
            CRASH;                                         \
        }                                                  \
    } while (false)                                        \

    CJ_API void* cj_alloc(u64 allocation_size);
    CJ_API void* MACRO_cj_free(void* data);
    #define cj_free(data) data = MACRO_cj_free(data)

    void cj_memory_copy(const void* source, void* destination, u64 source_size_in_bytes, u64 destination_size_in_bytes) {
        cj_assert(source);
        cj_assert(destination);
        cj_assert(source_size_in_bytes <= destination_size_in_bytes);
        if (source_size_in_bytes == 0) {
            return;
        }

        u8* temp_data_copy = (u8*)cj_alloc(source_size_in_bytes);
        for (u64 i = 0; i < source_size_in_bytes; i++) {
            temp_data_copy[i] = ((u8*)source)[i];
        }

        for (u64 i = 0; i < source_size_in_bytes; i++) {
            ((u8*)destination)[i] = temp_data_copy[i];
        }

        cj_free(temp_data_copy);
    }

    void cj_memory_zero(void* data, size_t data_size_in_bytes) {
        for (u64 i = 0; i < data_size_in_bytes; i++) {
            ((u8*)data)[i] = 0;
        }
    }

    bool cj_memory_compare(const void* buffer_one, const void* buffer_two, u64 buffer_one_size, u64 buffer_two_size) {
        cj_assert(buffer_one);
        cj_assert(buffer_two);

        if (buffer_one_size != buffer_two_size) {
            return false;
        }

        u8* buffer_one_data = (u8*)buffer_one;
        u8* buffer_two_data = (u8*)buffer_two;
        for (u64 i = 0; i < buffer_one_size; i++) {
            if (buffer_one_data[i] != buffer_two_data[i]) {
                return false;
            }
        }

        return true;
    }

    void* cj_alloc(u64 allocation_size) {
        cj_assert(allocation_size != 0);

        void* ret = malloc(allocation_size);
        cj_memory_zero(ret, allocation_size);
        return ret;
    }

    void* cj_realloc(void* data, u64 old_allocation_size, u64 new_allocation_size) {
        cj_assert(old_allocation_size != 0);
        cj_assert(new_allocation_size != 0);

        void* ret = cj_alloc(new_allocation_size);
        cj_memory_copy(data, ret, old_allocation_size, new_allocation_size);
        cj_free(data);
        return ret;
    }

    void* MACRO_cj_free(void* data) {
        free(data);
        data = NULLPTR;
        return data;
    }
#endif

#if defined(CJ_IMPL_OS)
    bool cj_os_path_exists(const char* path) {
        FILE *fptr = fopen(path, "r");

        if (fptr == NULL) {
            return false;
        }

        fclose(fptr);

        return true;
    }

    u8* cj_os_read_entire_file(const char* path, u64* returned_file_size) {
        cj_assert(cj_os_path_exists(path));

        FILE* file_handle = fopen(path, "r");

        if (file_handle == NULL) {
            return false;
        }

        fseek(file_handle, 0L, SEEK_END);
        size_t file_size = ftell(file_handle);
        rewind(file_handle);

        u8* file_data = cj_alloc(file_size);
        cj_assert(fread(file_data, file_size, 1 , file_handle) != file_size); // Error reading file
        rewind(file_handle);

        fclose(file_handle);
        *returned_file_size = file_size;

        return file_data;
    }
#endif

#if defined(CJ_IMPL_COLLECTIONS)
    //
    // ========== START CJ_VECTOR ==========
    //

    typedef struct CJ_VectorHeader {
        int count;
        int capacity;
    } CJ_VectorHeader;

    typedef struct CJ_Arena CJ_Arena;

    CJ_API void* cj_vector_grow_with_arena(CJ_Arena* arena, void* vector, size_t element_size);

    #define VECTOR_DEFAULT_CAPACITY 1
    #define cj_vector_header_base(vector) ((CJ_VectorHeader*)(((u8*)vector) - sizeof(CJ_VectorHeader)))
    #define cj_vector_count(vector) (vector ? (*cj_vector_header_base(vector)).count : 0)
    #define cj_vector_capacity(vector) (*cj_vector_header_base(vector)).capacity

    #ifdef __cplusplus
        #define cj_vector_push_arena(arena, vector, element) vector = (decltype(vector))cj_vector_grow_with_arena(arena, vector, sizeof(element)); vector[cj_vector_header_base(vector)->count++]
        #define cj_stack_push_arena(arena, stack, element) stack = (decltype(stack))cj_vector_grow_with_arena(arena, stack, sizeof(element)); vector[cj_vector_header_base(stack)->count++]
    #else 
        #define cj_vector_push_arena(arena, vector, element) vector = cj_vector_grow_with_arena(arena, vector, sizeof(element)); vector[cj_vector_header_base(vector)->count++] = element
        #define cj_stack_push_arena(arena, stack, element) stack = cj_vector_grow_with_arena(arena, stack, sizeof(element)); stack[cj_vector_header_base(stack)->count++] = element
    #endif

    #define cj_stack_count(stack) (stack ? (*cj_vector_header_base(stack)).count : 0)
    #define cj_stack_pop(stack) stack[--cj_vector_header_base(stack)->count]
    #define cj_stack_peek(stack) stack[cj_stack_count(stack) - 1]
    #define cj_stack_empty(stack) (cj_stack_count(stack) == 0)

    void* cj_vector_grow(void* vector, size_t element_size) {
        if (vector == NULLPTR) {
            vector = cj_alloc(sizeof(CJ_VectorHeader) + (VECTOR_DEFAULT_CAPACITY * element_size));
            cj_memory_zero(vector, sizeof(CJ_VectorHeader) + (VECTOR_DEFAULT_CAPACITY * element_size));
            vector = (u8*)vector + sizeof(CJ_VectorHeader);
            cj_vector_capacity(vector) = VECTOR_DEFAULT_CAPACITY;
        }

        
        int count = cj_vector_count(vector);
        int capactiy = cj_vector_capacity(vector);

        if (capactiy < count + 1) {
            size_t old_allocation_size = sizeof(CJ_VectorHeader) + (capactiy * element_size);
            int new_capactiy = capactiy * 2;
            size_t new_allocation_size = sizeof(CJ_VectorHeader) + (new_capactiy * element_size);

            vector = cj_realloc(cj_vector_header_base(vector), old_allocation_size, new_allocation_size);
            vector = (u8*)vector + sizeof(CJ_VectorHeader);

            cj_vector_header_base(vector)->count = count;
            cj_vector_header_base(vector)->capacity = new_capactiy;
        }

        return vector;
    }

    void* cj_vector_grow_with_arena(CJ_Arena* arena, void* vector, size_t element_size) {
        if (vector == NULLPTR) {
            vector = MACRO_cj_arena_push(arena, sizeof(CJ_VectorHeader) + (VECTOR_DEFAULT_CAPACITY * element_size));
            cj_memory_zero(vector, sizeof(CJ_VectorHeader) + (VECTOR_DEFAULT_CAPACITY * element_size));
            vector = (u8*)vector + sizeof(CJ_VectorHeader);
            cj_vector_capacity(vector) = VECTOR_DEFAULT_CAPACITY;
        }

        
        int count = cj_vector_count(vector);
        int capactiy = cj_vector_capacity(vector);

        if (capactiy < count + 1) {
            size_t old_allocation_size = sizeof(CJ_VectorHeader) + (capactiy * element_size);
            int new_capactiy = capactiy * 2;
            size_t new_allocation_size = sizeof(CJ_VectorHeader) + (new_capactiy * element_size);

            void* new_vector = MACRO_cj_arena_push(arena, new_allocation_size);
            cj_memory_copy(cj_vector_header_base(vector), new_vector, old_allocation_size, new_allocation_size);
            vector = (u8*)new_vector + sizeof(CJ_VectorHeader);

            cj_vector_header_base(vector)->count = count;
            cj_vector_header_base(vector)->capacity = new_capactiy;
        }

        return vector;
    }

    //
    // ========== END CJ_VECTOR ==========
    //

    //
    // ========== START CJ_LinkedList ==========
    //

    typedef struct CJ_Node {
        struct CJ_Node* next;
        struct CJ_Node* prev;
        size_t element_size_in_bytes;
        void* data;
    } CJ_Node;

    typedef struct CJ_LinkedList {
        CJ_Node* head;
        CJ_Node* tail;
        size_t element_size_in_bytes;
        int count;
        bool is_pointer_type;
    } CJ_LinkedList;

    #define cj_linked_list_create(type, is_pointer_type) MACRO_cj_linked_list_create(sizeof(type), is_pointer_type)

    #ifdef __cpluCJus
        #define cj_linked_list_free(linked_list) linked_list = (decltype(linked_list))MACRO_cj_linked_list_free(linked_list)
    #else 
        #define cj_linked_list_free(linked_list) linked_list = MACRO_cj_linked_list_free(linked_list)
    #endif

    CJ_LinkedList* MACRO_cj_linked_list_create(size_t element_size_in_bytes, bool is_pointer_type) {
        CJ_LinkedList* ret = (CJ_LinkedList*)cj_alloc(sizeof(CJ_LinkedList));
        ret->count = 0;
        ret->element_size_in_bytes = element_size_in_bytes;
        ret->head = NULLPTR;
        ret->tail = NULLPTR;
        ret->is_pointer_type = is_pointer_type;
        return ret;
    }

    CJ_Node* MACRO_cj_node_create(CJ_LinkedList* linked_list, void* data) {
        CJ_Node* ret = (CJ_Node*)cj_alloc(sizeof(CJ_Node));
        if (linked_list->is_pointer_type) {
            ret->data = data;
        } else {
            ret->data = cj_alloc(linked_list->element_size_in_bytes); // user has to cj_free
            cj_memory_copy(data, ret->data, linked_list->element_size_in_bytes, linked_list->element_size_in_bytes); 
        }

        ret->element_size_in_bytes = linked_list->element_size_in_bytes;
        ret->next = NULLPTR;
        ret->prev = NULLPTR;
        return ret;
    }
    #define cj_node_create(linked_list, data) MACRO_cj_node_create(linked_list, data)

    /**
     * @brief returns a null ptr
     * 
     * @param node 
     * @return CJ_Node* 
     */
    static CJ_Node* MACRO_cj_node_free(CJ_LinkedList* linked_list, CJ_Node* node) {
        cj_assert(linked_list);
        cj_assert(node);
        node->element_size_in_bytes = 0;
        node->next = NULLPTR;
        node->prev = NULLPTR;
        cj_free(node);

        return node;
    }
    #define cj_node_free(linked_list, node) node = MACRO_cj_node_free(linked_list, node)


    /**
     * @brief returns a null ptr
     * 
     * @param node 
     * @return CJ_Node* 
     */
    static CJ_Node* MACRO_cj_node_data_free(CJ_LinkedList* linked_list, CJ_Node* node) {
        cj_assert(linked_list);
        cj_assert(node);
        cj_assert(node->data);
        node->element_size_in_bytes = 0;
        node->next = NULLPTR;
        node->prev = NULLPTR;
        if (!linked_list->is_pointer_type) {
            cj_free(node->data);
        }
        cj_free(node);

        return node;
    }
    #define cj_node_data_free(linked_list, node) node = MACRO_cj_node_data_free(linked_list, node)

    CJ_Node* cj_linked_list_insert(CJ_LinkedList* linked_list, int index, void* data) {
        cj_assert(linked_list);
        cj_assert(data);
        cj_assert(index >= 0);

        int old_count = linked_list->count++;
        if (linked_list->head == NULLPTR) { // there is not head and by definition no tail
            CJ_Node* new_node_to_insert = cj_node_create(linked_list, data);
            linked_list->head = new_node_to_insert;
            linked_list->tail = new_node_to_insert;

            return linked_list->head;
        }

        cj_assert(index <= old_count);
        CJ_Node* new_node_to_insert = cj_node_create(linked_list, data);

        if (index == 0) { // insert at head
            linked_list->head->prev = new_node_to_insert;
            new_node_to_insert->next = linked_list->head;
            linked_list->head = new_node_to_insert;
            
            return new_node_to_insert;
        }

        if (index == old_count) { // insert at tail
            linked_list->tail->next = new_node_to_insert;
            new_node_to_insert->prev = linked_list->tail;
            linked_list->tail = new_node_to_insert;

            return new_node_to_insert;
        }

        // Date: July 19, 2024
        // TODO(Jovanni): check if index is closer to count or not then reverse the loop if approaching from the tail end.
        // as opposed to the head end.
        CJ_Node* current_node = linked_list->head; 
        for (int i = 0; i < index; i++) {
            current_node = current_node->next;
        }

        new_node_to_insert->prev = current_node;
        new_node_to_insert->next = current_node->next;

        current_node->next->prev = new_node_to_insert;
        current_node->next = new_node_to_insert;

        return new_node_to_insert;
    }

    CJ_Node* cj_linked_list_get_node(CJ_LinkedList* linked_list, int index) {
        cj_assert(linked_list);
        CJ_Node* current_node = linked_list->head; 
        for (int i = 0; i < index; i++) {
            current_node = current_node->next;
        }

        return current_node;
    }

    void* cj_linked_list_get(CJ_LinkedList* linked_list, int index) {
        return cj_linked_list_get_node(linked_list, index)->data;
    }

    void* cj_linked_list_peek_head(CJ_LinkedList* linked_list) {
        return cj_linked_list_get_node(linked_list, 0)->data;
    }

    void* cj_linked_list_peek_tail(CJ_LinkedList* linked_list) {
        return cj_linked_list_get_node(linked_list, linked_list->count - 1)->data;
    }

    CJ_Node* cj_linked_list_push(CJ_LinkedList* linked_list, void* data) {
        return cj_linked_list_insert(linked_list, linked_list->count, data);
    }

    int cj_linked_list_node_to_index(CJ_LinkedList* linked_list, CJ_Node* address) {
        CJ_Node* current_node = linked_list->head; 
        for (int i = 0; i < linked_list->count + 1; i++) {
            if (current_node == address) {
                return i;
            }
            current_node = current_node->next;
        }

        cj_assert(false); // couldn't match a node to an address
        return 0; // should never get here
    }

    CJ_Node cj_linked_list_remove(CJ_LinkedList* linked_list, int index) {
        cj_assert(linked_list); 
        cj_assert(linked_list->count > 0); 
        cj_assert(index >= 0);

        int old_count = linked_list->count--;
        if (index == 0 && old_count == 1) { // removing the head fully
            CJ_Node ret = *linked_list->head;
            cj_node_free(linked_list, linked_list->head);
            linked_list->head = NULLPTR;

            return ret;
        }

        if (index == 0) { // remove head
            CJ_Node* cached_head = linked_list->head;
            linked_list->head = linked_list->head->next;
            CJ_Node ret = *cached_head; 
            cj_node_free(linked_list, cached_head);

            return ret;
        }

        cj_assert(index < old_count);
        if (index == (old_count - 1)) { // remove tail
            CJ_Node* cached_tail = linked_list->tail;
            linked_list->tail = linked_list->tail->prev;
            CJ_Node ret = *cached_tail; 
            cj_node_free(linked_list, cached_tail);

            return ret;
        }

        CJ_Node* current_node = linked_list->head; 
        for (int i = 0; i < index; i++) {
            current_node = current_node->next;
        }

        current_node->next->prev = current_node->prev;
        current_node->prev->next = current_node->next;
        CJ_Node ret = *current_node; 
        cj_node_free(linked_list, current_node);

        return ret;
    }

    CJ_Node cj_linked_list_pop(CJ_LinkedList* linked_list) {
        return cj_linked_list_remove(linked_list, linked_list->count - 1);
    }


    void* MACRO_cj_linked_list_free(CJ_LinkedList* linked_list) {
        cj_assert(linked_list); 
        CJ_Node* current_node = linked_list->head; 
        CJ_Node* next_node = NULLPTR; 
        for (int i = 0; i < linked_list->count; i++) {
            next_node = current_node->next;
            cj_node_data_free(linked_list, current_node);
            current_node = next_node;
        }
        cj_free(linked_list);

        return linked_list;
    }
    //
    // ========== END CJ_LinkedList ==========
    //
#endif

#if defined(CJ_IMPL_CSTRING)
    CJ_StringView cj_strview_create(char* str, u64 start, u64 end) {
        cj_assert(str);

        CJ_StringView ret;
        ret.ptr = str;
        ret.start = start;
        ret.end = end;

        return ret;
    }

    #define CJ_SV_LIT(lit) cj_strview_create(lit, 0, sizeof(lit) - 1)
    #define CJ_LIT_ARG(lit) lit, sizeof(lit) - 1
    #define CJ_SV_ARG(sv) sv.ptr + sv.start, sv.end - sv.start
    #define cJ_strview_length(sv) (sv.end - sv.start)

    u64 cj_cstr_length(const char* cstring) {
        cj_assert(cstring);

        if (!cstring) {
            return 0; // This should never get here but the compiler want this
        }

        u64 length = 0;
        char* cursor = (char*)cstring;
        while(*cursor++ != '\0') {
            length++;
        }

        return length;
    }

    char* cj_strview_to_cstr(CJ_StringView str) {
        char* ret = cj_alloc((str.end - str.start) + 1);
        cj_memory_copy(str.ptr + str.start, ret, str.end - str.start, (str.end - str.start) + 1);
        return ret;
    }

    bool cj_cstr_equal(const char* s1, u64 s1_length, const char* s2, u64 s2_length) {
        return cj_memory_compare(s1, s2, s1_length, s2_length);
    }

    void cj_cstr_copy(char* s1, u64 s1_capacity, const char* s2, u64 s2_length) {
        cj_memory_zero((void*)s1, s1_capacity);
        cj_memory_copy(s2, s1, s2_length, s1_capacity);
    }

    void cj_cstr_insert(char* str, u64 str_length, u64 str_capacity, const char* to_insert, u64 to_insert_length, const u64 index) {
        cj_assert(str);
        cj_assert(to_insert);

        const u64 new_length = str_length + to_insert_length;

        cj_assert(index <= str_length);
        cj_assert_msg(new_length < str_capacity, "string_insert: str_capacity is %d but new valid cstring length is %d + %d + 1(null_term)= %d\n", (int)str_capacity, (int)str_length, (int)to_insert_length, (int)new_length + 1);
        u8* move_source_ptr = (u8*)(str + index);
        u8* move_dest_ptr = (u8*)(move_source_ptr + to_insert_length);

        cj_memory_copy(move_source_ptr, move_dest_ptr, str_length - index, str_capacity - (index + to_insert_length));
        
        u8* copy_dest_ptr = (u8*)(str + index);
        cj_memory_copy(to_insert, copy_dest_ptr, to_insert_length, str_capacity);
    }

    void cj_cstr_insert_char(char* str, u64 str_length, u64 str_capacity, const char to_insert, const u64 index) {
        cj_assert(str);
        cj_assert(to_insert);
        cj_assert(index <= str_length);

        u64 to_insert_length = 1;
        bool expression = (str_length + to_insert_length) < str_capacity;
        cj_assert_msg(expression, "cj_cstr_insert_char: str overflow new_capacity_required: %d >= current_capacity: %d\n",  (int)str_length + (int)to_insert_length, (int)str_capacity);

        char* source_ptr = str + index;
        cj_memory_copy(source_ptr, source_ptr + 1, str_length - index, str_capacity - (index + 1));
        str[index] = to_insert;
    }

    void cj_cstr_append(char* str, u64 str_length, u64 str_capacity, const char* to_append, u64 to_append_length) {
        cj_cstr_insert(str, str_length, str_capacity, to_append, to_append_length, str_length);
    }

    void cj_cstr_append_char(char* str, u64 str_length, u64 str_capacity, const char to_append) {
        cj_cstr_insert_char(str, str_length, str_capacity, to_append, str_length);
    }

    s64 cj_cstr_index_of(const char* str, u64 str_length, const char* substring, u64 substring_length) {
        cj_assert(str);
        cj_assert(substring);

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

        CJ_StringView substring_view = cj_strview_create((char*)substring, 0, substring_length);
        
        s64 ret_index = -1;
        for (u64 i = 0; i <= str_length - substring_length; i++) {
            if (ret_index != -1) {
                break;
            }
            
            if (str[i] != substring[0]) {
                continue;
            }

            u64 end_index = i + substring_length;
            if (end_index > str_length) {
                break;
            }

            CJ_StringView current_view = cj_strview_create((char*)str, i, end_index);
            if (cj_cstr_equal(CJ_SV_ARG(substring_view), CJ_SV_ARG(current_view))) {
                ret_index = i;
                break;
            }
        }

        return ret_index; // returns -1 if not found
    }

    bool cj_cstr_contains(const char* str, u64 str_length, const char* contains, u64 contains_length) {
        cj_assert(str);
        cj_assert(contains);

        return cj_cstr_index_of(str, str_length, contains, contains_length) != -1;
    }

    s64 cj_cstr_last_index_of(const char* str, u64 str_length, const char* substring, u64 substring_length) {
        cj_assert(str);
        cj_assert(substring);

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

        CJ_StringView substring_view = cj_strview_create((char*)substring, 0, substring_length);
        
        s64 ret_index = -1;
        for (u64 i = 0; i <= (str_length - substring_length); i++) {
            if (str[i] != substring[0]) {
                continue;
            }

            u64 end_index = i + substring_length;
            if (end_index > str_length) {
                break;
            }

            CJ_StringView current_view = cj_strview_create((char*)str, i, end_index);
            if (cj_cstr_equal(CJ_SV_ARG(current_view), CJ_SV_ARG(substring_view))) {
                ret_index = i;
            }
        }

        return ret_index;
    }

    char* cj_cstr_between_quotes(CJ_Arena* arena, const char* str) {
        cj_assert(str);

        u64 str_length = cj_cstr_length(str);

        s64 start_delimitor_index = cj_cstr_index_of(str, str_length, CJ_LIT_ARG("\"")); 
        s64 end_delimitor_index = cj_cstr_last_index_of(str, str_length, CJ_LIT_ARG("\""));
        if (start_delimitor_index == -1 || end_delimitor_index == -1) {
            return NULLPTR;
        }

        u64 allocation_size = str_length + 1;
        char* ret = MACRO_cj_arena_push(arena, allocation_size); // techinally allocating more than I need here

        if (start_delimitor_index == -1 || end_delimitor_index == -1) {
            return NULLPTR;
        } else if (start_delimitor_index > end_delimitor_index) {
            return NULLPTR; // The start delimtor is after the end delimitor
        }

        u64 i = (u64)(start_delimitor_index + 1);

        u64 index = 0;
        while (i < (u64)end_delimitor_index) {
            cj_cstr_append_char(ret, index++, allocation_size, str[i++]);
        }

        return ret;
    }
#endif

#if defined(CJ_IMPL_ARENA)
    #define ARENA_DEFAULT_ALLOCATION_SIZE MegaBytes(1)

    typedef struct CJ_ArenaPage {
        u8* base_address;
        u64 capacity;
        u64 used;
    } CJ_ArenaPage;

    typedef struct CJ_Arena {
        const char* name;
        CJ_LinkedList* pages;
        CJ_ArenaFlag flag;
        u8 alignment;
    } CJ_Arena;

    bool cj_is_set(CJ_Arena* arena, CJ_ArenaFlag flag) {
        return arena->flag == flag;
    }

    static CJ_ArenaPage* cj_arena_page_create(size_t allocation_size) {
        CJ_ArenaPage* ret = (CJ_ArenaPage*)cj_alloc(sizeof(CJ_ArenaPage));
        ret->used = 0;
        ret->capacity = allocation_size;
        ret->base_address = (u8*)cj_alloc(allocation_size != 0 ? allocation_size : ARENA_DEFAULT_ALLOCATION_SIZE);

        return ret;
    }

    CJ_Arena* MACRO_cj_arena_create(size_t allocation_size, CJ_ArenaFlag flag, u8 alignment) {
        CJ_Arena* arena = (CJ_Arena*)cj_alloc(sizeof(CJ_Arena));
        arena->alignment = alignment == 0 ? 8 : alignment;
        arena->flag = flag;
        arena->pages = cj_linked_list_create(CJ_ArenaPage*, true);
        CJ_ArenaPage* inital_page = cj_arena_page_create(allocation_size);
        cj_linked_list_push(arena->pages, inital_page);

        return arena;
    }

    CJ_Arena* MACRO_cj_arena_free(CJ_Arena* arena) {
        cj_assert(arena);

        int page_count = arena->pages->count;
        for (int i = 0; i < page_count; i++) {
            CJ_ArenaPage* page = (CJ_ArenaPage*)cj_linked_list_remove(arena->pages, 0).data;
            cj_assert(page->base_address);
            cj_free(page->base_address);
            cj_free(page);
        }
        cj_linked_list_free(arena->pages);
        cj_free(arena);

        return arena;
    }

    void cj_arena_clear(CJ_Arena* arena) {
        cj_assert(arena);

        for (int i = 0; i < arena->pages->count; i++) {
            CJ_ArenaPage* page = (CJ_ArenaPage*)cj_linked_list_get(arena->pages, i);
            cj_assert(page->base_address);
            cj_memory_zero(page->base_address, page->used);
            page->used = 0;
        }
    }

    void* MACRO_cj_arena_push(CJ_Arena* arena, size_t element_size) {
        CJ_ArenaPage* last_page = (CJ_ArenaPage*)cj_linked_list_peek_tail(arena->pages);
        if (cj_is_set(arena, CJ_ARENA_FLAG_FIXED)) { // single page assert if you run out of memory
            cj_assert((last_page->used + element_size <= last_page->capacity));
        } else if (cj_is_set(arena, CJ_ARENA_FLAG_CIRCULAR)) { // single page circle around if you run out of memory
            if ((last_page->used + element_size > last_page->capacity)) {
                last_page->used = 0;
                cj_assert((last_page->used + element_size <= last_page->capacity));
            }
        } else if (cj_is_set(arena, CJ_ARENA_FLAG_EXTENDABLE_PAGES)) {
            cj_assert(last_page->base_address);
            if ((last_page->used + element_size > last_page->capacity)) {
                CJ_ArenaPage* next_page = cj_arena_page_create((last_page->capacity + element_size) * 2);
                cj_assert(next_page->base_address);
                cj_linked_list_push(arena->pages, next_page);
            }
        } else {
            cj_assert(false);
        }

        last_page = (CJ_ArenaPage*)cj_linked_list_peek_tail(arena->pages); // tail might change

        u8* ret = last_page->base_address + last_page->used;
        last_page->used += element_size;
        if ((last_page->used & (arena->alignment - 1)) != 0) { // if first bit is set then its not aligned
            last_page->used += (arena->alignment - (last_page->used & (arena->alignment - 1)));
        }

        return ret;
    }
#endif

#if defined(CJ_IMPL_CREATION) 
    JSON* cj_create(CJ_Arena* arena) {
        JSON* ret = NULLPTR;
        if (arena != NULLPTR) {
            ret = (JSON*)cj_arena_push(arena, JSON);
        } else {
            ret = cj_alloc(sizeof(JSON));
        }

        ret->arena = arena;
        ret->type = CJ_TYPE_JSON;

        return ret;
    }

    JSON* cj_array_create(CJ_Arena* arena) {
        JSON* ret = NULLPTR;
        if (arena != NULLPTR) {
            ret = (JSON*)cj_arena_push(arena, JSON);
        } else {
            ret = cj_alloc(sizeof(JSON));
        }

        ret->arena = arena;
        ret->type = CJ_TYPE_ARRAY;

        return ret;
    }

    void MACRO_cj_push(JSON* root, char* key, JSON* value) {
        if (root->type != CJ_TYPE_JSON) {
            // I would normally assert, but Andy wouldn't like that so I guess error codes?
            // If its not of type JSON then you shouldn't be pushing to it
        }

        JSON_KEY_VALUE pair;
        pair.key = key;
        pair.value = value;

        cj_vector_push_arena(root->arena, root->cj_json.key_value_pair_vector, pair);
    }

    void MACRO_cj_array_push(JSON* root, JSON* value) {
        if (root->type != CJ_TYPE_ARRAY) {
            // I would normally assert, but Andy wouldn't like that so I guess error codes?
            // If its not of type JSON then you shouldn't be pushing to it
        }

        cj_vector_push_arena(root->arena, root->cj_array.jsonVector, value);
    }

    JSON* JSON_INT(CJ_Arena* arena, int value) {
        JSON* ret = cj_create(arena);
        ret->type = CJ_TYPE_INT;
        ret->cj_int = value;

        return ret;
    }

    JSON* JSON_FLOAT(CJ_Arena* arena, double value) {
        JSON* ret = cj_create(arena);
        ret->type = CJ_TYPE_FLOAT;
        ret->cj_float = value;

        return ret;
    }

    JSON* JSON_STRING(CJ_Arena* arena, char* value) {
        JSON* ret = cj_create(arena);
        ret->type = CJ_TYPE_STRING;
        ret->cj_string.ptr = value;
        ret->cj_string.start = 0;
        ret->cj_string.end = cj_cstr_length(value);

        return ret;
    }

    JSON* JSON_STRING_VIEW(CJ_Arena* arena, CJ_StringView value) {
        JSON* ret = cj_create(arena);
        ret->type = CJ_TYPE_STRING;
        ret->cj_string = value;

        return ret;
    }

    JSON* JSON_BOOL(CJ_Arena* arena, bool value) {
        JSON* ret = cj_create(arena);
        ret->type = CJ_TYPE_BOOL;
        ret->cj_bool = value;

        return ret;
    }

    JSON* JSON_JSON(CJ_Arena* arena, JSON* json) {
        (void)arena;
        return json;
    }

    JSON* JSON_NULL(CJ_Arena* arena) {
        JSON* ret = cj_create(arena);
        ret->type = CJ_TYPE_NULL;
        ret->cj_null = "null";

        return ret;
    }
#endif

#if defined (CJ_IMPL_FORMATTED_BUFFER)
    static char* global_intend = "    ";
    void cj_set_context_indent(char* new_indent) {
        global_intend = new_indent;
    }

    static char* generateSpaces(CJ_Arena* arena, u64 number_of_spaces) {
        char* ret = MACRO_cj_arena_push(arena, (sizeof(char) * number_of_spaces) + 1);
        for (u64 i = 0; i < number_of_spaces; i++) {
            ret[i] = ' ';
        }
        
        return ret;
    }

    char* cj_sprint(CJ_Arena* arena, u64* allocation_size, char* fmt, ...) {
        u64 ret_alloc = 0; 
        va_list args;
        va_list args_copy;
        va_start(args, fmt);

        va_copy(args_copy, args);
        ret_alloc = vsnprintf(NULLPTR, 0, fmt, args_copy) + 1;
        char* ret = NULLPTR;
        if (arena != NULLPTR) {
            ret = MACRO_cj_arena_push(arena, ret_alloc);
        } else {
            ret = cj_alloc(ret_alloc);
        }
        va_end(args_copy);

        vsnprintf(ret, ret_alloc, fmt, args);
        va_end(args);

        if (allocation_size != NULLPTR) {
            *allocation_size = ret_alloc;
        }

        return ret;
    }

    static char* cj_format_json_with_indent(size_t total_allocation_size, CJ_Arena* arena, u64 num_json, char delimitor_left, char delimitor_right, char** buffers, u64 count) {
        u64 ret_length = 0;
        u64 spaces_allocation_size = 0;

        char* end_spaces = cj_sprint(NULL, &spaces_allocation_size, "\n%s%c", generateSpaces(arena, num_json), delimitor_right);
        total_allocation_size += spaces_allocation_size;

        char* ret = cj_alloc(total_allocation_size);
        cj_cstr_append_char(ret, ret_length++, total_allocation_size, delimitor_left);
        if (delimitor_left == '{') {
            cj_cstr_append_char(ret, ret_length++, total_allocation_size, '\n');
        }

        for (u64 i = 0; i < count; i++) {
            u64 buffer_length = cj_cstr_length(buffers[i]);
            cj_cstr_append(ret, ret_length, total_allocation_size, buffers[i], buffer_length);
            ret_length += buffer_length;
        }

        cj_cstr_append(ret, ret_length, total_allocation_size, end_spaces, spaces_allocation_size);
 
        return ret;
    }

    static char* cj_to_string_helper(CJ_Arena* arena, JSON* root, int depth) {
        switch (root->type) {
            case CJ_TYPE_BOOL: {
                char* bool_string = root->cj_bool ? "true" : "false"; 
                return cj_sprint(arena, NULLPTR, "%s", bool_string);
            } break;

            case CJ_TYPE_INT: {
                return cj_sprint(arena, NULLPTR, "%d",  root->cj_int);
            } break;

            case CJ_TYPE_FLOAT: {
                return cj_sprint(arena, NULLPTR, "%.2g", root->cj_float);
            } break;

            case CJ_TYPE_STRING: {
                return cj_sprint(arena, NULLPTR, "\"%.*s\"", root->cj_string.end - root->cj_string.start, root->cj_string.ptr);    
            } break;
            
            case CJ_TYPE_NULL: {
                return cj_sprint(arena, NULLPTR, "%s",  root->cj_null);
            } break;

            case CJ_TYPE_ARRAY: {
                depth += 1;
                u64 num_json = (depth - 1) * cj_cstr_length(global_intend);
                u64 num_key = depth * cj_cstr_length(global_intend);

                u64 count = cj_vector_count(root->cj_json.key_value_pair_vector);
                char** buffers = cj_arena_push_array(arena, char*, count);
                u64 total_allocation_size = sizeof("[\n%s]");
                for (u64 i = 0; i < count; i++) {
                    u64 allocation_size = 0;
                    JSON* new_root = root->cj_array.jsonVector[i];
                    char* value = cj_to_string_helper(arena, root->cj_array.jsonVector[i], depth);
                    if (i == (count - 1)) {
                        buffers[i] = cj_sprint(arena, &allocation_size, "\n%s%s", generateSpaces(arena, num_key), value);
                    } else {
                        buffers[i] = cj_sprint(arena, &allocation_size, "\n%s%s, ", generateSpaces(arena, num_key), value);
                    }

                    total_allocation_size += allocation_size;
                    if (new_root->type == CJ_TYPE_JSON) {
                        cj_free(value);
                    }
                }

                return cj_format_json_with_indent(total_allocation_size, arena, num_json, '[', ']', buffers, count);
            } break;

            // Date: January 23, 2025
            // TODO(Jovanni): FIX THE MEMORY LEAKS with buffers and stuff
            case CJ_TYPE_JSON: {
                depth += 1;
                u64 num_json = (depth - 1) * cj_cstr_length(global_intend);
                u64 num_key = depth * cj_cstr_length(global_intend);
                u64 count = cj_vector_count(root->cj_json.key_value_pair_vector);

                char** buffers = cj_arena_push_array(arena, char*, count);
                u64 total_allocation_size = sizeof("{\n\n%s}");
                for (u64 i = 0; i < count; i++) {
                    u64 allocation_size = 0;
                    JSON* new_root = root->cj_json.key_value_pair_vector[i].value;
                    char* key = root->cj_json.key_value_pair_vector[i].key;
                    char* value = cj_to_string_helper(arena, new_root, depth);

                    if (i == (count - 1)) {
                        buffers[i] = cj_sprint(arena, &allocation_size, "%s\"%s\": %s", generateSpaces(arena, num_key), key, value); 
                    } else {
                        buffers[i] = cj_sprint(arena, &allocation_size, "%s\"%s\": %s,\n", generateSpaces(arena, num_key), key, value);  
                    }

                    total_allocation_size += allocation_size;
                    if (new_root->type == CJ_TYPE_JSON) {
                        cj_free(value);
                    }
                }

                return cj_format_json_with_indent(total_allocation_size, arena, num_json, '{', '}', buffers, count);
            } break;

            default: {
                cj_assert(false); // something has gone horribly wrong...
            } break;
        }

        return NULLPTR;
    }

    char* cj_to_string(JSON* root) {
        if (!root) {
            #define ERR_STR "CJ: (JSON* root = null)"
            u64 allocation_size = sizeof(ERR_STR);
            char* ret = cj_alloc(allocation_size);
            cj_cstr_append(ret, 0, allocation_size, CJ_LIT_ARG(ERR_STR));
            return ret;
        }

        CJ_Arena* temp_arena = cj_arena_create(KiloBytes(1));
        char* ret = cj_to_string_helper(temp_arena, root, 0);
        cj_arena_free(temp_arena);

        return ret;
    }
#endif

#if defined(CJ_IMPL_CJ_TOKEN)
    typedef enum CJ_TokenType {
        CJ_TOKEN_ILLEGAL_TOKEN,
        CJ_TOKEN_EOF,
        CJ_TOKEN_LEFT_PAREN,                 // "("
        CJ_TOKEN_RIGHT_PAREN,                // ")"
        CJ_TOKEN_COMMA,                      // ","
        CJ_TOKEN_MINUS,                      // "-"
        CJ_TOKEN_LEFT_BRACKET,               // "["
        CJ_TOKEN_RIGHT_BRACKET,              // "]"
        CJ_TOKEN_LEFT_CURLY,                 // "{"
        CJ_TOKEN_RIGHT_CURLY,                // "}"
        CJ_TOKEN_COLON,                      // ":"
        CJ_TOKEN_STRING_LITERAL,             // "TESTING
        CJ_TOKEN_INTEGER_LITERAL,            // 6
        CJ_TOKEN_FLOAT_LITERAL,              // 2.523
        CJ_TOKEN_TRUE,                       // true
        CJ_TOKEN_FALSE,                      // false
        CJ_TOKEN_NULL,                       // null
        CJ_TOKEN_COUNT
    } CJ_TokenType;

    typedef struct CJ_Token {
        char *lexeme;
        CJ_TokenType type;
        u64 line;
    } CJ_Token;

    const char* cj_tokenTypeToString(CJ_TokenType type);

    static const char* lookup_table[CJ_TOKEN_COUNT] = {
        stringify(CJ_TOKEN_ILLEGAL_TOKEN),
        stringify(CJ_TOKEN_EOF),
        stringify(CJ_TOKEN_LEFT_PAREN),
        stringify(CJ_TOKEN_RIGHT_PAREN),
        stringify(CJ_TOKEN_COMMA),
        stringify(CJ_TOKEN_MINUS),
        stringify(CJ_TOKEN_LEFT_BRACKET),
        stringify(CJ_TOKEN_RIGHT_BRACKET),
        stringify(CJ_TOKEN_LEFT_CURLY),
        stringify(CJ_TOKEN_RIGHT_CURLY),
        stringify(CJ_TOKEN_COLON),
        stringify(CJ_TOKEN_STRING_LITERAL),
        stringify(CJ_TOKEN_INTEGER_LITERAL),
        stringify(CJ_TOKEN_FLOAT_LITERAL),
        stringify(CJ_TOKEN_TRUE),
        stringify(CJ_TOKEN_FALSE),
        stringify(CJ_TOKEN_NULL),
    };

    const char* cj_tokenTypeToString(CJ_TokenType type) {
        return lookup_table[type];
    }

    CJ_Token cj_tokenCreate(CJ_TokenType type, char* lexeme, u64 line) {
        CJ_Token ret = {0};
        ret.lexeme = lexeme;
        ret.type = type;
        ret.line = line;

        return ret;
    }
#endif

#if defined (CJ_IMPL_LEXER)
    typedef struct CJ_Lexer {
        u64 left_pos;
        u64 right_pos;
        u64 line;

        u8 c;
        char* source;
        u64 source_size;

        CJ_Token* tokens;
        CJ_Arena* arena;
    } CJ_Lexer;

    static bool isWhitespace(char c) {
        return c == ' ' || c == '\t' || c == '\r' || c == '\n';
    }

    static CJ_Lexer cj_lexerCreate(CJ_Arena* arena) {
        CJ_Lexer lexer = {0};

        lexer.left_pos  = 0;
        lexer.right_pos = 0;
        lexer.line      = 1;
        lexer.c = '\0';
        lexer.source = NULLPTR;
        lexer.source_size = 0;
        lexer.arena = arena;

        return lexer;
    }

    void cj_lexerFree(CJ_Lexer* lexer) {
        u64 token_count = cj_vector_count(lexer->tokens);
        for (u64 i = 0; i < token_count; i++) {
            CJ_Token token = lexer->tokens[i];
            cj_free(token.lexeme);
        }
    }

    UNUSED_FUNCTION static void lexerReset(CJ_Lexer* lexer) {
        lexer->left_pos  = 0;
        lexer->right_pos = 0;
        lexer->line      = 1;
        lexer->c = '\0';
        lexer->source = NULLPTR;
        lexer->source_size = 0;
    }

    static CJ_StringView getScratchBuffer(CJ_Lexer* lexer) {
        return cj_strview_create(lexer->source, lexer->left_pos, lexer->right_pos);
    }

    static CJ_TokenType getAcceptedSyntax(CJ_Lexer* lexer) {
        static char syntaxLookup[] = { 
            '(', ')', ',', '-',
            '[', ']', '{', '}', ':'
        };

        static CJ_TokenType syntaxTokenTable[] = {
            CJ_TOKEN_LEFT_PAREN, CJ_TOKEN_RIGHT_PAREN,
            CJ_TOKEN_COMMA, CJ_TOKEN_MINUS,
            CJ_TOKEN_LEFT_BRACKET, CJ_TOKEN_RIGHT_BRACKET, 
            CJ_TOKEN_LEFT_CURLY, CJ_TOKEN_RIGHT_CURLY, CJ_TOKEN_COLON
        };

        for (int i = 0; i < ArrayCount(syntaxTokenTable); i++) {
            char syntax = syntaxLookup[i];
            CJ_StringView view = getScratchBuffer(lexer);
            if (syntax == (view.ptr + view.start)[0]) {
                return syntaxTokenTable[i];
            }
        }

        return CJ_TOKEN_ILLEGAL_TOKEN;
    }

    static CJ_TokenType getAcceptedKeyword(CJ_Lexer* lexer) {
        static CJ_TokenType keywordTokenTable[] = {
            CJ_TOKEN_TRUE, CJ_TOKEN_FALSE, CJ_TOKEN_NULL,
        };

        static char* keywords[] = {
            "true", "false", "null"
        };

        for (int i = 0; i < ArrayCount(keywords); i++) {
            char* keyword = keywords[i];
            CJ_StringView view = getScratchBuffer(lexer);
            if (cj_cstr_equal(keyword, cj_cstr_length(keyword), CJ_SV_ARG(view))) {
                return keywordTokenTable[i];
            }
        }

        return CJ_TOKEN_ILLEGAL_TOKEN;
    }

    static bool isEOF(CJ_Lexer* lexer) {
        return lexer->right_pos >= lexer->source_size;
    }

    static char peekNthChar(CJ_Lexer* lexer, u8 n) {
        if ((lexer->right_pos + n) >= lexer->source_size) {
            return '\0';
        }

        return lexer->source[lexer->right_pos + n];
    }

    static void consumeNextChar(CJ_Lexer* lexer) {
        lexer->c = lexer->source[lexer->right_pos];
        lexer->right_pos += 1;
    }

    static void addToken(CJ_Lexer* lexer, CJ_TokenType type) {
        char* lexeme = cj_strview_to_cstr(getScratchBuffer(lexer));
        cj_vector_push_arena(lexer->arena, lexer->tokens, cj_tokenCreate(type, lexeme, lexer->line));
    }

    static bool consumeWhitespace(CJ_Lexer* lexer) {
        if (!isWhitespace(lexer->c)) {
            return false;
        }

        if (lexer->c == '\n') {
            lexer->line += 1;
        }

        return true;
    }

    static void lexer_reportError(CJ_Lexer* lexer, char* msg) {
        char* scratch_buffer = cj_strview_to_cstr(getScratchBuffer(lexer));
        printf("Lexical Error: %s | Line: %d\n", scratch_buffer, (int)lexer->line);
        printf("Msg: %s\n", msg);
        exit(-1);
    }

    static bool tryConsumeWord(CJ_Lexer* lexer) {
        if (!isalpha(lexer->c)) {
        return false;
        }

        while (isalnum(peekNthChar(lexer, 0)) || peekNthChar(lexer, 0) == '_') {
            if (isEOF(lexer)) {
                break;
            }

            consumeNextChar(lexer);
        }

        return true;
    }

    static void tryConsumeStringLiteral(CJ_Lexer* lexer) {
        while (peekNthChar(lexer, 0) != '\"') {
            if (isEOF(lexer)) {
                lexer_reportError(lexer, "String literal doesn't have a closing double quote!");
            }

            consumeNextChar(lexer);
        }

        consumeNextChar(lexer);
        addToken(lexer, CJ_TOKEN_STRING_LITERAL);
    }


    static void tryConsumeDigitLiteral(CJ_Lexer* lexer) {
        CJ_TokenType kind = CJ_TOKEN_INTEGER_LITERAL;

        if (lexer->c == '-') {
            consumeNextChar(lexer);
        }

        while (isdigit(peekNthChar(lexer, 0)) || peekNthChar(lexer, 0) == '.') {
            if (lexer->c == '.') {
                kind = CJ_TOKEN_FLOAT_LITERAL;
            }

            consumeNextChar(lexer);
        }

        addToken(lexer, kind);
    }


    static bool consumeLiteral(CJ_Lexer* lexer) {
        if (isdigit(lexer->c) || (lexer->c == '-' && isdigit(peekNthChar(lexer, 0)))) {
            tryConsumeDigitLiteral(lexer);
            return true;
        } else if (lexer->c == '\"') {
            tryConsumeStringLiteral(lexer);
            return true;
        } else {
            return false;
        }
    }

    static bool consumeKeyword(CJ_Lexer* lexer) {
        if (!tryConsumeWord(lexer)) {
            return false;
        }

        CJ_TokenType token_type = getAcceptedKeyword(lexer);
        if (token_type != CJ_TOKEN_ILLEGAL_TOKEN) {
            addToken(lexer, token_type);
            return true;
        }

        return false;
    }

    static bool consumeSyntax(CJ_Lexer* lexer) {
        CJ_TokenType type = getAcceptedSyntax(lexer);
        if (type != CJ_TOKEN_ILLEGAL_TOKEN) {
            addToken(lexer, type);
            return true;
        }

        return false;
    }

    static void lexer_consumeNextToken(CJ_Lexer* lexer) {
        lexer->left_pos = lexer->right_pos;
        consumeNextChar(lexer);

        if (consumeWhitespace(lexer)) {}
        else if (consumeLiteral(lexer)) {}
        else if (consumeKeyword(lexer)) {}
        else if (consumeSyntax(lexer)) {}
        else {
            lexer_reportError(lexer, "Illegal token found");
        }
    }

    static CJ_Token* lexerGenerateTokenStream(CJ_Lexer* lexer, char* file_data, u64 file_size) {
        lexer->source = file_data;
        lexer->source_size = file_size;

        while (!isEOF(lexer)) {
            lexer_consumeNextToken(lexer);
        }

        char* emtpy = cj_alloc(1);
        cj_vector_push_arena(lexer->arena, lexer->tokens, cj_tokenCreate(CJ_TOKEN_EOF, emtpy, lexer->line));
        return lexer->tokens;
    }
#endif

#if defined(CJ_IMPL_PARSING)
    typedef struct CJ_Parser {
        CJ_Token* tokens;
        CJ_Token tok;
        u64 current;
        CJ_Arena* arena_allocator;
    } CJ_Parser;

    CJ_Parser cj_parserCreate(CJ_Token* token_stream) {
        CJ_Parser ret;
        ret.tokens = token_stream;
        ret.current = 0;
        ret.arena_allocator = cj_arena_create(KiloBytes(2));

        return ret;
    }

    void cj_parserFree(CJ_Parser* parser) {
        cj_arena_free(parser->arena_allocator);
    }

    static void parser_consumeNextToken(CJ_Parser* parser) {
        parser->tok = parser->tokens[parser->current];
        parser->current += 1;
    }

    static CJ_Token parser_peekNthToken(CJ_Parser* parser, int n) {
        return parser->tokens[parser->current + n];
    }

    static void parser_reportError(CJ_Parser* parser, char* msg) {
        printf("CJ_Parser Error: %s | Line: %d\n", parser_peekNthToken(parser, 0).lexeme, (int)parser_peekNthToken(parser, 0).line);
        printf("Msg: %s\n", msg);
        exit(-1);
    }

    UNUSED_FUNCTION static void expect(CJ_Parser* parser, CJ_TokenType expected_type) {
        if (parser_peekNthToken(parser, 0).type != expected_type) {
            printf("Expected: %s | Got: %s", cj_tokenTypeToString(expected_type), parser_peekNthToken(parser, 0).lexeme);
            parser_reportError(parser, "\n");
        }
    }

    UNUSED_FUNCTION static bool parser_consumeOnMatch(CJ_Parser* parser, CJ_TokenType expected_type) {
        if (parser_peekNthToken(parser, 0).type == expected_type) {
            parser_consumeNextToken(parser);
            return true;
        }

        return false;
    }

    UNUSED_FUNCTION static CJ_Token previousToken(CJ_Parser* parser) {
        return parser->tokens[parser->current - 1];
    }

    // if you have a { token append JSON

    // if you have "": $value, | append key and make sure there is another one
    // if you have "": $value  | append key

    // if you have 5 append primary
    // if you have true append bool
    // if you have null append JSON_NULL()
    // if you have a [] append JSON_Array
    // if you have ""

    static bool parseJSON(CJ_Parser* parser, CJ_Arena* arena, JSON** ret_state) {
        if (parser->current >= (u64)cj_vector_count(parser->tokens)) {
            *ret_state = NULLPTR;
        }

        if (parser_consumeOnMatch(parser, CJ_TOKEN_RIGHT_CURLY)) {
            return parser_consumeOnMatch(parser, CJ_TOKEN_COMMA);
        } else if (parser_consumeOnMatch(parser, CJ_TOKEN_RIGHT_BRACKET)) {
            return parser_consumeOnMatch(parser, CJ_TOKEN_COMMA);
        }

        if ((*ret_state)->type == CJ_TYPE_JSON) {
            if (!parser_consumeOnMatch(parser, CJ_TOKEN_STRING_LITERAL)) {
                *ret_state = NULLPTR;
            }

            char* key = cj_cstr_between_quotes(arena, parser->tok.lexeme);
    
            if (!parser_consumeOnMatch(parser, CJ_TOKEN_COLON)) {
                *ret_state = NULLPTR;
            }

            JSON_KEY_VALUE pair;
            pair.key = key;
            pair.value = NULLPTR;
            cj_vector_push_arena(arena, (*ret_state)->cj_json.key_value_pair_vector, pair);
        }

        parser_consumeNextToken(parser);

        switch (parser->tok.type) {
            case CJ_TOKEN_TRUE: {
                *ret_state = JSON_BOOL(arena, true);
            } break;

            case CJ_TOKEN_FALSE: {
                *ret_state = JSON_BOOL(arena, false);
            } break;

            case CJ_TOKEN_MINUS: {
                parser_consumeNextToken(parser);
                if (parser->tok.type == CJ_TOKEN_INTEGER_LITERAL) {
                    *ret_state = JSON_INT(arena, atoi(parser->tok.lexeme));
                } else if (parser->tok.type == CJ_TOKEN_FLOAT_LITERAL) {
                    *ret_state = JSON_FLOAT(arena, atof(parser->tok.lexeme));
                }
            } break;
            
            case CJ_TOKEN_INTEGER_LITERAL: {
                *ret_state = JSON_INT(arena, atoi(parser->tok.lexeme));
            } break;

            case CJ_TOKEN_FLOAT_LITERAL: {
                *ret_state = JSON_FLOAT(arena, atof(parser->tok.lexeme));
            } break;

            case CJ_TOKEN_STRING_LITERAL: {
                char* str_in_between_quotes = cj_cstr_between_quotes(arena, parser->tok.lexeme);
                *ret_state = JSON_STRING(arena, str_in_between_quotes);
            } break;

            case CJ_TOKEN_NULL: {
                *ret_state = JSON_NULL(arena);
            } break;

            case CJ_TOKEN_LEFT_BRACKET: { // Parse array
                *ret_state = cj_array_create(arena);
                return true;
            } break;

            case CJ_TOKEN_LEFT_CURLY: { // Parse object
                *ret_state = cj_create(arena);
                return true;
            } break;

            default: {
                cj_assert(false); // something has gone horribly wrong...
            } break;
        }

        return parser_consumeOnMatch(parser, CJ_TOKEN_COMMA);
    }

    JSON* cj_parse(CJ_Arena* arena, char* json_buffer) {
        CJ_Lexer lexer = cj_lexerCreate(arena);
        CJ_Token* token_stream = lexerGenerateTokenStream(&lexer, json_buffer, cj_cstr_length(json_buffer));
        CJ_Parser parser = cj_parserCreate(token_stream);
        if (!parser_consumeOnMatch(&parser, CJ_TOKEN_LEFT_CURLY)) {
            return NULLPTR;
        }

        JSON* root = cj_create(arena);
        if (parser_consumeOnMatch(&parser, CJ_TOKEN_RIGHT_CURLY)) {
            return root;
        }

        JSON** states = NULLPTR;
        cj_stack_push_arena(arena, states, root);
        while (cj_stack_empty(states) == false) {
            JSON* current_state = cj_stack_peek(states);
            JSON* ret_state = current_state;
            bool needs_to_recurse = parseJSON(&parser, arena, &ret_state);

            if (ret_state == NULLPTR) { // error occured in parsing
                return NULLPTR;
            }

            if (!needs_to_recurse) {
                JSON* unused = cj_stack_pop(states);
                (void)unused;
            }

            if (current_state != ret_state) {
                if (current_state->type == CJ_TYPE_JSON) {
                    u64 count = cj_vector_count(current_state->cj_json.key_value_pair_vector);
                    current_state->cj_json.key_value_pair_vector[count - 1].value = ret_state;
                } else if (current_state->type == CJ_TYPE_ARRAY) {
                    cj_array_push(current_state, ret_state);
                }
                
                if (ret_state->type == CJ_TYPE_JSON || ret_state->type == CJ_TYPE_ARRAY) {
                    cj_stack_push_arena(arena, states, ret_state);
                }
            }
        }

        cj_parserFree(&parser);
        cj_lexerFree(&lexer);

        return root;
    }

#endif