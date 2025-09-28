#include <ckg.h>

typedef u32 TypeMask;
enum {
    ION_TS_INT     = 1u << 0,
    ION_TS_FLOAT   = 1u << 1,
    ION_TS_BOOL    = 1u << 2,
    ION_TS_ARRAY   = 1u << 3,
    ION_TS_FUNC    = 1u << 4,
    ION_TS_STRUCT  = 1u << 5,

    ION_TS_ADD      = 1u << 7,
    ION_TS_SUB      = 1u << 8,
    ION_TS_MUL      = 1u << 9,
    ION_TS_DIV      = 1u << 10,
    ION_TS_MOD      = 1u << 11,

    ION_TS_COMPARABLE  = 1u << 12,
    ION_TS_INDEXABLE   = 1u << 13,
    ION_TS_CALLABLE    = 1u << 14,

    //BIT_SIGNED      = 1u << 20,
    //BIT_UNSIGNED    = 1u << 21,

    //BIT_CONST       = 1u << 24,
    //BIT_NULLABLE    = 1u << 25,
};

typedef struct Type {
    TypeMask mask;
    u8 array_depth;  // 0 for non-array types
    CKG_Vector(Type) parameters; // if you are func type
} Type;

Type ionTypeFunc(CKG_Vector(Type) parameters) {
    Type ret = {0};
    ret.mask = ION_TS_FUNC|ION_TS_CALLABLE;
    ret.array_depth = 0;
    ret.parameters = parameters;
}

Type ionTypeInt() {
    Type ret = {0};
    ret.mask = ION_TS_INT|ION_TS_ADD|ION_TS_SUB|ION_TS_MUL|ION_TS_DIV|ION_TS_MOD|ION_TS_COMPARABLE;
    ret.array_depth = 0;
    ret.array_depth = 0;
}

Type ionTypeAddArrayDepth(Type t) {
    t.mask |= ION_TS_ARRAY;
    t.array_depth += 1;

    return t;
}

Type ionTypeRemoveArrayDepth(Type t) {
    ckg_assert(t.mask & ION_TS_ARRAY);

    t.array_depth -= 1;
    if (t.array_depth == 0) {
        t.mask = t.mask & ~ION_TS_ARRAY;
    }

    return t;
}