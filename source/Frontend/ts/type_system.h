#include <ckg.h>

#include "../lexer/token.h"

enum {
    ION_TS_UNRESOLVED = 1u << 0,
    ION_TS_VOID       = 1u << 1, // "void"
    ION_TS_INT        = 1u << 2, // "int"
    ION_TS_FLOAT      = 1u << 3, // "float"
    ION_TS_BOOL       = 1u << 4, // "bool"
    ION_TS_STRING     = 1u << 5, // "string"
    ION_TS_ARRAY      = 1u << 6, // "[]"
    ION_TS_FUNC       = 1u << 7, // "func(...) -> "
    ION_TS_STRUCT     = 1u << 8,

    ION_TS_COMPARE_EQ  = 1u << 16,
    ION_TS_COMPARE_NEQ = 1u << 17,
    ION_TS_COMPARE_GT  = 1u << 18,
    ION_TS_COMPARE_GTE = 1u << 19,
    ION_TS_COMPARE_LTE = 1u << 20,
    ION_TS_COMPARE_LT  = 1u << 21,
    ION_TS_COMPARE_EQUALITY = ION_TS_COMPARE_EQ|ION_TS_COMPARE_NEQ,
    ION_TS_FULLY_COMPARABLE = ION_TS_COMPARE_EQUALITY|ION_TS_COMPARE_GT|ION_TS_COMPARE_GTE|ION_TS_COMPARE_LTE|ION_TS_COMPARE_LT,

    ION_TS_OP_ADD      = 1u << 24,
    ION_TS_OP_SUB      = 1u << 25,
    ION_TS_OP_MUL      = 1u << 26,
    ION_TS_OP_DIV      = 1u << 27,
    ION_TS_OP_MOD      = 1u << 28,
    ION_TS_OP_ALL = ION_TS_OP_ADD|ION_TS_OP_SUB|ION_TS_OP_MUL|ION_TS_OP_DIV|ION_TS_OP_MOD,

    ION_TS_INFO_INDEXABLE      = 1u << 29,
    ION_TS_INFO_CALLABLE      = 1u << 30,
};

typedef struct Parameter Parameter;

typedef u32 InfoMask;

typedef struct Type {
    InfoMask mask;
    u8 array_depth;  // 0 for non-array types
    CKG_Vector(Parameter) parameters; // if you are func type
} Type;

typedef struct Parameter  {
	IonToken token;
	Type type;
} Parameter;

Type ionTypeUnresolved() {
    Type ret = {0};
    ret.mask = ION_TS_UNRESOLVED;
    ret.array_depth = 0;

    return ret;
}

Type ionTypeVoid() {
    Type ret = {0};
    ret.mask = ION_TS_VOID;
    ret.array_depth = 0;

    return ret;
}

Type ionTypeInt() {
    Type ret = {0};
    ret.mask = (InfoMask)(ION_TS_INT|ION_TS_FULLY_COMPARABLE|ION_TS_OP_ALL);
    ret.array_depth = 0;

    return ret;
}

Type ionTypeFloat() {
    Type ret = {0};
    ret.mask = (InfoMask)(ION_TS_FLOAT|ION_TS_FULLY_COMPARABLE|ION_TS_OP_ALL);
    ret.array_depth = 0;
    
    return ret;
}

Type ionTypeBool() {
    Type ret = {0};
    ret.mask = (InfoMask)(ION_TS_BOOL|ION_TS_COMPARE_EQUALITY);
    ret.array_depth = 0;
    
    return ret;
}

Type ionTypeString() {
    Type ret = {0};
    ret.mask = (InfoMask)(ION_TS_STRING|ION_TS_COMPARE_EQUALITY|ION_TS_OP_ADD);
    ret.array_depth = 0;
    return ret;
}

Type ionTypeFunc(CKG_Vector(Parameter) parameters) {
    Type ret = {0};
    ret.mask = (InfoMask)(ION_TS_FUNC|ION_TS_INFO_CALLABLE);
    ret.array_depth = 0;
    ret.parameters = parameters;

    return ret;
}

Type ionTypeCreate(CKG_StringView sv) {
    if (ckg_str_equal(sv.data, sv.length, "void", sizeof("void") - 1)) {
        return ionTypeVoid();
    } else if (ckg_str_equal(sv.data, sv.length, "int", sizeof("int") - 1)) {
        return ionTypeInt();
    } else if (ckg_str_equal(sv.data, sv.length, "float", sizeof("float") - 1)) {
        return ionTypeFloat();
    } else if (ckg_str_equal(sv.data, sv.length, "bool", sizeof("bool") - 1)) {
        return ionTypeBool();
    } else if (ckg_str_equal(sv.data, sv.length, "string", sizeof("string") - 1)) {
        return ionTypeString();
    }

    // PROB gonna be a struct type later
    return ionTypeUnresolved(); // default fallback
}

// TypeCompare NOTE(Jovanni):
// Later on this might have like subtype and type group implications so it probably
// won't just be a bool it will be some type of int 0 is exact type 1 is super type, -1 is not equal
bool TypeCompare(Type c1, Type c2) {
	if (c1.mask != c2.mask) {
		return false;
    } else if (ckg_vector_count(c1.parameters) != ckg_vector_count(c2.parameters)) {
        return false;
    } else {
        for (int i = 0; i < ckg_vector_count(c1.parameters); i++) {
            Parameter param1 = c1.parameters[i];
            Parameter param2 = c2.parameters[i];
            if (!TypeCompare(param1.type, param2.type)) {
                return false;
            }
        }
    }

	return true;
}

Type ionTypePromote(IonToken op, Type a, Type b) {
    // if either is unresolved → unresolved
    if ((a.mask & ION_TS_UNRESOLVED) || (b.mask & ION_TS_UNRESOLVED)) {
        return ionTypeUnresolved();
    }

    // same type → return it
    if ((a.mask & ION_TS_INT) && (b.mask & ION_TS_INT))  {
        return ionTypeInt();
    } else if ((a.mask & ION_TS_FLOAT) && (b.mask & ION_TS_FLOAT)) {
        return ionTypeFloat();
    } else if ((a.mask & ION_TS_BOOL) && (b.mask & ION_TS_BOOL)) { 
        return ionTypeBool();
    } else if ((a.mask & ION_TS_STRING) && (b.mask & ION_TS_STRING)) {
        return ionTypeString();
    }

    if (op.kind == ION_TS_PLUS && ((a.mask & ION_TS_STRING) || (b.mask & ION_TS_STRING))) {
        return ionTypeString();
    }

    if ((a.mask & ION_TS_INT && b.mask & ION_TS_FLOAT) || (a.mask & ION_TS_FLOAT && b.mask & ION_TS_INT)) {
        return ionTypeFloat();
    }

    // anything else: unresolved
    return ionTypeUnresolved();
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