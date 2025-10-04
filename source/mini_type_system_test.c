#define CKG_IMPL
#define CKG_OS_INCLUDE
#include <ckg.h>

#include "frontend/ts/new_type_system.h"

#define TOTAL_STACK_MEMORY_SIZE KiloBytes(800)
#define TOTAL_HEAP_MEMORY_SIZE MegaBytes(35)
void* custom_alloc_callback(CKG_Allocator* allocator, size_t allocation_size) {
    CKG_Arena* arena = (CKG_Arena*)allocator->ctx;
    return ckg_arena_push_custom(arena, allocation_size);
}

void custom_free_callback(CKG_Allocator* allocator, void* data) {
    CKG_Arena* arena = (CKG_Arena*)allocator->ctx;
    ckg_arena_pop_data(arena, data);
}

int main() {
    void* program_heap_memory = ckg_alloc(TOTAL_HEAP_MEMORY_SIZE);
    CKG_Arena arena = ckg_arena_create_fixed(program_heap_memory, TOTAL_HEAP_MEMORY_SIZE, false);
    ckg_bind_custom_allocator(custom_alloc_callback, custom_free_callback, &arena);

    IonType ty_int = ionTypeInt32();
    ty_int = ionTypeWrap(ty_int, ION_TYPE_WRAPPER_ARRAY, 1);
    ty_int = ionTypeWrap(ty_int, ION_TYPE_WRAPPER_ARRAY, 2);
    IonType ty_int2 = ionTypeInt32();
    ty_int2 = ionTypeWrap(ty_int2, ION_TYPE_WRAPPER_ARRAY, 1);
    ty_int2 = ionTypeWrap(ty_int2, ION_TYPE_WRAPPER_ARRAY, 2);
    IonType inter_int_intptr = ionTypeIntersect(ty_int, ty_int2);
    ionTypePrint(inter_int_intptr);


    CKG_LOG_SUCCESS("\n\nused/cap: %zu / %zu = %f\n", arena.used, arena.capacity, (float)arena.used / (float)arena.capacity);
    ckg_arena_free(&arena);

    return 0;
}