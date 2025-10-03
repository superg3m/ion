#pragma once

#ifdef __cplusplus
    #define CKG_API extern "C"
#else 
    #define CKG_API
#endif 

#if defined(CKG_IMPL)
    #define CKG_IMPL_TYPES
    #define CKG_IMPL_LOGGER
    #define CKG_IMPL_ASSERT
    #define CKG_IMPL_ERRORS
    #define CKG_IMPL_MEMORY
    #define CKG_IMPL_ARENA
    #define CKG_IMPL_STRING
    #define CKG_IMPL_CHAR
    #define CKG_IMPL_COLLECTIONS
    #define CKG_IMPL_SERIALIZATION
    #define CKG_IMPL_IO
    #define CKG_IMPL_OS
#endif

#define CKG_INCLUDE_TYPES
#define CKG_INCLUDE_LOGGER
#define CKG_INCLUDE_ASSERT
#define CKG_INCLUDE_ERRORS
#define CKG_INCLUDE_MEMORY
#define CKG_INCLUDE_ARENA
#define CKG_INCLUDE_STRING
#define CKG_INCLUDE_CHAR
#define CKG_INCLUDE_COLLECTIONS
#define CKG_INCLUDE_SERIALIZATION
#define CKG_INCLUDE_IO
#define CKG_INCLUDE_OS

// KEY NOTES:
// - Whenever you see [OPTIONAL] that means you can use NULLPTR for that param

#define CKG_INCLUDE_TYPES
#if defined(CKG_INCLUDE_TYPES)
    #undef NULLPTR
    #undef PI
    #undef DEGREES_TO_RAD
    #undef RAD_TO_DEGREES
    #undef stringify
    #undef glue
    #undef KiloBytes
    #undef MegaBytes
    #undef GigaBytes
    #undef MIN
    #undef MAX
    #undef CLAMP
    #undef SQUARED
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

    typedef int8_t  s8;
    typedef int16_t s16;
    typedef int16_t s32;
    typedef int64_t s64;

    typedef uint8_t  u8;
    typedef uint16_t u16;
    typedef uint16_t u32;
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
    #define SQUARED(a) ((a) * (a))

    #define FIRST_DIGIT(number) ((int)number % 10);
    #define GET_BIT(number, bit_to_check) ((number & (1 << bit_to_check)) >> bit_to_check)
    #define SET_BIT(number, bit_to_set) number |= (1 << bit_to_set);
    #define UNSET_BIT(number, bit_to_unset) number &= (~(1 << bit_to_unset));

    #define ArrayCount(array) (int)(sizeof(array) / sizeof(array[0]))

    #if defined(_WIN32)
        #undef NOMINMAX
        #undef WIN32_LEAN_AND_MEAN
        #define NOMINMAX
        #define WIN32_LEAN_AND_MEAN
        #if defined(CKG_OS_INCLUDE)
            #include <windows.h>
        #endif
        #define PLATFORM_WINDOWS
        #define OS_DELIMITER '\\'
        #define CRASH __debugbreak()
    #elif defined(__APPLE__)
        #include <dlfcn.h>
        #define PLATFORM_APPLE
        #define OS_DELIMITER '/'
        #define CRASH __builtin_trap()
    #elif (defined(__linux__) || defined(__unix__) || defined(__POSIX__))
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

    CKG_API void U32_EndianSwap(u32* number_to_endian_swap);
    CKG_API void U64_EndianSwap(u64* number_to_endian_swap);
    CKG_API void ckg_stack_trace_dump(const char* function, const char* file, int line);
#endif

#if defined(CKG_INCLUDE_LOGGER)
    //Regular text
    #define CKG_BLACK "\033[0;30m"
    #define CKG_RED "\033[0;31m"
    #define CKG_GREEN "\033[0;32m"
    #define CKG_YELLOW "\033[0;33m"
    #define CKG_BLUE "\033[0;34m"
    #define CKG_PURPLE "\033[0;35m"
    #define CKG_CYAN "\033[0;36m"

    //Regular background
    #define CKG_BLACK_BACKGROUND "\033[40m"
    #define CKG_RED_BACKGROUND "\033[41m"
    #define CKG_GREENN_BACKGROUND "\033[42m"
    #define CKG_YELLOW_BACKGROUND "\033[43m"
    #define CKG_BLUE_BACKGROUND "\033[44m"
    #define CKG_PURPLE_BACKGROUND "\033[45m"
    #define CKG_CYAN_BACKGROUND "\033[46m"
    #define CKG_COLOR_RESET "\033[0m"

    #define CKG_LOG_LEVEL_CHARACTER_LIMIT 12
    #define CKG_PLATFORM_CHARACTER_LIMIT 512

    // Log Levels
    typedef enum CKG_LogLevel {
        LOG_LEVEL_FATAL,
        LOG_LEVEL_ERROR,
        LOG_LEVEL_WARN,
        LOG_LEVEL_DEBUG,
        LOG_LEVEL_SUCCESS,
        LOG_LEVEL_PRINT,
        LOG_LEVEL_COUNT
    } CKG_LogLevel;

    CKG_API void MACRO_ckg_log_output(CKG_LogLevel log_level, const char* message, ...);
    #define ckg_log_output(log_level, message, ...) MACRO_ckg_log_output(log_level, message, ##__VA_ARGS__)
    #define CKG_LOG_PRINT(message, ...) ckg_log_output(LOG_LEVEL_PRINT, message, ##__VA_ARGS__)
    #define CKG_LOG_SUCCESS(message, ...) ckg_log_output(LOG_LEVEL_SUCCESS, message, ##__VA_ARGS__)
    #define CKG_LOG_DEBUG(message, ...) ckg_log_output(LOG_LEVEL_DEBUG, message, ##__VA_ARGS__)
    #define CKG_LOG_WARN(message, ...) ckg_log_output(LOG_LEVEL_WARN, message, ##__VA_ARGS__)
    #define CKG_LOG_ERROR(message, ...) ckg_log_output(LOG_LEVEL_ERROR, message, ##__VA_ARGS__)
    #define CKG_LOG_FATAL(message, ...) ckg_log_output(LOG_LEVEL_FATAL, message, ##__VA_ARGS__)
#endif

#if defined(CKG_INCLUDE_ASSERT)
    CKG_API void MACRO_ckg_assert(bool expression,const char* function, const char* file, int line);
    CKG_API void MACRO_ckg_assert_msg(bool expression, const char* function, const char* file, int line, const char* msg, ...);

    #define CKG_ASSERT_ENABLED true
    #if CKG_ASSERT_ENABLED == true 
        #define ckg_assert(expression) MACRO_ckg_assert((expression), __func__, __FILE__, __LINE__)
        #define ckg_assert_msg(expression, message, ...) MACRO_ckg_assert_msg((expression), __func__, __FILE__, __LINE__, message, ##__VA_ARGS__)
    #else
        #define ckg_assert(expression)
        #define ckg_assert_msg(expression, message, ...)
    #endif
#endif

#if defined(CKG_INCLUDE_ERRORS)
    /*
    +------------+----------------------------+
    | Error Type |        Error Code          |
    | (4bits)    |         (28bits)           |
    +------------+----------------------------+
    */
    // this only allows for 4 types (CKG_ERROR_TYPE_COUNT <= 4)
    #define CKG_ERROR_TYPE_COUNT 2
    #define CKG_ERROR_TYPE_MASK (0b1111 << 28)

    #define CKG_ERROR_IO_MASK (0 << 28)
    #define CKG_ERROR_ARGS_MASK (1 << 28)
    typedef enum CKG_Error {
        CKG_ERROR_SUCCESS = -1,
        CKG_ERROR_IO_RESOURCE_NOT_FOUND = (CKG_ERROR_IO_MASK | 0),
        CKG_ERROR_IO_RESOURCE_TOO_BIG,
        CKG_ERROR_IO_COUNT = 2,

        CKG_ERROR_ARG_ONE_INVALID = (CKG_ERROR_ARGS_MASK | 0),
        CKG_ERROR_ARG_ONE_NULLPTR,
        CKG_ERROR_ARG_ONE_ZERO,
        CKG_ERROR_ARG_TWO_INVALID,
        CKG_ERROR_ARG_TWO_NULLPTR,
        CKG_ERROR_ARG_TWO_ZERO,
        CKG_ERROR_ARG_THREE_INVALID,
        CKG_ERROR_ARG_THREE_NULLPTR,
        CKG_ERROR_ARG_THREE_ZERO,
        CKG_ERROR_ARG_FOUR_INVALID,
        CKG_ERROR_ARG_FOUR_NULLPTR,
        CKG_ERROR_ARG_FOUR_ZERO,
        CKG_ERROR_ARGS_COUNT = 12
    } CKG_Error;

    /**
     * @brief returns a string literal of the error code
     * 
     * @param error_code 
     * @return const char*
     */
    CKG_API const char* ckg_error_str(CKG_Error error_code);
#endif

#if defined(CKG_INCLUDE_MEMORY)
    typedef struct CKG_Allocator CKG_Allocator;
    typedef void*(CKG_Alloc_T)(CKG_Allocator* allocator, size_t allocation_size);
    typedef void(CKG_Free_T)(CKG_Allocator* allocator, void* data);

    typedef struct CKG_Allocator {
        CKG_Alloc_T* allocate;
        CKG_Free_T* free;
        void* ctx;
    } CKG_Allocator;

    CKG_API void ckg_bind_custom_allocator(CKG_Alloc_T* a, CKG_Free_T* f, void* ctx);

    CKG_API void* ckg_alloc(size_t allocation_size);
    CKG_API void* MACRO_ckg_free(void* data);
    CKG_API void* ckg_realloc(void* data, size_t old_allocation_size, size_t new_allocation_size);
    #ifdef __cplusplus
        #define ckg_free(data) data = (decltype(data))(MACRO_ckg_free(const_cast<void*>(reinterpret_cast<const void*>(data))))
    #else 
        #define ckg_free(data) data = MACRO_ckg_free(data)
    #endif

    /**
     * @brief Compares the bytes in the two buffers returns true if equal
     * 
     * @param buffer_one 
     * @param buffer_two 
     * @param b1_allocation_size 
     * @param b2_allocation_size 
     * @return bool 
     */
    CKG_API bool ckg_memory_compare(const void* buffer_one, size_t b1_size, const void* buffer_two, size_t b2_size);
    CKG_API void ckg_memory_copy(void* destination, size_t destination_size, const void* source, size_t source_size);
    CKG_API void ckg_memory_zero(void* data, size_t data_size_in_bytes);

    CKG_API void MACRO_ckg_memory_delete_index(void* data, int number_of_elements, int data_capacity, size_t element_size_in_bytes, int index);
    CKG_API void MACRO_ckg_memory_insert_index(void* data, int number_of_elements, int data_capacity, size_t element_size_in_bytes, int index);

    #define ckg_memory_fill(_buffer, _buffer_count, _fill_element) \
    do {														   \
        for (int i = 0; i < (int)_buffer_count; i++) { 			   \
            _buffer[i] = _fill_element;                            \
        }                                                  	       \
    } while(0)                                                     \

    #define ckg_memory_delete_index(data, number_of_elements, data_capacity, index) MACRO_ckg_memory_delete_index(data, number_of_elements, data_capacity, sizeof(data[0]), index)
    #define ckg_memory_insert_index(data, number_of_elements, data_capacity, element, index) MACRO_ckg_memory_insert_index(data, number_of_elements, data_capacity, sizeof(data[0]), index); data[index] = element;
#endif

