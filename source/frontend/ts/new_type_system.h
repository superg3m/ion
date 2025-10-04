#pragma once

#include <ckg.h>

#define IonNode void  // @FIXME: hack due to running test headless (detached from ast.h)

typedef u32 IonBuiltinTypeID;
enum {
    ION_BTYPE_POISON = 0,
    ION_BTYPE_void,
    ION_BTYPE_bool,
    ION_BTYPE_u8,ION_BTYPE_u16,ION_BTYPE_u32,ION_BTYPE_u64,
    ION_BTYPE_i8,ION_BTYPE_i16,ION_BTYPE_i32,ION_BTYPE_i64,
    ION_BTYPE_f32,ION_BTYPE_f64,
    ION_BTYPE_str,

    ION_BTYPE_COUNT,
};

typedef u8 IonTypeCompatSet;
#define ION_TYPE_COMPAT_BIT_INDEX(compat_suffix) __builtin_ctz(ION_TYPE_COMPAT_ ## compat_suffix)
enum {
    ION_TYPE_COMPAT_VOID  = 0x01, // to be fair, for both `void` and `bool` the only possible members of the
    ION_TYPE_COMPAT_BOOL  = 0x02, // compatibility set is `void` and `bool` respectively, so these are redundant
    ION_TYPE_COMPAT_UINT  = 0x04,
    ION_TYPE_COMPAT_SINT  = 0x08,
    ION_TYPE_COMPAT_FLOAT = 0x10,
    ION_TYPE_COMPAT_STR   = 0x20,
};
#define ION_TYPE_COMPAT_NONE 0x0  // used by UserDef Types (compat only with same concrete, not compatible as a set!)
#define ION_TYPE_COMPAT_ALL 0xFF  // used by Poison Type


typedef u8 IonTypeWrapperKind8;
enum {
    ION_TYPE_WRAPPER_NONE = 0,
    ION_TYPE_WRAPPER_POINTER,
    ION_TYPE_WRAPPER_SLICE,
    ION_TYPE_WRAPPER_ARRAY,
};

typedef struct IonTypeWrapper {
    IonTypeWrapperKind8 kind;
    union {
        u32 arr_item_count; // array wrapper has associated item count
    };
} IonTypeWrapper;

// IonType will be used by type-checker during type-inference
// meaning not all IonType will be resolved / known: they may exist as "compatibility sets" til inference finishes.
typedef struct IonType {
    // for non concrete types (e.g. during inference), we always have at least the compat set
    // (we fallback to poison in some cases for error recovery)
    IonTypeCompatSet compat_set;

    union {
        // builtin types are represented with a special ID
        IonBuiltinTypeID builtin_type_id;
        // user-def types: ptr to definition site (which has schema / proc prototype)
        IonNode* type_def;
        IonNode* func_def;
    
        size_t _bits;
    };
    
    // wrappers (pointer|slice|array) "applying to" the base type (or the compat set)
    u32            n_wrappers;  // non-zero means at least one wrapper present
    IonTypeWrapper wrappers[4]; // support up to 4 wrapper levels
} IonType;


// Function declarations
bool ionTypeIsConcrete(IonType ty);
bool ionTypeIsBuiltin(IonType ty);
bool ionTypeIsPoison(IonType ty);
bool ionTypeWrapperEq(IonTypeWrapper ty1, IonTypeWrapper ty2);
bool ionTypeWrappersEq(IonType ty1, IonType ty2);

IonTypeCompatSet ionTypeToCompatSet(IonType ty);

IonType ionTypePoison(void);
IonType ionTypeVoid(void);
IonType ionTypeInt32(void);
IonType ionTypeFloat32(void);
IonType ionTypeBool(void);
IonType ionTypeStr(void);
IonType ionTypeCreate(CKG_StringView sv);

#define ionTypeWrapPointer(ty) ionTypeWrap(ty, ION_TYPE_WRAPPER_POINTER, 0)
#define ionTypeWrapSlice(ty) ionTypeWrap(ty, ION_TYPE_WRAPPER_SLICE, 0)
#define ionTypeWrapArray(ty, arr_item_count) ionTypeWrap(ty, ION_TYPE_WRAPPER_ARRAY, arr_item_count)
IonType ionTypeWrap(IonType ty, IonTypeWrapperKind8 wrapper_kind, u32 arr_item_count);

IonType ionTypeIntersect(IonType ty1, IonType ty2);

void ionTypePrint(IonType ty);
