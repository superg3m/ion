#pragma once

#include <ckg.h>
#include "../lexer/token.h"

typedef u64 InfoMask;
enum {
    ION_TYPE_UNRESOLVED  = (1u << 0),
    ION_TYPE_VOID        = (1u << 1), // "void"
    ION_TYPE_INT         = (1u << 2), // "int"
    ION_TYPE_FLOAT       = (1u << 3), // "float"
    ION_TYPE_BOOL        = (1u << 4), // "bool"
    ION_TYPE_STRING      = (1u << 5), // "string"
    ION_TYPE_ARRAY       = (1u << 6), // "[]"
    ION_TYPE_FUNC        = (1u << 7), // "fn(...) -> "
    ION_TYPE_STRUCT      = (1u << 8),

    ION_TYPE_COMPARE_EQ  = (1u << 16),
    ION_TYPE_COMPARE_NEQ = (1u << 17),
    ION_TYPE_COMPARE_GT  = (1u << 18),
    ION_TYPE_COMPARE_GTE = (1u << 19),
    ION_TYPE_COMPARE_LTE = (1u << 20),
    ION_TYPE_COMPARE_LT  = (1u << 21),
    ION_TYPE_COMPARE_EQUALITY = ION_TYPE_COMPARE_EQ|ION_TYPE_COMPARE_NEQ,
    ION_TYPE_FULLY_COMPARABLE = ION_TYPE_COMPARE_EQUALITY|ION_TYPE_COMPARE_GT|ION_TYPE_COMPARE_GTE|ION_TYPE_COMPARE_LTE|ION_TYPE_COMPARE_LT,

    ION_TYPE_OP_ADD      = (1u << 24),
    ION_TYPE_OP_SUB      = (1u << 25),
    ION_TYPE_OP_MUL      = (1u << 26),
    ION_TYPE_OP_DIV      = (1u << 27),
    ION_TYPE_OP_MOD      = (1u << 28),
    ION_TYPE_OP_ALL = ION_TYPE_OP_ADD|ION_TYPE_OP_SUB|ION_TYPE_OP_MUL|ION_TYPE_OP_DIV|ION_TYPE_OP_MOD,

    ION_TYPE_INFO_INDEXABLE = (1u << 29),
};

// Im not sure if this can represent types like
// func() -> func(int) -> array -> array -> int

/*
fn test() -> (fn(int) -> [][]int) {
    return fn(x: int) -> [][]int {
        return [][].int[[1, 2, 3], [4, 5]]
    }
}
*/

typedef struct Type {
    InfoMask mask;
    u8 array_depth;  // 0 for non-array types

    // struct Type* next;
} Type;

Type ionGetReturnType(Type t);
Type ionTypeCreate(CKG_StringView sv);
Type ionTypePromote(IonToken op, Type a, Type b);
bool TypeCompare(Type c1, Type c2);

Type ionTypeAddArrayDepth(Type t);
Type ionTypeRemoveArrayDepth(Type t);