#if defined(CKG_INCLUDE_COLLECTIONS)
    //
    // ========== START CKG_VECTOR ==========
    //
    typedef struct CKG_VectorHeader {
        int count;
        int capacity;
        size_t element_size;
    } CKG_VectorHeader;

    #define CKG_Vector(type) type*

    CKG_API void* ckg_vector_grow(void* vector, size_t element_size, int capacity);
    CKG_API void* MACRO_ckg_vector_free(void* vector);

    #define VECTOR_DEFAULT_CAPACITY 1
    #define ckg_vector_header_base(vector) ((CKG_VectorHeader*)(((u8*)vector) - sizeof(CKG_VectorHeader)))
    #define ckg_vector_count(vector) (ckg_assert(vector), (*ckg_vector_header_base(vector)).count)
    #define ckg_vector_capacity(vector) (ckg_assert(vector), (*ckg_vector_header_base(vector)).capacity)

    #define ckg_stack_count(stack) (ckg_assert(stack), (*ckg_vector_header_base(stack)).count)

    #if defined(__cplusplus)
        #define ckg_vector_push(vector, element) vector = (decltype(vector))ckg_vector_grow(vector, sizeof(vector[0]), 0); vector[ckg_vector_header_base(vector)->count++] = element
        #define ckg_stack_push(stack, element) stack = (decltype(stack))ckg_vector_grow(stack, sizeof(stack[0]), 0); stack[ckg_vector_header_base(stack)->count++] = element
        #define ckg_vector_free(vector) vector = (decltype(vector))MACRO_ckg_vector_free(vector)
        #define ckg_stack_free(stack) stack = (decltype(stack))MACRO_ckg_vector_free(stack)
    #else
        #define ckg_vector_push(vector, element) vector = ckg_vector_grow(vector, sizeof(vector[0]), 0); vector[ckg_vector_header_base(vector)->count++] = element
        #define ckg_stack_push(stack, element) stack = ckg_vector_grow(stack, sizeof(stack[0]), 0); stack[ckg_vector_header_base(stack)->count++] = element
        #define ckg_vector_free(vector) vector = MACRO_ckg_vector_free(vector)
        #define ckg_stack_free(stack) stack = MACRO_ckg_vector_free(stack)
    #endif
    
    #define ckg_stack_pop(stack) (ckg_assert(stack), stack[--ckg_vector_header_base(stack)->count])
    #define ckg_stack_peek(stack) (ckg_assert(stack), stack[ckg_stack_count(stack) - 1])
    #define ckg_stack_empty(stack) (ckg_assert(stack), (ckg_stack_count(stack) == 0))
    //
    // ========== END CKG_VECTOR ==========
    //


    //
    // ========== START CKG_CircularBuffer ==========
    //
    typedef struct CKG_RingBufferHeader {
        int read;
        int write;
        int count;
        int capacity;
        size_t element_size;
    } CKG_RingBufferHeader;

    CKG_API void* ckg_ring_buffer_create(size_t element_size, int capacity);
    CKG_API void* MACRO_ckg_ring_buffer_free(void* buffer);
    #define ckg_ring_buffer_header_base(buffer) ((CKG_RingBufferHeader*)(((char*)(buffer)) - sizeof(CKG_RingBufferHeader)))

    #if defined(__cplusplus)
        #define ckg_ring_buffer_free(buffer) buffer = (decltype(buffer))MACRO_ckg_ring_buffer_free(buffer)
    #else
        #define ckg_ring_buffer_free(buffer) buffer = MACRO_ckg_ring_buffer_free(buffer)
    #endif
    
    #define ckg_ring_buffer_read(buffer)         (ckg_ring_buffer_header_base(buffer)->read)
    #define ckg_ring_buffer_write(buffer)        (ckg_ring_buffer_header_base(buffer)->write)
    #define ckg_ring_buffer_count(buffer)        (ckg_assert(buffer != NULLPTR), ckg_ring_buffer_header_base(buffer)->count)
    #define ckg_ring_buffer_capacity(buffer)     (ckg_assert(buffer != NULLPTR), ckg_ring_buffer_header_base(buffer)->capacity)
    #define ckg_ring_buffer_element_size(buffer) (ckg_assert(buffer != NULLPTR), ckg_ring_buffer_header_base(buffer)->element_size)

    #define ckg_ring_buffer_empty(buffer) (ckg_ring_buffer_count(buffer) == 0)
    #define ckg_ring_buffer_full(buffer)  (ckg_ring_buffer_count(buffer) == ckg_ring_buffer_capacity(buffer))

    #define ckg_ring_buffer_enqueue(buffer, element)                                                                         \
    do {                                                                                                                     \
        ckg_assert_msg(!ckg_ring_buffer_full(buffer), "Ring buffer is overwriting unread memory!\n");                        \
        (buffer)[ckg_ring_buffer_write(buffer)] = (element);                                                                 \
        ckg_ring_buffer_header_base(buffer)->count++;                                                                        \
        ckg_ring_buffer_header_base(buffer)->write = (int)((ckg_ring_buffer_write(buffer) + 1) % ckg_ring_buffer_capacity(buffer)); \
    } while(0)                                                                                                               \

    #define ckg_ring_buffer_dequeue(buffer) buffer[ckg_ring_buffer_read(buffer)]; ckg_assert_msg(!ckg_ring_buffer_empty(buffer), "Ring buffer is empty!\n"); ckg_ring_buffer_header_base(buffer)->count--; ckg_ring_buffer_header_base(buffer)->read = (ckg_ring_buffer_read(buffer) + 1) % ckg_ring_buffer_capacity(buffer)

    //
    // ========== END CKG_CircularBuffer ==========
    //


    //
    // ========== START CKG_LinkedList ==========
    //
    typedef struct CKG_Node {
        struct CKG_Node* next;
        struct CKG_Node* prev;
        size_t element_size_in_bytes;
        void* data;
    } CKG_Node;

    typedef struct CKG_LinkedList {
        CKG_Node* head;
        CKG_Node* tail;
        size_t element_size_in_bytes;
        size_t count;
        bool is_pointer_type;
    } CKG_LinkedList;

    CKG_API CKG_LinkedList* MACRO_ckg_linked_list_create(size_t element_size_in_bytes, bool is_pointer_type);
    CKG_API CKG_Node* ckg_linked_list_insert(CKG_LinkedList* linked_list, u64 index, void* data);
    CKG_API CKG_Node* ckg_linked_list_get_node(CKG_LinkedList* linked_list, u64 index);
    CKG_API void* ckg_linked_list_get(CKG_LinkedList* linked_list, u64 index);
    CKG_API void* ckg_linked_list_peek_head(CKG_LinkedList* linked_list);
    CKG_API void* ckg_linked_list_peek_tail(CKG_LinkedList* linked_list);
    CKG_API CKG_Node* ckg_linked_list_push(CKG_LinkedList* linked_list, void* data);
    CKG_API CKG_Node ckg_linked_list_pop(CKG_LinkedList* linked_list);
    CKG_API CKG_Node ckg_linked_list_remove(CKG_LinkedList* linked_list, u64 index);
    CKG_API void* MACRO_ckg_linked_list_free(CKG_LinkedList* linked_list);
    CKG_API size_t ckg_linked_list_node_to_index(CKG_LinkedList* linked_list, CKG_Node* address);

    #define ckg_linked_list_create(type, is_pointer_type) MACRO_ckg_linked_list_create(sizeof(type), is_pointer_type)

    #ifdef __cplusplus
        #define ckg_linked_list_free(linked_list) linked_list = (decltype(linked_list))MACRO_ckg_linked_list_free(linked_list)
    #else 
        #define ckg_linked_list_free(linked_list) linked_list = MACRO_ckg_linked_list_free(linked_list)
    #endif
    //
    // ========== END CKG_LinkedList ==========
    //


    //
    // ========== START CKG_HashMap ==========
    //
    #if !defined(__cplusplus)
        typedef u64 (*CKG_HashFunction)(void* data, u64 size);
        typedef bool (*CKG_EqualityFunction)(void* c1, size_t c1_size, void* c2, size_t c2_size);

        #define CKG_HashMapEntry(KeyType, ValueType) \
        struct {                                     \
            KeyType key;                             \
            ValueType value;                         \
            bool filled;                             \
            bool dead;                               \
        }                                            \

        // Date: Aug 27, 2025
        // NOTE(Jovanni): Its important to note that temp_key and temp_value are used on
        // insert to act as a stack object for the value or key literal.
        typedef struct CKG_HashMapMeta {
            int temp_key_offset;
            int temp_value_offset;
            
            int entries_offset; // bytes to the entries from the map

            // These are all relative to the entries[i] address
            // I thought about storing these in CKG_HashMapEntry but that seems wasteful since you only need it once
            int entry_key_offset;
            int entry_value_offset;
            int entry_filled_offset;
            int entry_dead_offset;

            int key_size;
            int value_size;
            int entry_size;

            u64 capacity;
            u64 count;
            u64 dead_count;
            
            bool key_is_ptr;
            CKG_HashFunction hash_fn;
            CKG_EqualityFunction equal_fn;
        } CKG_HashMapMeta;

        // Date: May 15, 2025
        // NOTE(Jovanni): 
        // One very interesting detail that makes this all possible is that 
        // CKG_HashMapMeta is the first member of this struct
        // and CKG_HashMapMeta is known at compile time so I know if I have a map
        // I can always cast it to (CKG_HashMapMeta*) and get the offsets needed to get to temp_key, ect
        #define CKG_HashMap(KeyType, ValueType)            \
        struct {                                           \
            CKG_HashMapMeta meta;                          \
            KeyType temp_key;                              \
            ValueType temp_value;                          \
            CKG_HashMapEntry(KeyType, ValueType)* entries; \
        }                                                  \

        #define CKG_HASHMAP_DEFAULT_CAPACITY 4
        #define CKG_HASHMAP_DEFAULT_LOAD_FACTOR 0.70f

        /**
         * @brief only works with trival structs
         * 
         * @param source 
         * @param source_size 
         * @return u64 
         */
        u64 siphash24(void* source, u64 source_size);

        /**
         * @brief only works with trival structs
         * 
         * @param c1 
         * @param c1_size 
         * @param c2 
         * @param c2_size 
         * @return bool 
         */
        bool byte_equality(void* c1, size_t c1_size, void* c2, size_t c2_size);

        u64 ckg_string_hash(void* str, u64 str_length);
        bool string_equality(void* c1, size_t c1_size, void* c2, size_t c2_size);

        u64 ckg_string_view_hash(void* view, u64 str_length);
        bool string_view_equality(void* c1, size_t c1_size, void* c2, size_t c2_size);

        float ckg_hashmap_load_factor(void* map);
        void ckg_hashmap_grow(void* map);
        float ckg_hashmap_load_factor(void* map);
        
        bool ckg_hashmap_has_helper(void* map);
        void ckg_hashmap_get_helper(void* map);
        void ckg_hashmap_put_helper(void* map);
        void ckg_hashmap_pop_helper(void* map);

        /**
         * @brief Inserts a key-value pair into a generic hashmap.
         *
         * Supports value-type keys (e.g., integers, structs without pointers) via `ckg_hashmap_put`,
         * and pointer-type keys via `ckg_hashmap_put_key_ptr`.
         *
         * A **trivially hashable struct**:
         * - Contains only value types (e.g., int, float, char).
         * - Does **not** contain any pointers.
         * - Can be safely hashed via byte-wise hashing.
         *
         * If your key is a pointer (e.g., `char *`, `void *`), use `ckg_hashmap_put_key_ptr`.
         * For non-trivial structs (e.g., structs with pointers), you may also define a custom hash function
         * and assign it to the hashmap's `meta.hash_fn` field.
         *
         * @example
         *     typedef struct { int x, y; } Point; // Trivially hashable
         *     ckg_hashmap_put(map, (Point){1, 2}, 42);
         *
         *     const char *name = "Alice";
         *     ckg_hashmap_put_key_ptr(map, name, 100); // Required for pointer keys
         */
    #define ckg_hashmap_init_with_hash(map, KeyType, ValueType, __key_is_ptr, __hash_function, __eq_function) \
        do {                                                                                                  \
            map = ckg_alloc(sizeof(CKG_HashMap(KeyType, ValueType)));                                         \
            map->meta.temp_key_offset = offsetof(CKG_HashMap(KeyType, ValueType), temp_key);                  \
            map->meta.temp_value_offset = offsetof(CKG_HashMap(KeyType, ValueType), temp_value);              \
            map->meta.entries_offset = offsetof(CKG_HashMap(KeyType, ValueType), entries);                    \
            map->meta.entry_key_offset = offsetof(CKG_HashMapEntry(KeyType, ValueType), key);                 \
            map->meta.entry_value_offset = offsetof(CKG_HashMapEntry(KeyType, ValueType), value);             \
            map->meta.entry_filled_offset = offsetof(CKG_HashMapEntry(KeyType, ValueType), filled);           \
            map->meta.entry_dead_offset = offsetof(CKG_HashMapEntry(KeyType, ValueType), dead);               \
            map->meta.key_size = sizeof(KeyType);                                                             \
            map->meta.value_size = sizeof(ValueType);                                                         \
            map->meta.entry_size = sizeof(CKG_HashMapEntry(KeyType, ValueType));                              \
            map->meta.capacity = CKG_HASHMAP_DEFAULT_CAPACITY;                                                \
            map->meta.count = 0;                                                                              \
            map->meta.dead_count = 0;                                                                         \
            map->meta.hash_fn = __hash_function;                                                              \
            map->meta.equal_fn = __eq_function;                                                               \
            map->meta.key_is_ptr = __key_is_ptr;                                                              \
            map->entries = ckg_alloc(map->meta.entry_size * map->meta.capacity);                              \
            (KeyType)map->entries[0].key;                                                                     \
            (ValueType)map->entries[0].value;                                                                 \
        } while(0)                                                                                            \


        #define ckg_hashmap_init_siphash(map, KeyType, ValueType) ckg_hashmap_init_with_hash(map, KeyType, ValueType, false, siphash24, byte_equality)
        #define ckg_hashmap_init_string_hash(map, KeyType, ValueType) ckg_hashmap_init_with_hash(map, KeyType, ValueType, true, ckg_string_hash, string_equality)
        #define ckg_hashmap_init_string_view_hash(map, KeyType, ValueType) ckg_hashmap_init_with_hash(map, KeyType, ValueType, false, ckg_string_view_hash, string_view_equality)

        #define ckg_hashmap_put(map, __key, __value) \
        do {                                         \
            (map)->temp_key = (__key);               \
            (map)->temp_value = (__value);           \
            ckg_hashmap_put_helper((u8*)(map));      \
        } while(0)                                   \

        #define ckg_hashmap_has(map, key)       \
        (                                       \
            (map)->temp_key = (key),            \
            ckg_hashmap_has_helper((u8*)(map))  \
        )                                       \


        #define ckg_hashmap_get(map, key)       \
        (                                       \
            (map)->temp_key = (key),            \
            ckg_hashmap_get_helper((u8*)(map)), \
            (map)->temp_value                   \
        )                                       \

        #define ckg_hashmap_pop(map, key)       \
        (                                       \
            (map)->temp_key = (key),            \
            ckg_hashmap_pop_helper((u8*)(map)), \
            (map)->temp_value                   \
        )                                       \

        #define ckg_hashmap_free(map)                                 \
        do {                                                          \
            ckg_free(map->entries);                                   \
            ckg_free(map);                                            \
        } while(0)                                                    \

    #endif
    //
    // ========== END CKG_HashMap ==========
    //
