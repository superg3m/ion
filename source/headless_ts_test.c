#define CKG_IMPL
#define CKG_OS_INCLUDE
#include <ckg.h>

#include "frontend/ts/type_system.h"

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

    IonType void1 = ionTypeVoid();
    IonType void2 = ionTypeVoid();
    ionTypePrint(ionTypeIntersect(void1, void2)); CKG_LOG_PRINT("\n");

    IonType voidptr1 = ionTypeWrapPointer(ionTypeVoid());
    IonType voidptr2 = ionTypeWrapPointer(ionTypeVoid());
    ionTypePrint(ionTypeIntersect(voidptr1, voidptr2)); CKG_LOG_PRINT("\n");
    
    IonType slice_voidptr1 = ionTypeWrapSlice(ionTypeWrapPointer(ionTypeVoid()));
    IonType slice_voidptr2 = ionTypeWrapSlice(ionTypeWrapPointer(ionTypeVoid()));
    ionTypePrint(ionTypeIntersect(slice_voidptr1, slice_voidptr2)); CKG_LOG_PRINT("\n");

    IonType arr_voidptr1 = ionTypeWrapArray(ionTypeWrapPointer(ionTypeVoid()), 1);
    IonType arr_voidptr2 = ionTypeWrapArray(ionTypeWrapPointer(ionTypeVoid()), 1);
    ionTypePrint(ionTypeIntersect(arr_voidptr1, arr_voidptr2)); CKG_LOG_PRINT("\n");

    CKG_LOG_SUCCESS("\n\nused/cap: %zu / %zu = %f\n", arena.used, arena.capacity, (float)arena.used / (float)arena.capacity);
    ckg_arena_free(&arena);

    return 0;
}