#endif 

#if defined(CKG_INCLUDE_SERIALIZATION)
    typedef enum CKG_CollectionType {
        CKG_COLLECTION_VECTOR,
        CKG_COLLECTION_STACK,
        CKG_COLLECTION_RING_BUFFER,
        CKG_COLLECTION_LINKED_LIST
    } CKG_CollectionType;

    typedef enum CKG_DataType {
        CKG_DATA_TYPE_CSTRING,
        CKG_DATA_TYPE_STRING_VIEW,
        CKG_DATA_TYPE_BITS
    } CKG_DataType;

    CKG_API void  ckg_serialize_collection(void* collection, FILE* file_handle, CKG_CollectionType collection_type, CKG_DataType data_type);
    
    /**
     * @brief 
     * REMARKS:  -----------------------------------------------------------------------------------------------------------------------
     *  CKG_StringView - If you deserialize using CKG_DATA_TYPE_STRING_VIEW the ascii data is allocated inside .data of the stringview
     *  for (int i = 0; i < ckg_vector_count(sv_vector); i++) {
     *      CKG_StringView sv = sv_vector[i]
     *      ckg_free(sv.data);
     *  }
     * 
     *  CString - If you deserialize using CKG_DATA_TYPE_CSTRING the ascii data is allocated
     *  for (int i = 0; i < ckg_vector_count(str_vector); i++) {
     *      char* str = str_vector[i]
     *      ckg_free(str);
     *  }
     * 
     * @param file_handle 
     * @param collection_type 
     * @param data_type 
     * @return CKG_API* 
     */
    CKG_API void* ckg_deserialize_collection(FILE* file_handle, CKG_CollectionType collection_type, CKG_DataType data_type);
#endif

#if defined(CKG_INCLUDE_ARENA)
    #define ARENA_DEFAULT_ALLOCATION_SIZE MegaBytes(1)

    typedef enum CKG_ArenaFlag {
        CKG_ARENA_FLAG_INVALID = -1,
        CKG_ARENA_FLAG_FIXED = 0x1,
        CKG_ARENA_FLAG_CIRCULAR = 0x2,
        CKG_ARENA_FLAG_STACK_MEMORY = 0x4,
        CKG_ARENA_FLAG_COUNT = 4,
    } CKG_ArenaFlag;

    typedef struct CKG_Arena {
        int flags;
        size_t used;
        size_t used_save_point;
        size_t capacity;
        u8 alignment;
        u8* base_address;
        size_t* size_stack;
    } CKG_Arena;
    
    /**
     * @brief if memory is stack memory make sure to set the CKG_ARENA_FLAG_STACK_MEMORY bit in the flags
     * also alignment has to be a power of two!
     * @param memory 
     * @param allocation_size 
     * @param flag 
     * @param alignment
     * @return CKG_API* 
     */
    CKG_API CKG_Arena ckg_arena_create_custom(void* memory, size_t allocation_size, int flags, u8 alignment);
    CKG_API void ckg_arena_free(CKG_Arena* arena);
    CKG_API void* ckg_arena_push_custom(CKG_Arena* arena, size_t element_size);	
    CKG_API void ckg_arena_begin_temp(CKG_Arena* arena);
    CKG_API void ckg_arena_end_temp(CKG_Arena* arena);
    CKG_API void ckg_arena_zero(CKG_Arena* arena);
    CKG_API void ckg_arena_reset(CKG_Arena* arena);
    CKG_API void MACRO_ckg_arena_pop(CKG_Arena* arena, void* data, size_t bytes_to_pop);
    
    #define ckg_arena_create_fixed(memory, allocation_size, is_stack_memory) ckg_arena_create_custom(memory, allocation_size, CKG_ARENA_FLAG_FIXED|(is_stack_memory ? CKG_ARENA_FLAG_STACK_MEMORY : 0), 8)
    #define ckg_arena_create_circular(memory, allocation_size, is_stack_memory) ckg_arena_create_custom(memory, allocation_size, CKG_ARENA_FLAG_CIRCULAR|(is_stack_memory ? CKG_ARENA_FLAG_STACK_MEMORY : 0), 8)
    #define ckg_arena_push(arena, type) ((type*)ckg_arena_push_custom(arena, sizeof(type)))
    #define ckg_arena_push_array(arena, type, element_count) ((type*)ckg_arena_push_custom(arena, sizeof(type) * element_count))
    #define ckg_arena_pop(arena, type) MACRO_ckg_arena_pop(arena, NULLPTR, sizeof(type))
    #define ckg_arena_pop_array(arena, type) MACRO_ckg_arena_pop(arena, NULLPTR, sizeof(type) * element_count)
    #define ckg_arena_pop_data(arena, data) MACRO_ckg_arena_pop(arena, data, ckg_stack_pop(arena->size_stack))
    #define ckg_arena_temp(arena, code_block) ckg_arena_begin_temp(arena); code_block; ckg_arena_end_temp(arena);
#endif

#if defined(CKG_INCLUDE_STRING)

    /**
     * @brief returns a string buffer with nullterm
     * in most cases free with ckg_free()
     * @param s1 
     * @return char* 
     */
    CKG_API char* ckg_str_alloc(const char* s1, u64 length);
    CKG_API u64   ckg_cstr_length(const char* c_string);
    CKG_API bool ckg_str_equal(const char* s1, u64 s1_length, const char* s2, u64 s2_length);
    CKG_API bool ckg_str_contains(const char* s1, u64 s1_length, const char* contains, u64 contains_length);
    CKG_API char* ckg_str_reverse(const char* str, u64 str_length);
    CKG_API char* ckg_str_va_sprint(u64* str_length, const char* fmt, va_list args);
    CKG_API char* MACRO_ckg_str_sprint(u64* str_length_ptr_back, const char* fmt, ...);
    #define ckg_str_sprint(str_length_ptr_back, fmt, ...) MACRO_ckg_str_sprint(str_length_ptr_back, fmt, ##__VA_ARGS__)

    /**
     * @brief Check the return value of -1 to know if there is not a substring in the string provided
     * 
     * @param str 
     * @param str_length 
     * @param substring 
     * @param substring_length 
     * @return s64 
     */
    CKG_API s64  ckg_str_index_of(const char* str, u64 str_length, const char* substring, u64 substring_length);
    CKG_API s64  ckg_str_last_index_of(const char* str, u64 str_length, const char* substring, u64 substring_length);
    CKG_API bool ckg_str_starts_with(const char* str, u64 str_length, const char* starts_with, u64 starts_with_length);
    CKG_API bool ckg_str_ends_with(const char* str, u64 str_length, const char* ends_with, u64 ends_with_length);

    CKG_API void  ckg_str_clear(char* s1, u64 length);
    CKG_API void  ckg_str_copy(char* s1, size_t s1_capacity, const char* s2, u64 s2_length);
    CKG_API void  ckg_str_append(char* str, u64* str_length_out, size_t str_capacity, const char* to_append, u64 to_append_length);
    CKG_API void  ckg_str_append_char(char* str, u64* str_length_out, size_t str_capacity, char to_append);
    CKG_API void  ckg_str_insert(char* str, u64* str_length_out, size_t str_capacity, const char* to_insert, u64 to_insert_length, u64 index);
    CKG_API void  ckg_str_insert_char(char* str, u64* str_length_out, size_t str_capacity, char to_insert, u64 index);

    typedef struct CKG_StringView {
        const char* data;
        u64 length;
    } CKG_StringView;
    
    CKG_API CKG_StringView  ckg_sv_create(const char* data, u64 length);
    CKG_API CKG_StringView  ckg_sv_between_delimiters(const char* str, u64 str_length, const char* start_delimitor, u64 start_delimitor_length, const char* end_delimitor, u64 end_delimitor_length);
    CKG_API CKG_StringView* ckg_sv_split(const char* data, u64 length, const char* delimitor, u64 delimitor_length);

    #define CKG_SV_LIT(literal) (CKG_StringView){literal, sizeof(literal) - 1}
    #define CKG_SV_NULL() (CKG_StringView){NULLPTR, 0}
    #define CKG_LIT_ARG(literal) literal, sizeof(literal) - 1

    #define ckg_sv_equal(sv1, sv2) ckg_str_equal(sv1.data, sv1.length, sv2.data, sv2.length)
    #define ckg_sv_contains(sv1, sv2) ckg_str_contains(sv1.data, sv1.length, sv2.data, sv2.length)
    #define ckg_sv_index_of(sv1, sv2) ckg_str_index_of(sv1.data, sv1.length, sv2.data, sv2.length)
    #define ckg_sv_last_index_of(sv1, sv2) ckg_str_last_index_of(sv1.data, sv1.length, sv2.data, sv2.length)
    #define ckg_sv_starts_with(sv1, sv2) ckg_str_starts_with(sv1.data, sv1.length, sv2.data, sv2.length)
    #define ckg_sv_ends_with(sv1, sv2) ckg_str_ends_with(sv1.data, sv1.length, sv2.data, sv2.length)
#endif

#if defined(CKG_INCLUDE_CHAR)
    #define ckg_char_is_digit(c) (c >= '0' && c <= '9')
    #define ckg_char_is_upper(c) (c >= 'A' && c <= 'Z')
    #define ckg_char_is_lower(c) (c >= 'a' && c <= 'z')

    CKG_API bool ckg_char_is_alpha(char c);
    CKG_API bool ckg_char_is_alpha_numeric(char c);
#endif

#if defined(CKG_INCLUDE_IO)
    CKG_API bool ckg_io_path_exists(const char* path);
    /**
     * @brief returns null terminated file data 
     * 
     * @param file_name 
     * @param file_name_length
     * @param file_size [OPTIONAL]
     * @param err [OPTIONAL]
     * @return u8* 
     */
    CKG_API u8* ckg_io_read_entire_file(const char* file_name, size_t* returned_file_size, CKG_Error* err);
#endif

#if defined(CKG_INCLUDE_OS)
    typedef void* CKG_DLL;

    /**
     * @brief
     * 
     * @param dll_name 
     * @param err [OPTIONAL]
     * @return CKG_DLL* 
     */
    CKG_API CKG_DLL ckg_io_load_dll(const char* dll_name, CKG_Error* err);

    /**
     * @brief 
     * 
     * @param dll 
     * @param proc_name 
     * @param err [OPTIONAL]
     * @return void* 
     */
    CKG_API void* ckg_os_get_proc_address(CKG_DLL dll, const char* proc_name, CKG_Error* err);

    /**
     * @brief 
     * 
     * @param dll 
     * @return CKG_DLL* 
     */
    CKG_API CKG_DLL MACRO_ckg_os_free_dll(CKG_DLL dll);

    #define ckg_os_free_dll(dll) dll = MACRO_ckg_os_free_dll(dll)
#endif

//
// ===================================================== CKG_IMPL =====================================================
//

#if defined(CKG_IMPL_TYPES)
    void U32_EndianSwap(u32* number_to_endian_swap) {
        u32 temp = *number_to_endian_swap;
        
        u32 b0 = (temp >> 0) & 0xFF;
        u32 b1 = (temp >> 8) & 0xFF;
        u32 b2 = (temp >> 16) & 0xFF;
        u32 b3 = (temp >> 24) & 0xFF;

        *number_to_endian_swap = (b0 << 24)|(b1 << 16)|(b2 << 8)|(b3 << 0);
    } 

    void U64_EndianSwap(u64* number_to_endian_swap) {
        u64 temp = *number_to_endian_swap;
        
        u64 b0 = (temp >> 0) & 0xFF;
        u64 b1 = (temp >> 8) & 0xFF;
        u64 b2 = (temp >> 16) & 0xFF;
        u64 b3 = (temp >> 24) & 0xFF;
        u64 b4 = (temp >> 32) & 0xFF;
        u64 b5 = (temp >> 40) & 0xFF;
        u64 b6 = (temp >> 48) & 0xFF;
        u64 b7 = (temp >> 56) & 0xFF;

        *number_to_endian_swap = (b0 << 56)|(b1 << 48)|(b2 << 40)|(b3 << 32)|(b4 << 24)|(b5 << 16)|(b6 << 8)|(b7 << 0);
    }

    #if defined(PLATFORM_WINDOWS) && defined(_MSC_VER)
        #include <DbgHelp.h>
        #pragma comment(lib, "dbghelp")

        static const char* GetModuleNameFromAddress(HANDLE process, DWORD64 address) {
            static char moduleName[MAX_PATH];
            IMAGEHLP_MODULE64 moduleInfo;
            ZeroMemory(&moduleInfo, sizeof(moduleInfo));
            moduleInfo.SizeOfStruct = sizeof(moduleInfo);
            if (SymGetModuleInfo64(process, address, &moduleInfo)) {
                strncpy_s(moduleName, sizeof(moduleName), moduleInfo.ModuleName, _TRUNCATE);
                return moduleName;
            }
            return "Unknown Module";
        }

        void ckg_stack_trace_dump(const char* function, const char* file, int line) {
            printf("------------------ Error Stack Trace ------------------\n");
            void* stack[100];
            unsigned short max_frames = 100;
            unsigned short frames_captured;
            HANDLE process = GetCurrentProcess();
            BOOL sym_initialized = FALSE;

            sym_initialized = SymInitialize(process, NULL, TRUE);
            if (!sym_initialized) {
                printf("Failed to initialize symbol handler: %lu\n", GetLastError());
                printf("------------------ Error Stack Trace End ------------------\n");
                return;
            }

            frames_captured = CaptureStackBackTrace(0, max_frames, stack, NULL);
            if (frames_captured == 0) {
                printf("Failed to capture stack trace\n");
                SymCleanup(process);
                printf("------------------ Error Stack Trace End ------------------\n");
                return;
            }

            SYMBOL_INFO* symbol = (SYMBOL_INFO*)malloc(sizeof(SYMBOL_INFO) + (MAX_SYM_NAME * sizeof(char)));
            if (!symbol) {
                printf("Error allocating symbol info buffer\n");
                SymCleanup(process);
                printf("------------------ Error Stack Trace End ------------------\n");
                return;
            }

            symbol->MaxNameLen = MAX_SYM_NAME;
            symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

            IMAGEHLP_LINE64 line_info;
            line_info.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
            
            int skip = 2;
            for (int i = skip; i < frames_captured - 6; i++) {
                DWORD64 address = (DWORD64)stack[i];
                DWORD64 displacement = 0;
                DWORD line_displacement = 0;

                if (i == skip) {
                    printf("0: %s - %s:%u\n", function, file, line);
                    continue;
                } 
                
                if (SymFromAddr(process, address, &displacement, symbol)) {
                    if (SymGetLineFromAddr64(process, address, &line_displacement, &line_info)) {
                        printf("%d: %s - %s:%u\n", i - skip, symbol->Name, line_info.FileName, (int)line_info.LineNumber);
                    } else {
                        const char* module_name = GetModuleNameFromAddress(process, address);
                        printf("%d: %s [%s+0x%llx]\n", i - skip, symbol->Name, module_name, displacement);
                    }
                } else {
                    printf("%d: [Unknown symbol] 0x%p (Error: %lu)\n", i - skip, stack[i], GetLastError());
                }
            }

            free(symbol);
            SymCleanup(process);
            printf("------------------ Error Stack Trace End ------------------\n");
        }
    #elif defined(PLATFORM_LINUX)
        #include <stdio.h>
        #include <stdlib.h>
        #include <execinfo.h>
        #include <string.h>

        void ckg_stack_trace_dump(const char* function, const char* file, int line) {
            printf("------------------ Error Stack Trace ------------------\n");

            // Log the triggering function first
            printf("0: %s - %s:%u\n", function, file, line);

            void* array[100];
            int size = backtrace(array, 100);
            char** symbols = backtrace_symbols(array, size);

            if (symbols == NULL) {
                printf("Failed to get backtrace symbols\n");
                printf("------------------ Error Stack Trace End ------------------\n");
                return;
            }

            for (int i = 1; i < size; ++i) {
                char* symbol_str = symbols[i];
                char* name_begin = NULL;
                char* name_end = NULL;
                char* offset_begin = NULL;
                
                name_begin = strchr(symbol_str, '(');
                if (name_begin) {
                    name_begin++;
                    offset_begin = strchr(name_begin, '+');
                    if (offset_begin) {
                        name_end = offset_begin;
                        offset_begin++;
                    }
                }
                
                if (name_begin && name_end && name_end > name_begin) {
                    size_t name_len = name_end - name_begin;
                    char* symbol_name = (char*)malloc(name_len + 1);
                    if (symbol_name) {
                        memcpy(symbol_name, name_begin, name_len);
                        symbol_name[name_len] = '\0';
                        
                        printf("%d: %s\n", i, symbol_str);
                        
                        free(symbol_name);
                    } else {
                        printf("%d: %s\n", i, symbol_str);
                    }
                } else {
                    printf("%d: %s\n", i, symbol_str);
                }
            }

            free(symbols);
            printf("------------------ Error Stack Trace End ------------------\n");
        }
    #elif defined(PLATFORM_APPLE)
        #include <stdio.h>
        #include <stdlib.h>
        #include <execinfo.h>
        #include <dlfcn.h>

        void ckg_stack_trace_dump(const char* function, const char* file, int line) {
            printf("------------------ Error Stack Trace ------------------\n");
            printf("0: %s - %s:%u\n", function, file, line);

            void* array[100];
            int size = backtrace(array, 100);
            char** symbols = backtrace_symbols(array, size);
            
            if (symbols == NULL) {
                printf("Failed to get backtrace symbols\n");
                printf("------------------ Error Stack Trace End ------------------\n");
                return;
            }

            for (int i = 0; i < size - 1; ++i) {
                Dl_info info;
                if (dladdr(array[i], &info) && info.dli_sname) {
                    printf("%d: %s in %s\n", i, info.dli_sname, info.dli_fname ? info.dli_fname : "unknown");
                } else {
                    printf("%d: %s\n", i, symbols[i]);
                }
            }

            free(symbols);
            printf("------------------ Error Stack Trace End ------------------\n");
        }
    #else
        void ckg_stack_trace_dump(const char* function, const char* file, int line) {
            printf("------------------ Error Stack Trace ------------------\n");
            printf("0: %s - %s:%u\n", function, file, line);
            printf("Stack trace functionality not implemented for this platform\n");
            printf("------------------ Error Stack Trace End ------------------\n");
        }
    #endif
#endif

#if defined(CKG_IMPL_LOGGER)
    char LOGGER_START_DELIM[] = "${";
    char LOGGER_END_DELIM[] = "}";

    static const char* ckg_log_level_format[LOG_LEVEL_COUNT] = {
        CKG_RED_BACKGROUND,
        CKG_RED,
        CKG_PURPLE,
        CKG_BLUE,
        CKG_GREEN,
        CKG_COLOR_RESET
    };

    static bool __ckg_message_has_special_delmitor(const char* message, u64 message_length) {
        bool start_delimitor_index = ckg_str_contains(message, message_length, LOGGER_START_DELIM, sizeof(LOGGER_START_DELIM) - 1);
        bool end_delimitor_index = ckg_str_contains(message, message_length, LOGGER_END_DELIM, sizeof(LOGGER_END_DELIM) - 1);

        return start_delimitor_index && end_delimitor_index;
    }

    static void __ckg_special_print_helper(const char* message, u64 message_length, CKG_LogLevel log_level) {
        CKG_StringView middle_to_color = ckg_sv_between_delimiters(message, message_length, LOGGER_START_DELIM, sizeof(LOGGER_START_DELIM) - 1, LOGGER_END_DELIM, sizeof(LOGGER_END_DELIM) - 1);
        if (!middle_to_color.data) {
            bool found = message[message_length - 1] == '\n';
            printf("%.*s", (int)(message_length - found), message);
            return;
        }

        s64 start_delimitor_index = ckg_str_index_of(message, message_length, LOGGER_START_DELIM, sizeof(LOGGER_START_DELIM) - 1);
        s64 end_delimitor_index = ckg_str_index_of(message, message_length, LOGGER_END_DELIM, sizeof(LOGGER_END_DELIM) - 1);

        CKG_StringView left_side_view = ckg_sv_create(message, (u64)start_delimitor_index);
        CKG_StringView right_side_view = ckg_sv_create(message + ((u64)end_delimitor_index + (sizeof(LOGGER_END_DELIM) - 1)), (u64)message_length);

        printf("%.*s%s%.*s%s", (int)left_side_view.length, left_side_view.data, ckg_log_level_format[log_level], (int)middle_to_color.length, middle_to_color.data, CKG_COLOR_RESET);

        __ckg_special_print_helper(right_side_view.data, right_side_view.length, log_level);
    }

    void MACRO_ckg_log_output(CKG_LogLevel log_level, const char* message, ...) {
        va_list args_list;
        va_start(args_list, message);
        
        u64 out_message_length = 0;
        char* out_message = ckg_str_va_sprint(&out_message_length, message, args_list);
        
        va_end(args_list);
        
        if (__ckg_message_has_special_delmitor(out_message, out_message_length)) {
            __ckg_special_print_helper(out_message, out_message_length, log_level);
        } else {
            bool found = out_message[out_message_length - 1] == '\n';
            printf("%s%.*s%s", ckg_log_level_format[log_level], (int)(out_message_length - found), out_message, CKG_COLOR_RESET);
        }
        
        if (out_message[out_message_length - 1] == '\n') {
            printf("\n");
        }
        
        ckg_free(out_message);
    }
#endif

#if defined(CKG_IMPL_ASSERT)
    void* ckg_default_libc_malloc(CKG_Allocator* allocator, size_t allocation_size);
    void ckg_default_libc_free(CKG_Allocator* allocator, void* data);
    void MACRO_ckg_assert(bool expression, const char* function, const char* file, int line) {
        if (!expression) {               
            ckg_bind_custom_allocator(ckg_default_libc_malloc, ckg_default_libc_free, NULLPTR);                       
            ckg_stack_trace_dump(function, file, line);                               
            char msg[] = "Func: %s, File: %s:%d\n";          
            CKG_LOG_FATAL(msg, function, file, line);
            CRASH;                   
        }                                                         
    }

    void MACRO_ckg_assert_msg(bool expression, const char* function, const char* file, int line, const char* msg, ...) {  
        if (!(expression)) {            
            ckg_bind_custom_allocator(ckg_default_libc_malloc, ckg_default_libc_free, NULLPTR);                                  
            ckg_stack_trace_dump(function, file, line);                               
            char ckg_msg[] = "Func: %s, File: %s:%d\n";          
            CKG_LOG_FATAL(ckg_msg, function, file, line);

            va_list args;
            va_start(args, msg);
                CKG_LOG_FATAL("%s", ckg_str_va_sprint(NULLPTR, msg, args));   
            va_end(args); 

            CRASH;                                                
        }                                                                                 
    }
#endif

#if defined(CKG_IMPL_ERRORS)
    static const char* CKG_ERROR_IO_STRINGS[CKG_ERROR_IO_COUNT] = {
        stringify(CKG_ERROR_IO_RESOURCE_NOT_FOUND),
        stringify(CKG_ERROR_IO_RESOURCE_TOO_BIG)
    };

    static const char* CKG_ERROR_ARGS_STRINGS[CKG_ERROR_ARGS_COUNT] = {
        stringify(CKG_ERROR_ARG_ONE_INVALID),
        stringify(CKG_ERROR_ARG_ONE_NULLPTR),
        stringify(CKG_ERROR_ARG_ONE_ZERO),
        stringify(CKG_ERROR_ARG_TWO_INVALID),
        stringify(CKG_ERROR_ARG_TWO_NULLPTR),
        stringify(CKG_ERROR_ARG_TWO_ZERO),
        stringify(CKG_ERROR_ARG_THREE_INVALID),
        stringify(CKG_ERROR_ARG_THREE_NULLPTR),
        stringify(CKG_ERROR_ARG_THREE_ZERO),
        stringify(CKG_ERROR_ARG_FOUR_INVALID),
        stringify(CKG_ERROR_ARG_FOUR_NULLPTR),
        stringify(CKG_ERROR_ARG_FOUR_ZERO)
    };

    const char* ckg_error_str(CKG_Error error_code) {
        if (error_code == CKG_ERROR_SUCCESS) {
            return stringify(CKG_ERROR_SUCCESS);
        }

        int error_type = CKG_ERROR_TYPE_MASK & error_code;
        int error_index = (~CKG_ERROR_TYPE_MASK) & error_code;
            
        if (error_type == CKG_ERROR_IO_MASK) {
            return CKG_ERROR_IO_STRINGS[error_index];
        } else if (error_type == CKG_ERROR_ARGS_MASK) {
            return CKG_ERROR_ARGS_STRINGS[error_index];
        }

        return NULLPTR; // SHOULD NEVER GET HERE!
    }

    static void ckg_error_safe_set(CKG_Error* error, CKG_Error error_code) {
        if (error) {
            *error = error_code;
        }
    }
#endif 

#if defined(CKG_IMPL_MEMORY)
    void* ckg_default_libc_malloc(CKG_Allocator* allocator, size_t allocation_size) {
        (void)allocator;
        return malloc(allocation_size);
    }

    void ckg_default_libc_free(CKG_Allocator* allocator, void* data) {
        (void)allocator;
        free(data);
    }

    static CKG_Allocator global_allocator = {ckg_default_libc_malloc, ckg_default_libc_free, NULLPTR};

    void ckg_bind_custom_allocator(CKG_Alloc_T* a, CKG_Free_T* f, void* ctx) {
        ckg_assert_msg(a, "Alloc function is NULLPTR\n");
        ckg_assert_msg(f, "Free function is NULLPTR\n");
        
        global_allocator.allocate = a;
        global_allocator.free = f;
        global_allocator.ctx = ctx;
    }

    void* ckg_alloc(size_t allocation_size) {
        ckg_assert(allocation_size != 0);

        void* ret = global_allocator.allocate(&global_allocator, allocation_size);
        ckg_memory_zero(ret, allocation_size);
        return ret;
    }

    void* MACRO_ckg_free(void* data) {
        ckg_assert(data);
        
        global_allocator.free(&global_allocator, data);
        return NULLPTR;
    }

    void* ckg_realloc(void* data, size_t old_allocation_size, size_t new_allocation_size) {
        ckg_assert(old_allocation_size != 0);
        ckg_assert(new_allocation_size != 0);

        void* ret = ckg_alloc(new_allocation_size);
        ckg_memory_copy(ret, new_allocation_size, data, old_allocation_size);
        ckg_free(data);
        return ret;
    }

    bool ckg_memory_compare(const void* buffer_one, size_t b1_size, const void* buffer_two, size_t b2_size) {
        ckg_assert(buffer_one);
        ckg_assert(buffer_two);

        if (b1_size != b2_size) {
            return false;
        }

        u8* buffer_one_data = (u8*)buffer_one;
        u8* buffer_two_data = (u8*)buffer_two;
        for (size_t i = 0; i < b1_size; i++) {
            if (buffer_one_data[i] != buffer_two_data[i]) {
                return false;
            }
        }

        return true;
    }

    void ckg_memory_copy(void* destination, size_t destination_size, const void* source, size_t source_size) {
        ckg_assert(source);
        ckg_assert(destination);
        ckg_assert(source_size <= destination_size);
        if (source_size == 0) {
            return;
        }

        u8* src = (u8*)source;
        u8* dst = (u8*)destination;

        bool overlap = dst < src || dst >= src + source_size;
        if (overlap) {
            for (size_t i = 0; i < source_size; i++) {
                dst[i] = src[i];
            }
        } else {
            for (size_t i = source_size; i-- > 0;) {
                dst[i] = src[i];
            }
        }
    }

    void ckg_memory_zero(void* data, size_t data_size_in_bytes) {
        for (size_t i = 0; i < data_size_in_bytes; i++) {
            ((u8*)data)[i] = 0;
        }
    }

    void MACRO_ckg_memory_delete_index(void* data, int number_of_elements, int data_capacity, size_t element_size_in_bytes, int index) {
        ckg_assert(index >= 0);
        ckg_assert(index < data_capacity);

        u8* byte_data = (u8*)data;

        size_t total_size = element_size_in_bytes * data_capacity;
        size_t source_offset = (index + 1) * element_size_in_bytes;
        size_t dest_offset =  index * element_size_in_bytes;

        size_t payload_source_size = (number_of_elements * element_size_in_bytes) - source_offset;
        ckg_memory_copy(byte_data + dest_offset, total_size - source_offset, byte_data + source_offset, payload_source_size);
    }

    void MACRO_ckg_memory_insert_index(void* data, int number_of_elements, int data_capacity, size_t element_size_in_bytes, int index) {
        ckg_assert((number_of_elements + 1) < data_capacity);
        ckg_assert(index < data_capacity - 1);

        u8* byte_data = (u8*)data;

        size_t total_size = element_size_in_bytes * data_capacity;
        size_t source_offset = index * element_size_in_bytes;
        size_t dest_offset = (index + 1) * element_size_in_bytes;

        size_t payload_source_size = (number_of_elements * element_size_in_bytes) - source_offset;
        ckg_memory_copy(byte_data + dest_offset, total_size - source_offset, byte_data + source_offset, payload_source_size);
    }
#endif

#if defined(CKG_IMPL_ARENA)
    CKG_Arena ckg_arena_create_custom(void* memory, size_t allocation_size, int flags, u8 alignment) {
        ckg_assert_msg(memory, "Memory can't be a null pointer!\n");
        ckg_assert_msg(allocation_size != 0, "Can't have a zero allocation size!\n");
        ckg_assert_msg(!((flags & CKG_ARENA_FLAG_CIRCULAR) && (flags & CKG_ARENA_FLAG_FIXED)), "Can't have both a fixed an circular arena!\n");

        CKG_Arena arena;
        arena.flags = flags;
        arena.used = 0;
        arena.used_save_point = 0;
        arena.capacity = allocation_size;
        arena.alignment = alignment;
        arena.base_address = (u8*)memory;
        arena.size_stack = NULLPTR;

        return arena;
    }
    
    void ckg_arena_free(CKG_Arena* arena) {
        ckg_assert_msg(arena->flags != CKG_ARENA_FLAG_INVALID, "Arena is invalid!\n");

        if (!(arena->flags & CKG_ARENA_FLAG_STACK_MEMORY)) {
            ckg_free(arena->base_address);
        }

        if (arena->size_stack) {
            CKG_Alloc_T* a = global_allocator.allocate;
            CKG_Free_T* f  = global_allocator.free;
            void* ctx  = global_allocator.ctx;
            ckg_bind_custom_allocator(ckg_default_libc_malloc, ckg_default_libc_free, NULLPTR);
            ckg_stack_free(arena->size_stack);
            ckg_bind_custom_allocator(a, f, ctx);
        }

        arena->flags = CKG_ARENA_FLAG_INVALID;
    }

    void ckg_arena_zero(CKG_Arena* arena) {
        ckg_assert_msg(arena->flags != CKG_ARENA_FLAG_INVALID, "Arena is invalid!\n");

        ckg_memory_zero(arena->base_address, arena->capacity);
        arena->used = 0;
    }

    void ckg_arena_reset(CKG_Arena* arena) {
        ckg_assert_msg(arena->flags != CKG_ARENA_FLAG_INVALID, "Arena is invalid!\n");

        int initial_count = ckg_stack_count(arena->size_stack);
        for (int i = 0; i < initial_count; i++) {
            ckg_stack_pop(arena->size_stack);
        }
        arena->used = 0;
    }

    void* ckg_arena_push_custom(CKG_Arena* arena, size_t element_size) {
        ckg_assert_msg(arena->flags != CKG_ARENA_FLAG_INVALID, "Arena is invalid!\n");
        ckg_assert_msg(element_size != 0, "Element size can't be zero!\n");

        if (arena->flags & CKG_ARENA_FLAG_FIXED) {
            ckg_assert_msg(arena->used + element_size <= arena->capacity, "Ran out of arena memory!\n");
        } else if (arena->flags & CKG_ARENA_FLAG_CIRCULAR) {
            if ((arena->used + element_size > arena->capacity)) {
                arena->used = 0;
                ckg_assert_msg(arena->used + element_size <= arena->capacity, "Element size exceeds circular arena allocation capacity!\n");
            }
        }

        u8* ret = arena->base_address + arena->used;
        size_t previous_used = arena->used;
        arena->used += element_size;
        if ((arena->used & (arena->alignment - 1)) != 0) { // if first bit is set then its not aligned
            arena->used += (arena->alignment - (arena->used & (arena->alignment - 1)));
        }

        CKG_Alloc_T* a = global_allocator.allocate;
        CKG_Free_T* f  = global_allocator.free;
        void* ctx  = global_allocator.ctx;
        ckg_bind_custom_allocator(ckg_default_libc_malloc, ckg_default_libc_free, NULLPTR);
        ckg_stack_push(arena->size_stack, arena->used - previous_used);
        ckg_bind_custom_allocator(a, f, ctx);

        return ret;
    }

    void MACRO_ckg_arena_pop(CKG_Arena* arena, void* data, size_t bytes_to_pop) {
        ckg_assert_msg(arena->flags != CKG_ARENA_FLAG_INVALID, "Arena is invalid!\n");

        if (data) {
            // ckg_assert_msg(arena->base_address + (arena->used - bytes_to_pop) == data, "The address you are trying to pop is not the top most data\n");
            if (arena->base_address + (arena->used - bytes_to_pop) == data) {
                arena->used -= bytes_to_pop;
            }
        } else {
            arena->used -= bytes_to_pop;
        }
    }

    void ckg_arena_begin_temp(CKG_Arena* arena) {
        ckg_assert_msg(arena->flags != CKG_ARENA_FLAG_INVALID, "Arena is invalid!\n");

        arena->used_save_point = arena->used;
    }

    void ckg_arena_end_temp(CKG_Arena* arena) {
        ckg_assert_msg(arena->flags != CKG_ARENA_FLAG_INVALID, "Arena is invalid!\n");
        ckg_memory_zero(arena->base_address + arena->used_save_point, arena->capacity - arena->used_save_point);

        arena->used = arena->used_save_point;
        arena->used_save_point = 0;
    }
#endif

#if defined(CKG_IMPL_STRING)
    char* ckg_str_alloc(const char* s1, u64 length) {
        char* ret = (char*)ckg_alloc(length + 1) ;
        ckg_memory_copy(ret, length, s1, length);
        return ret;
    }

    u64 ckg_cstr_length(const char* cstring) {
        ckg_assert(cstring);

        if (!cstring) {
            return 0; // This should never get here but the compiler wants this
        }

        u64 length = 0;
        const char* cursor = cstring;
        while(*cursor++ != '\0') {
            length++;
        }

        return length;
    }

    void ckg_str_clear(char* s1, u64 length) {
        ckg_memory_zero(s1, length);
    }

    CKG_StringView ckg_sv_create(const char* data, u64 length) {
        ckg_assert(data);

        CKG_StringView ret;
        ret.data = data;
        ret.length = length;

        return ret;
    }

    CKG_StringView ckg_sv_between_delimiters(const char* str, u64 str_length, const char* start_delimitor, u64 start_delimitor_length, const char* end_delimitor, u64 end_delimitor_length) {
        ckg_assert(str);
        ckg_assert(start_delimitor);
        ckg_assert(end_delimitor);
        ckg_assert(!ckg_str_equal(start_delimitor, start_delimitor_length, end_delimitor, end_delimitor_length));

        s64 start_delimitor_index = ckg_str_index_of(str, str_length, start_delimitor, start_delimitor_length); 
        s64 end_delimitor_index = ckg_str_index_of(str, str_length, end_delimitor, end_delimitor_length);
        if (start_delimitor_index == -1 || end_delimitor_index == -1) {
            return CKG_SV_NULL();
        }

        if (start_delimitor_index == -1 || end_delimitor_index == -1) {
            return CKG_SV_NULL();
        } else if (start_delimitor_index > end_delimitor_index) {
            return CKG_SV_NULL(); // The start delimtor is after the end delimitor
        }

        CKG_StringView ret = CKG_SV_NULL();
        u64 start_str_index = (u64)((u64)start_delimitor_index + start_delimitor_length);
        ret.data = str + start_str_index;
        ret.length = (u64)end_delimitor_index - start_str_index;

        return ret;
    }

    CKG_StringView* ckg_sv_split(const char* data, u64 length, const char* delimitor, u64 delimitor_length) {
        ckg_assert(data);
        ckg_assert(delimitor);
        ckg_assert_msg(delimitor_length > 0, "delimitor can not be a empty string!\n");

        if (length == 0) {
            CKG_StringView* ret_vector = NULLPTR;
            CKG_StringView current = ckg_sv_create(data, length);
            ckg_vector_push(ret_vector, current);

            return ret_vector;
        }

        CKG_StringView* ret_vector = NULLPTR;
        CKG_StringView str_view = ckg_sv_create(data, length);
        while (true) {
            s64 found_index = ckg_str_index_of(str_view.data, str_view.length, delimitor, delimitor_length);
            if (found_index == -1) {
                ckg_vector_push(ret_vector, str_view);
                return ret_vector;
            }

            if (found_index == 0) {
                ckg_vector_push(ret_vector, CKG_SV_NULL());
            } else {
                CKG_StringView substring = ckg_sv_create(str_view.data, (u64)found_index);
                ckg_vector_push(ret_vector, substring);
            }

            str_view.data += ((u64)found_index + 1);
            str_view.length -= ((u64)found_index + 1);
        }

        return ret_vector;
    }

    bool ckg_str_equal(const char* s1, u64 s1_length, const char* s2, u64 s2_length) {
        return ckg_memory_compare(s1, s1_length, s2, s2_length);
    }

    void ckg_str_copy(char* s1, size_t s1_capacity, const char* s2, u64 s2_length) {
        ckg_memory_zero(s1, s1_capacity);
        ckg_memory_copy(s1, s1_capacity, s2, s2_length);
    }

    void ckg_str_insert(char* str, u64* str_length_out, size_t str_capacity, const char* to_insert, u64 to_insert_length, u64 index) {
        ckg_assert(str);
        ckg_assert(to_insert);
        ckg_assert(str_length_out);

        u64 new_length = *str_length_out + to_insert_length;
        ckg_assert_msg(*str_length_out < str_capacity, "ckg_str_insert: str_capacity is %lld but new valid cstring length is %d + %d + 1(null_term) = %d\n", str_capacity, *str_length_out, to_insert_length, new_length + 1);
        
        u8* move_source_ptr = (u8*)(str + index);
        u8* move_dest_ptr = (u8*)(move_source_ptr + to_insert_length);

        ckg_memory_copy(move_dest_ptr, str_capacity - (index + to_insert_length), move_source_ptr, *str_length_out - index);
        u8* copy_dest_ptr = (u8*)(str + index);
        ckg_memory_copy(copy_dest_ptr, str_capacity, to_insert, to_insert_length);

        *str_length_out += to_insert_length;
    }

    void ckg_str_insert_char(char* str, u64* str_length_out, size_t str_capacity, char to_insert, u64 index) {
        ckg_assert(str);
        ckg_assert(to_insert);
        ckg_assert(str_length_out);

        u64 to_insert_length = 1;
        u64 new_length = *str_length_out + to_insert_length;

        ckg_assert_msg(*str_length_out < str_capacity, "ckg_str_insert_char: str overflow new_capacity_required: %d >= current_capacity: %lld\n",  new_length, str_capacity);

        char* source_ptr = str + index;
        ckg_memory_copy(source_ptr + 1, str_capacity - (index + 1), source_ptr, *str_length_out - index);
        str[index] = to_insert;

        *str_length_out += to_insert_length;
    }

    void ckg_str_append(char* str, u64* str_length_out, size_t str_capacity, const char* to_append, u64 to_append_length) {
        ckg_str_insert(str, str_length_out, str_capacity, to_append, to_append_length, *str_length_out);
    }

    void ckg_str_append_char(char* str, u64* str_length_out, size_t str_capacity, char to_append) {
        ckg_str_insert_char(str, str_length_out, str_capacity, to_append, *str_length_out);
    }

    char* ckg_str_reverse(const char* str, u64 str_length) {
        ckg_assert(str);

        char* ret = (char*)ckg_alloc(str_length + 1);
        u64 length = 0;
        for (s64 i = (s64)str_length - 1; i >= 0; i--) {
            ckg_str_append_char(ret, &length, str_length + 1, str[i]);
        }

        return ret;
    }

    s64 ckg_str_index_of(const char* str, u64 str_length, const char* substring, u64 substring_length) {
        ckg_assert(str);
        ckg_assert(substring);

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

            CKG_StringView current_view = ckg_sv_create(str + i, substring_length);
            if (ckg_str_equal(substring, substring_length, current_view.data, current_view.length)) {
                ret_index = (s64)i;
                break;
            }
        }

        return ret_index; // returns -1 if not found
    }

    bool ckg_str_contains(const char* str, u64 str_length, const char* contains, u64 contains_length) {
        ckg_assert(str);
        ckg_assert(contains);

        return ckg_str_index_of(str, str_length, contains, contains_length) != -1;
    }

    s64 ckg_str_last_index_of(const char* str, u64 str_length, const char* substring, u64 substring_length) {
        ckg_assert(str);
        ckg_assert(substring);

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

            CKG_StringView current_view = ckg_sv_create(str + i, substring_length);
            if (ckg_str_equal(current_view.data, current_view.length, substring, substring_length)) {
                ret_index = (s64)i;
            }
        }

        return ret_index;
    }

    bool ckg_str_starts_with(const char* str, u64 str_length, const char* starts_with, u64 starts_with_length) {
        ckg_assert(str);
        ckg_assert(starts_with);

        if (str_length == 0 && starts_with_length == 0) {
            return true;
        } else if (str[0] != starts_with[0] || str_length < starts_with_length) {
            return false;
        }
        
        if (ckg_str_equal(str, starts_with_length, starts_with, starts_with_length)) {
            return true;
        }

        return false;
    }

    bool ckg_str_ends_with(const char* str, u64 str_length, const char* ends_with, u64 ends_with_length) {
        ckg_assert(str);
        ckg_assert(ends_with);

        if ((str_length == 0) || (ends_with_length == 0)) {
            return true;
        } else if (str_length < ends_with_length) {
            return false;
        }

        u64 start_index = MAX(str_length - 1, 0) - MAX(ends_with_length - 1, 0);
        if (str[start_index] != ends_with[0]) {
            return false;
        }

        if (ckg_str_equal(str + start_index, ends_with_length, ends_with, ends_with_length)) {
            return true;
        }

        return false;
    }

    char* ckg_str_va_sprint(u64* str_length_ptr_back, const char* fmt, va_list args) {
        va_list args_copy;
        va_copy(args_copy, args);
        u64 allocation_ret = (u64)vsnprintf(NULLPTR, 0, fmt, args_copy) + 1; // +1 for null terminator
        va_end(args_copy);

        char* buffer = (char*)ckg_alloc(allocation_ret);

        va_copy(args_copy, args);
        vsnprintf(buffer, allocation_ret, fmt, args_copy);
        va_end(args_copy);

        if (str_length_ptr_back != NULLPTR) {
            *str_length_ptr_back = allocation_ret - 1;
        }

        return buffer;
    }

    char* MACRO_ckg_str_sprint(u64* str_length_ptr_back, const char* fmt, ...) {
        va_list args;
        va_start(args, fmt);
        char* ret = ckg_str_va_sprint(str_length_ptr_back, fmt, args);
        va_end(args);

        return ret;
    }
#endif

#if defined(CKG_IMPL_CHAR)
    bool ckg_char_is_alpha(char c) {
        c = (char)((int)c & (0b11011111)); // mask off the 32 bit
        return ckg_char_is_upper(c);
    }

    bool ckg_char_is_alpha_numeric(char c) {
        return ckg_char_is_alpha(c) || ckg_char_is_digit(c);
    }
#endif


#if defined(CKG_IMPL_COLLECTIONS)
    //
    // ========== START CKG_VECTOR ==========
    //
    void* ckg_vector_grow(void* vector, size_t element_size, int capacity) {
        if (vector == NULLPTR) {
            int real_capacity = (capacity > 0 ? capacity : VECTOR_DEFAULT_CAPACITY);
            vector = ckg_alloc(sizeof(CKG_VectorHeader) + (real_capacity * element_size));
            vector = (u8*)vector + sizeof(CKG_VectorHeader);
            ckg_vector_header_base(vector)->capacity = real_capacity;
            ckg_vector_header_base(vector)->element_size = element_size;
        }

        int count = ckg_vector_count(vector);
        int capactiy = ckg_vector_capacity(vector);

        if (capactiy < count + 1) {
            size_t old_allocation_size = sizeof(CKG_VectorHeader) + (capactiy * element_size);
            int new_capactiy = capactiy * 2;
            size_t new_allocation_size = sizeof(CKG_VectorHeader) + (new_capactiy * element_size);

            vector = ckg_realloc(ckg_vector_header_base(vector), old_allocation_size, new_allocation_size);
            vector = (u8*)vector + sizeof(CKG_VectorHeader);

            ckg_vector_header_base(vector)->element_size = element_size;
            ckg_vector_header_base(vector)->count = count;
            ckg_vector_header_base(vector)->capacity = new_capactiy;
        }

        return vector;
    }

    void* MACRO_ckg_vector_free(void* vector) {
        CKG_VectorHeader* vector_base = ckg_vector_header_base(vector);
        vector_base->count = 0;
        vector_base->capacity = 0;
        ckg_free(vector_base);

        return vector_base;
    }
    //
    // ========== END CKG_VECTOR ==========
    //

    //
    // ========== START CKG_RingBuffer ==========
    //
    // Date: March 22, 2025
    // NOTE(Jovanni): Do we actually need this at all because we have a circular arena?
    void* ckg_ring_buffer_create(size_t element_size, int capacity) {
        ckg_assert(capacity > 0);

        size_t allocation_size = sizeof(CKG_RingBufferHeader) + (capacity * element_size);
        CKG_RingBufferHeader* header = (CKG_RingBufferHeader*)ckg_alloc(allocation_size);
        header->element_size = element_size;
        header->capacity = capacity;

        void* buffer = (u8*)header + sizeof(CKG_RingBufferHeader);
        return buffer;
    }

    void* MACRO_ckg_ring_buffer_free(void* vector) {
        CKG_RingBufferHeader* ring_base = ckg_ring_buffer_header_base(vector);
        ring_base->count = 0;
        ring_base->capacity = 0;
        ring_base->read = 0;
        ring_base->write = 0;
        ckg_free(vector);

        return NULLPTR;
    }
    //
    // ========== END CKG_RingBuffer ==========
    //

    //
    // ========== START CKG_LinkedList ==========
    //
    CKG_LinkedList* MACRO_ckg_linked_list_create(size_t element_size_in_bytes, bool is_pointer_type) {
        CKG_LinkedList* ret = (CKG_LinkedList*)ckg_alloc(sizeof(CKG_LinkedList));
        ret->count = 0;
        ret->element_size_in_bytes = element_size_in_bytes;
        ret->head = NULLPTR;
        ret->tail = NULLPTR;
        ret->is_pointer_type = is_pointer_type;
        return ret;
    }

    CKG_Node* MACRO_ckg_node_create(CKG_LinkedList* linked_list, void* data) {
        CKG_Node* ret = (CKG_Node*)ckg_alloc(sizeof(CKG_Node));
        if (linked_list->is_pointer_type) {
            ret->data = data;
        } else {
            ret->data = ckg_alloc(linked_list->element_size_in_bytes); // user has to free
            ckg_memory_copy(ret->data, linked_list->element_size_in_bytes, data, linked_list->element_size_in_bytes); 
        }

        ret->element_size_in_bytes = linked_list->element_size_in_bytes;
        ret->next = NULLPTR;
        ret->prev = NULLPTR;
        return ret;
    }
    #define ckg_node_create(linked_list, data) MACRO_ckg_node_create(linked_list, data)

    /**
     * @brief returns a null ptr
     * 
     * @param node 
     * @return CKG_Node* 
     */
    static CKG_Node* MACRO_ckg_node_free(CKG_LinkedList* linked_list, CKG_Node* node) {
        ckg_assert(linked_list);
        ckg_assert(node);
        node->element_size_in_bytes = 0;
        node->next = NULLPTR;
        node->prev = NULLPTR;
        ckg_free(node);

        return node;
    }
    #define ckg_node_free(linked_list, node) node = MACRO_ckg_node_free(linked_list, node)


    /**
     * @brief returns a null ptr
     * 
     * @param node 
     * @return CKG_Node* 
     */
    static CKG_Node* MACRO_ckg_node_data_free(CKG_LinkedList* linked_list, CKG_Node* node) {
        ckg_assert(linked_list);
        ckg_assert(node);
        ckg_assert(node->data);
        node->element_size_in_bytes = 0;
        node->next = NULLPTR;
        node->prev = NULLPTR;
        if (!linked_list->is_pointer_type) {
            ckg_free(node->data);
        }
        ckg_free(node);

        return node;
    }
    #define ckg_node_data_free(linked_list, node) node = MACRO_ckg_node_data_free(linked_list, node)

    CKG_Node* ckg_linked_list_insert(CKG_LinkedList* linked_list, u64 index, void* data) {
        ckg_assert(linked_list);
        ckg_assert(data);

        u64 old_count = linked_list->count++;
        if (linked_list->head == NULLPTR) { // there is not a head and by definition no tail
            CKG_Node* new_node_to_insert = ckg_node_create(linked_list, data);
            linked_list->head = new_node_to_insert;
            linked_list->tail = new_node_to_insert;

            return linked_list->head;
        }

        ckg_assert(index <= old_count);
        CKG_Node* new_node_to_insert = ckg_node_create(linked_list, data);

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

        CKG_Node* current_node = linked_list->head; 
        for (size_t i = 0; i < index; i++) {
            current_node = current_node->next;
        }

        new_node_to_insert->prev = current_node;
        new_node_to_insert->next = current_node->next;

        current_node->next->prev = new_node_to_insert;
        current_node->next = new_node_to_insert;

        return new_node_to_insert;
    }

    CKG_Node* ckg_linked_list_get_node(CKG_LinkedList* linked_list, u64 index) {
        ckg_assert(linked_list);
        CKG_Node* current_node = linked_list->head; 
        for (size_t i = 0; i < index; i++) {
            current_node = current_node->next;
        }

        return current_node;
    }

    void* ckg_linked_list_get(CKG_LinkedList* linked_list, u64 index) {
        return ckg_linked_list_get_node(linked_list, index)->data;
    }

    void* ckg_linked_list_peek_head(CKG_LinkedList* linked_list) {
        return ckg_linked_list_get_node(linked_list, 0)->data;
    }

    void* ckg_linked_list_peek_tail(CKG_LinkedList* linked_list) {
        return ckg_linked_list_get_node(linked_list, linked_list->count - 1)->data;
    }

    CKG_Node* ckg_linked_list_push(CKG_LinkedList* linked_list, void* data) {
        return ckg_linked_list_insert(linked_list, linked_list->count, data);
    }

    size_t ckg_linked_list_node_to_index(CKG_LinkedList* linked_list, CKG_Node* address) {
        CKG_Node* current_node = linked_list->head; 
        for (size_t i = 0; i < linked_list->count + 1; i++) {
            if (current_node == address) {
                return i;
            }
            current_node = current_node->next;
        }

        ckg_assert(false); // couldn't match a node to an address
        return 0; // should never get here
    }

    CKG_Node ckg_linked_list_pop(CKG_LinkedList* linked_list) {
        return ckg_linked_list_remove(linked_list, linked_list->count - 1);
    }

    CKG_Node ckg_linked_list_remove(CKG_LinkedList* linked_list, u64 index) {
        ckg_assert(linked_list); 
        ckg_assert(linked_list->count > 0); 

        size_t old_count = linked_list->count--;
        if (index == 0 && old_count == 1) { // removing the head fully
            CKG_Node ret = *linked_list->head;
            ckg_node_free(linked_list, linked_list->head);
            linked_list->head = NULLPTR;

            return ret;
        }

        if (index == 0) { // remove head
            CKG_Node* cached_head = linked_list->head;
            linked_list->head = linked_list->head->next;
            CKG_Node ret = *cached_head; 
            ckg_node_free(linked_list, cached_head);

            return ret;
        }

        ckg_assert(index < old_count);
        if (index == (old_count - 1)) { // remove tail
            CKG_Node* cached_tail = linked_list->tail;
            linked_list->tail = linked_list->tail->prev;
            CKG_Node ret = *cached_tail; 
            ckg_node_free(linked_list, cached_tail);

            return ret;
        }

        CKG_Node* current_node = linked_list->head; 
        for (size_t i = 0; i < index; i++) {
            current_node = current_node->next;
        }

        current_node->next->prev = current_node->prev;
        current_node->prev->next = current_node->next;
        CKG_Node ret = *current_node; 
        ckg_node_free(linked_list, current_node);

        return ret;
    }

    void* MACRO_ckg_linked_list_free(CKG_LinkedList* linked_list) {
        ckg_assert(linked_list); 
        CKG_Node* current_node = linked_list->head; 
        CKG_Node* next_node = NULLPTR; 
        for (size_t i = 0; i < linked_list->count; i++) {
            next_node = current_node->next;
            ckg_node_data_free(linked_list, current_node);
            current_node = next_node;
        }
        ckg_free(linked_list);

        return linked_list;
    }
    //
    // ========== END CKG_LinkedList ==========
    //


    //
    // ========== Start CKG_HashMap ==========
    //
    #if !defined(__cplusplus)
        // Original location:
        // https://github.com/majek/csiphash/1

        #if defined(_WIN32)
            #define _le64toh(x) ((uint64_t)(x))
        #elif defined(__APPLE__)
            #include <libkern/OSByteOrder.h>
            #define _le64toh(x) OSSwapLittleToHostInt64(x)
        #else
            #if defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__)
                #include <sys/endian.h>
            #else
                #include <endian.h>
            #endif

            #if defined(__BYTE_ORDER) && defined(__LITTLE_ENDIAN) && __BYTE_ORDER == __LITTLE_ENDIAN   
                #define _le64toh(x) ((uint64_t)(x))
            #else
                #define _le64toh(x) le64toh(x)
            #endif
        #endif

        #define ROTATE(x, b) (uint64_t)( ((x) << (b)) | ( (x) >> (64 - (b))) )

        #define HALF_ROUND(a,b,c,d,s,t)	\
            a += b; c += d;			    \
            b = ROTATE(b, s) ^ a;		\
            d = ROTATE(d, t) ^ c;		\
            a = ROTATE(a, 32);          \

        #define DOUBLE_ROUND(v0,v1,v2,v3)  \
            HALF_ROUND(v0,v1,v2,v3,13,16); \
            HALF_ROUND(v2,v1,v0,v3,17,21); \
            HALF_ROUND(v0,v1,v2,v3,13,16); \
            HALF_ROUND(v2,v1,v0,v3,17,21)

        uint64_t siphash24(void* source, u64 source_size) {
            const char key[16] = {
                0x00, 0x01, 0x02, 0x03,
                0x04, 0x05, 0x06, 0x07,
                0x08, 0x09, 0x0A, 0x0B,
                0x0C, 0x0D, 0x0E, 0x0F
            };

            const uint64_t *_key = (const uint64_t*)key;
            uint64_t k0 = _le64toh(_key[0]);
            uint64_t k1 = _le64toh(_key[1]);
            uint64_t b = (uint64_t)source_size << 56;
            const uint64_t* in = (const uint64_t*)source;

            uint64_t v0 = k0 ^ 0x736f6d6570736575ULL;
            uint64_t v1 = k1 ^ 0x646f72616e646f6dULL;
            uint64_t v2 = k0 ^ 0x6c7967656e657261ULL;
            uint64_t v3 = k1 ^ 0x7465646279746573ULL;

            while (source_size >= 8) {
                uint64_t mi = _le64toh(*in);
                in += 1; source_size -= 8;
                v3 ^= mi;
                DOUBLE_ROUND(v0,v1,v2,v3);
                v0 ^= mi;
            }

            uint64_t t = 0; 
            uint8_t* pt = (uint8_t*)&t; 
            const uint8_t* m = (const uint8_t*)in;
            switch (source_size) {
                case 7: pt[6] = m[6];
                case 6: pt[5] = m[5];
                case 5: pt[4] = m[4];
                case 4: *((uint32_t*)&pt[0]) = *((uint32_t*)&m[0]); break;
                case 3: pt[2] = m[2];
                case 2: pt[1] = m[1];
                case 1: pt[0] = m[0];
            }
            b |= _le64toh(t);

            v3 ^= b;
            DOUBLE_ROUND(v0,v1,v2,v3);
            v0 ^= b; v2 ^= 0xff;
            DOUBLE_ROUND(v0,v1,v2,v3);
            DOUBLE_ROUND(v0,v1,v2,v3);

            u64 ret = (v0 ^ v1) ^ (v2 ^ v3);
            return ret;
        }

        bool byte_equality(void* c1, size_t c1_size, void* c2, size_t c2_size) {
            return ckg_memory_compare(c1, c1_size, c2, c2_size);
        }

        float ckg_hashmap_load_factor(void* map) {
            CKG_HashMapMeta* meta = (CKG_HashMapMeta*)map;
            return ((float)meta->count + (float)meta->dead_count) / (float)meta->capacity;
        }

        s64 ckit_hashmap_resolve_collision(void* map, void* key, u64 inital_hash_index) {
            CKG_HashMapMeta* meta = (CKG_HashMapMeta*)map;
            u8* entries_base_address = NULLPTR;
            ckg_memory_copy(&entries_base_address, sizeof(void*), (u8*)map + meta->entries_offset, sizeof(void*));

            s64 cannonical_hash_index = inital_hash_index;

            u64 visited_count = 0;
            while (true) {
                u8* entry = entries_base_address + (cannonical_hash_index * meta->entry_size);
                u8* entry_key = NULLPTR;
                if (meta->key_is_ptr) {
                    ckg_memory_copy(&entry_key, sizeof(void*), entry + meta->entry_key_offset, sizeof(void*));
                } else {
                    entry_key = entry + meta->entry_key_offset;
                }

                bool entry_filled = *(entry + meta->entry_filled_offset);
                if (!entry_filled) {
                    break;
                }

                bool equality_match = meta->equal_fn(entry_key, meta->key_size, key, meta->key_size);
                if (equality_match) {
                    break;
                }

                if (visited_count > meta->capacity) {
                    return -1;
                }

                visited_count += 1;
                cannonical_hash_index += 1;
                cannonical_hash_index = cannonical_hash_index % meta->capacity;
            }

            return cannonical_hash_index;
        }

        u64 ckg_string_hash(void* str, u64 str_length) {
            (void)str_length;

            u64 hash = 5381;
            u8* str_ptr = str;
            int c;

            while ((c = *str_ptr++)) {
                hash = ((hash << 5) + hash) + c;
            }

            return hash;
        }

        bool string_equality(void* c1, size_t c1_size, void* c2, size_t c2_size) {
            (void)c1_size;
            (void)c2_size;
            
            return ckg_str_equal(c1, ckg_cstr_length(c1), c2, ckg_cstr_length(c2));
        }

        u64 ckg_string_view_hash(void* view, u64 str_length) {
            (void)str_length;
            CKG_StringView* str_view = (CKG_StringView*)view;
            u64 hash = 5381;
            int c;

            for (u64 i = 0; i < str_view->length; i++) {
                c = str_view->data[i];
                hash = ((hash << 5) + hash) + c;
            }

            return hash;
        }

        bool string_view_equality(void* c1, size_t c1_size, void* c2, size_t c2_size) {
            (void)c1_size;
            (void)c2_size;

            CKG_StringView* s1 = (CKG_StringView*)c1;
            CKG_StringView* s2 = (CKG_StringView*)c2;

            return ckg_str_equal(s1->data, s1->length, s2->data, s2->length);
        }

        // Date: Aug 27th, 2025
        // TODO(Jovanni): This context is just weird it really shouldn't be this complex?
        typedef struct HashMapContext {
            CKG_HashMapMeta* meta;
            void* temp_key_address;
            void* entry; // This is the actual entry im talking about
            void* entry_key_address;
            void* entry_value_address;
            bool* entry_filled_address;
            bool* entry_dead_address;
            s64 real_index;
        } HashMapContext;

        static HashMapContext ckg_hashmap_get_context(void* map) {
            HashMapContext context;
            context.meta = (CKG_HashMapMeta*)map;
            context.temp_key_address = NULLPTR;

            if (context.meta->key_is_ptr) {
                ckg_memory_copy(&context.temp_key_address, sizeof(void*), (u8*)map + context.meta->temp_key_offset, sizeof(void*));
            } else {
                context.temp_key_address = (u8*)map + context.meta->temp_key_offset;
            }

            u64 hash = context.meta->hash_fn(context.temp_key_address, context.meta->key_size);
            u64 index = hash % context.meta->capacity;
            context.real_index = ckit_hashmap_resolve_collision(map, context.temp_key_address, index);

            u8* entries = NULLPTR;
            ckg_memory_copy(&entries, sizeof(void*), (u8*)map + context.meta->entries_offset,  sizeof(void*));
            context.entry = entries + (context.real_index * context.meta->entry_size);
            context.entry_key_address = (u8*)context.entry + context.meta->entry_key_offset;
            context.entry_value_address = (u8*)context.entry + context.meta->entry_value_offset;
            context.entry_filled_address = (bool*)((u8*)context.entry + context.meta->entry_filled_offset);
            context.entry_dead_address = (bool*)((u8*)context.entry + context.meta->entry_dead_offset);

            return context;
        }

        bool ckg_hashmap_has_helper(void* map) {
            HashMapContext context = ckg_hashmap_get_context(map);
            if (context.real_index == -1) {
                return false;
            }

            bool filled = *context.entry_filled_address;
            bool dead = *context.entry_dead_address;

            return filled && !dead;
        }

        void ckg_hashmap_get_helper(void* map) {
            HashMapContext context = ckg_hashmap_get_context(map);
            ckg_assert_msg(*(bool*)(context.entry_filled_address), "The key doesn't exist in the hashmap!\n");
            ckg_memory_copy((u8*)map + context.meta->temp_value_offset, context.meta->value_size, context.entry_value_address, context.meta->value_size);
        }

        void ckg_hashmap_put_helper(void* map) {
            if (ckg_hashmap_load_factor(map) >= CKG_HASHMAP_DEFAULT_LOAD_FACTOR) {
                ckg_hashmap_grow(map);
            }

            HashMapContext context = ckg_hashmap_get_context(map);
            bool filled = *context.entry_filled_address;
            bool dead = *context.entry_dead_address;
            if (!filled || dead) {
                context.meta->count++;
            }
            ckg_memory_copy(context.entry_key_address, context.meta->key_size, (u8*)map + context.meta->temp_key_offset, context.meta->key_size);
            ckg_memory_copy(context.entry_value_address, context.meta->value_size, (u8*)map + context.meta->temp_value_offset, context.meta->value_size);
            *context.entry_filled_address = true;
        }

        void ckg_hashmap_pop_helper(void* map) {
            HashMapContext context = ckg_hashmap_get_context(map);
            ckg_assert_msg(*context.entry_filled_address, "The key doesn't exist in the hashmap!\n");

            context.meta->count -= 1;
            context.meta->dead_count += 1;
            *context.entry_dead_address = true;
            ckg_memory_copy((u8*)map + context.meta->temp_value_offset, context.meta->value_size, context.entry_value_address, context.meta->value_size);
        }

        void ckg_hashmap_grow(void* map) {
            if (ckg_hashmap_load_factor(map) < CKG_HASHMAP_DEFAULT_LOAD_FACTOR) {
                return;
            }

            CKG_HashMapMeta* meta = (CKG_HashMapMeta*)map;
            u8* entries_base_address = NULLPTR;
            ckg_memory_copy(&entries_base_address, sizeof(void*), (u8*)map + meta->entries_offset, sizeof(void*));
            u64 old_capacity = meta->capacity;
            meta->capacity *= 2;
            void* new_entries = ckg_alloc(meta->capacity * meta->entry_size);
            ckg_memory_copy((u8*)map + meta->entries_offset, sizeof(void*), &new_entries, sizeof(void*));

            // rehash
            for (u64 i = 0; i < old_capacity; i++) {
                u8* entry = entries_base_address + (i * meta->entry_size);
                u8* entry_key = NULLPTR;
                if (meta->key_is_ptr) {
                    ckg_memory_copy(&entry_key, sizeof(void*), entry + meta->entry_key_offset,  sizeof(void*));
                } else {
                    entry_key = entry + meta->entry_key_offset;
                }

                bool entry_filled = *(bool*)(entry + meta->entry_filled_offset);
                bool entry_dead = *(bool*)(entry + meta->entry_dead_offset);
                if (!entry_filled || entry_dead) {
                    continue;
                }

                u64 hash = meta->hash_fn(entry_key, meta->key_size);
                u64 index = hash % meta->capacity;
                s64 real_index = ckit_hashmap_resolve_collision((u8*)map, entry_key, index);
                ckg_assert(real_index != -1);

                u8* new_entry = (u8*)new_entries + (real_index * meta->entry_size);
                ckg_memory_copy(new_entry, meta->entry_size, entry, meta->entry_size);
            }

            ckg_free(entries_base_address);
        }
    #endif
    //
    // ========== END CKG_HashMap ==========
    //
#endif

#if defined(CKG_IMPL_SERIALIZATION)
   void ckg_serialize_collection(void* collection, FILE* file_handle, CKG_CollectionType collection_type, CKG_DataType data_type) {
        ckg_assert(file_handle);

        switch (collection_type) {
            case CKG_COLLECTION_STACK:
            case CKG_COLLECTION_VECTOR: {
                CKG_VectorHeader* header = ckg_vector_header_base(collection);
                fwrite(header, sizeof(CKG_VectorHeader), 1, file_handle);
                if (data_type == CKG_DATA_TYPE_BITS) {
                    fwrite(collection, (header->element_size * header->count), 1, file_handle);
                } else if (data_type == CKG_DATA_TYPE_STRING_VIEW) {
                    CKG_StringView* sv_vector = (CKG_StringView*)collection;
                    for (int i = 0; i < header->count; i++) {
                        CKG_StringView sv = sv_vector[i];
                        fwrite(&sv.length, sizeof(size_t), 1, file_handle);
                        fwrite(sv.data, sv.length, 1, file_handle);
                    }
                } else if (data_type == CKG_DATA_TYPE_CSTRING) {
                    char** string_vector = (char**)collection;
                    for (int i = 0; i < header->count; i++) {
                        char* current_string = string_vector[i];
                        size_t count = ckg_cstr_length(current_string);
                        fwrite(&count, sizeof(size_t), 1, file_handle);
                        fwrite(current_string, count, 1, file_handle);
                    }
                }
            } break;

            case CKG_COLLECTION_RING_BUFFER: {
                CKG_RingBufferHeader* header = ckg_ring_buffer_header_base(collection);
                fwrite(header, sizeof(CKG_VectorHeader), 1, file_handle);
                if (data_type == CKG_DATA_TYPE_BITS) {
                    fwrite(collection, (header->element_size * header->count), 1, file_handle);
                } else if (data_type == CKG_DATA_TYPE_STRING_VIEW) {
                    CKG_StringView* string_vector = (CKG_StringView*)collection;
                    for (int i = 0; i < header->count; i++) {
                        CKG_StringView sv = string_vector[i];
                        fwrite(&sv.length, sizeof(size_t), 1, file_handle);
                        fwrite(sv.data, sv.length, 1, file_handle);
                    }
                } else if (data_type == CKG_DATA_TYPE_CSTRING) {
                    char** string_vector = (char**)collection;
                    for (int i = 0; i < header->count; i++) {
                        char* current_string = string_vector[i];
                        size_t count = ckg_cstr_length(current_string);
                        fwrite(&count, sizeof(size_t), 1, file_handle);
                        fwrite(current_string, count, 1, file_handle);
                    }
                }
            } break;

            case CKG_COLLECTION_LINKED_LIST: {

            } break;
        }
    }

    void* ckg_deserialize_collection(FILE* file_handle, CKG_CollectionType collection_type, CKG_DataType data_type) {
        ckg_assert(file_handle);

        switch (collection_type) {
            case CKG_COLLECTION_STACK:
            case CKG_COLLECTION_VECTOR: {
                CKG_VectorHeader header;
                fread(&header, sizeof(CKG_VectorHeader), 1, file_handle);

                if (data_type == CKG_DATA_TYPE_BITS) {
                    u8* vector = (u8*)ckg_vector_grow(NULLPTR, header.element_size, header.capacity);
                    fread(vector, header.element_size, header.count, file_handle);
                    return vector;
                } else if (data_type == CKG_DATA_TYPE_STRING_VIEW) {
                    CKG_StringView* sv_vector = (CKG_StringView*)ckg_vector_grow(NULLPTR, header.element_size, header.capacity);

                    for (int i = 0; i < header.count; i++) {
                        CKG_StringView sv;
                        fread(&sv.length, sizeof(size_t), 1, file_handle);

                        sv.data = (const char*)ckg_alloc(sv.length + 1);
                        fread((char*)sv.data, sv.length, 1, file_handle);
                       
                        ckg_vector_push(sv_vector, sv);
                    }

                    return sv_vector;
                } else if (data_type == CKG_DATA_TYPE_CSTRING) {
                    char** string_vector = (char**)ckg_vector_grow(NULLPTR, header.element_size, header.capacity);

                    for (int i = 0; i < header.count; i++) {
                        size_t char_count = 0;
                        fread(&char_count, sizeof(size_t), 1, file_handle);
                        char* current_string = (char*)ckg_alloc(char_count + 1); // prob want a way to specify if you should allocate or not
                        fread(current_string, char_count, 1, file_handle);

                        ckg_vector_push(string_vector, current_string);
                    }

                    return string_vector;
                }
            } break;

            case CKG_COLLECTION_RING_BUFFER: {
                CKG_RingBufferHeader header;
                fread(&header, sizeof(CKG_RingBufferHeader), 1, file_handle);

                if (data_type == CKG_DATA_TYPE_BITS) {
                    u8* ring_buffer = (u8*)ckg_ring_buffer_create(header.element_size, header.capacity);
                    fread(ring_buffer, header.element_size, header.count, file_handle);
                    return ring_buffer;
                } else if (data_type == CKG_DATA_TYPE_STRING_VIEW) {
                    CKG_StringView* sv_ring_buffer = (CKG_StringView*)ckg_ring_buffer_create(header.element_size, header.capacity);

                    for (int i = 0; i < header.count; i++) {
                        CKG_StringView sv;
                        fread(&sv.length, sizeof(size_t), 1, file_handle);
                        sv.data = (const char*)ckg_alloc(sv.length + 1);
                        fread((char*)sv.data, sv.length, 1, file_handle);

                        ckg_ring_buffer_enqueue(sv_ring_buffer, sv);
                    }

                    return sv_ring_buffer;
                } else if (data_type == CKG_DATA_TYPE_CSTRING) {
                    char** string_ring_buffer = (char**)ckg_ring_buffer_create(header.element_size, header.capacity);

                    for (int i = 0; i < header.count; i++) {
                        size_t char_count = 0;
                        fread(&char_count, sizeof(size_t), 1, file_handle);
                        char* current_string = (char*)(char_count + 1);
                        fread(current_string, char_count, 1, file_handle);

                        ckg_ring_buffer_enqueue(string_ring_buffer, current_string);
                    }

                    return string_ring_buffer;
                }
            } break;

            case CKG_COLLECTION_LINKED_LIST:{
                // return linked_list;
            } break;
        }

        return NULLPTR;
    }
#endif

#if defined(CKG_IMPL_IO)
    #if defined(PLATFORM_WINDOWS)
        bool ckg_io_path_exists(const char* path) {
            return (GetFileAttributesA(path) != INVALID_FILE_ATTRIBUTES);
        }

        u8* ckg_io_read_entire_file(const char* file_name, size_t* returned_file_size, CKG_Error* err) {
            HANDLE file_handle = CreateFileA(file_name, GENERIC_READ, 0, NULLPTR, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULLPTR);
            if (file_handle == INVALID_HANDLE_VALUE) {
                CKG_LOG_ERROR("CreateFileA() returned an INVALID_HANDLE_VALUE, the file_name/path is likely wrong: ckg_io_read_entire_file(%s)\n", file_name);
                ckg_error_safe_set(err, CKG_ERROR_IO_RESOURCE_NOT_FOUND);
                return NULLPTR;
            }

            LARGE_INTEGER large_int;
            ckg_memory_zero(&large_int, sizeof(LARGE_INTEGER));
            BOOL success = GetFileSizeEx(file_handle, &large_int);
            if (!success) {
                CKG_LOG_ERROR("GetFileSizeEx() Failed to get size from file_handle: ckg_io_read_entire_file(%s)\n", file_name);
                ckg_error_safe_set(err, CKG_ERROR_IO_RESOURCE_NOT_FOUND);
                CloseHandle(file_handle);
                return NULLPTR;
            }

            size_t file_size = (size_t)large_int.QuadPart;
            if (file_size > SIZE_MAX) {
                CKG_LOG_ERROR("File size is bigger than max size: ckg_io_read_entire_file(%s)\n", file_name);
                ckg_error_safe_set(err, CKG_ERROR_IO_RESOURCE_TOO_BIG);
                CloseHandle(file_handle);

                return NULLPTR;
            }

            u8* file_data = (u8*)ckg_alloc(file_size + 1); // +1 for null-terminator

            DWORD bytes_read = 0;
            success = ReadFile(file_handle, file_data, (DWORD)file_size, &bytes_read, NULLPTR);
            CloseHandle(file_handle);
            if (!success && bytes_read == file_size) {
                CKG_LOG_ERROR("ReadFile() Failed to get the file data or bytes read doesn't match file_size: ckg_io_read_entire_file(%s)\n", file_name);
                ckg_error_safe_set(err, CKG_ERROR_IO_RESOURCE_NOT_FOUND);
                ckg_free(file_data);
                return NULLPTR;
            }

            if (returned_file_size) {
                *returned_file_size = (size_t)file_size;
            }

            return file_data;
        }
    #else
        bool ckg_io_path_exists(const char* path) {
            FILE *fptr = fopen(path, "r");

            if (fptr == NULLPTR) {
                return false;
            }

            fclose(fptr);

            return true;
        }

        u8* ckg_io_read_entire_file(const char* file_name, size_t* returned_file_size, CKG_Error* err) {
            FILE* file_handle = fopen(file_name, "rb");
            if (file_handle == NULLPTR) {
                CKG_LOG_ERROR("Invalid file_handle, the file_name/path is likely wrong: ckg_io_read_entire_file(%s)\n", file_name);
                ckg_error_safe_set(err, CKG_ERROR_IO_RESOURCE_NOT_FOUND);

                return NULLPTR;
            }

            if (fseek(file_handle, 0L, SEEK_END) != 0) {
                CKG_LOG_ERROR("fseek failed: ckg_io_read_entire_file(%s)\n", file_name);
                ckg_error_safe_set(err, CKG_ERROR_IO_RESOURCE_NOT_FOUND);
                fclose(file_handle);
                return NULL;
            }

            long file_size = ftell(file_handle);
            if (file_size == -1L) {
                CKG_LOG_ERROR("ftell failed: ckg_io_read_entire_file(%s)\n", file_name);
                ckg_error_safe_set(err, CKG_ERROR_IO_RESOURCE_NOT_FOUND);
                fclose(file_handle);
                return NULL;
            }

            rewind(file_handle);
            if (ferror(file_handle)) {
                CKG_LOG_ERROR("rewind() failed: ckg_io_read_entire_file(%s)\n", file_name);
                ckg_error_safe_set(err, CKG_ERROR_IO_RESOURCE_NOT_FOUND);
                fclose(file_handle);
                return NULL;
            }

            u8* file_data = ckg_alloc((size_t)file_size + 1); // +1 for null terminator
            if (fread(file_data, file_size, 1, file_handle) != 1) {
                CKG_LOG_ERROR(false, "fread() failed: ckg_io_read_entire_file(%s)\n", file_name);
                ckg_error_safe_set(err, CKG_ERROR_IO_RESOURCE_NOT_FOUND);
                ckg_free(file_data);
                fclose(file_handle);

                return NULLPTR;
            }

            fclose(file_handle);

            if (returned_file_size) {
                *returned_file_size = file_size + 1;
            }

            return file_data;
        }
    #endif
#endif

#if defined(CKG_IMPL_OS)
    #if defined(PLATFORM_WINDOWS)
        CKG_DLL ckg_io_load_dll(const char* dll_name, CKG_Error* err) {
            HMODULE library = LoadLibraryA(dll_name);
            if (!library) {
                CKG_LOG_ERROR("LoadLibraryA() failed: ckg_io_load_dll(%s)\n", dll_name);
                ckg_error_safe_set(err, CKG_ERROR_IO_RESOURCE_NOT_FOUND);

                return NULLPTR;
            }

            return library;
        }

        void* ckg_os_get_proc_address(CKG_DLL dll, const char* proc_name, CKG_Error* err) {
            ckg_assert(dll);

            void* proc = (void*)GetProcAddress((HMODULE)dll, proc_name);
            if (!proc) {
                CKG_LOG_ERROR("GetProcAddress() failed: ckg_os_get_proc_address(%s)\n", proc_name);
                ckg_error_safe_set(err, CKG_ERROR_IO_RESOURCE_NOT_FOUND);
                return NULLPTR;
            }

            return proc;
        }

        CKG_DLL MACRO_ckg_os_free_dll(CKG_DLL dll) {
            ckg_assert(dll);
            FreeLibrary((HMODULE)dll);

            return NULLPTR;
        }
    #else
        CKG_DLL ckg_io_load_dll(const char* dll_name, CKG_Error* err) {
            void* library = dlopen(dll_name, RTLD_LAZY);
            if (!library) {
                CKG_LOG_ERROR("dlopen() failed: ckg_io_load_dll(%s)\n", dll_name);
                ckg_error_safe_set(err, CKG_ERROR_IO_RESOURCE_NOT_FOUND);
                return NULLPTR;
            }

            return library;
        }

        void* ckg_os_get_proc_address(CKG_DLL dll, const char* proc_name, CKG_Error* err) {
            void* proc = dlsym(dll, proc_name);
            if (!proc) {
                CKG_LOG_ERROR("dlsym() failed: ckg_os_get_proc_address(%s)\n", proc_name);
                ckg_error_safe_set(err, CKG_ERROR_IO_RESOURCE_NOT_FOUND);
                return NULLPTR;
            }

            return proc;
        }

        CKG_DLL MACRO_ckg_os_free_dll(CKG_DLL dll) {
            ckg_assert(dll);
            dlclose(dll);

            return NULLPTR;
        }
    #endif
#endif