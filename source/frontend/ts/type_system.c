#include "type_system.h"
/*
Type ionTypeUnresolved() {
    Type ret = {0};
    ret.mask = ION_TYPE_UNRESOLVED;
    ret.array_depth = 0;

    return ret;
}

Type ionTypeVoid() {
    Type ret = {0};
    ret.mask = ION_TYPE_VOID;
    ret.array_depth = 0;

    return ret;
}

Type ionTypeInt() {
    Type ret = {0};
    ret.mask = (InfoMask)(ION_TYPE_INT|ION_TYPE_FULLY_COMPARABLE|ION_TYPE_OP_ALL);
    ret.array_depth = 0;

    return ret;
}

Type ionTypeFloat() {
    Type ret = {0};
    ret.mask = (InfoMask)(ION_TYPE_FLOAT|ION_TYPE_FULLY_COMPARABLE|ION_TYPE_OP_ALL);
    ret.array_depth = 0;
    
    return ret;
}

Type ionTypeBool() {
    Type ret = {0};
    ret.mask = (InfoMask)(ION_TYPE_BOOL|ION_TYPE_COMPARE_EQUALITY);
    ret.array_depth = 0;
    
    return ret;
}

Type ionTypeString() {
    Type ret = {0};
    ret.mask = (InfoMask)(ION_TYPE_STRING|ION_TYPE_COMPARE_EQUALITY|ION_TYPE_OP_ADD);
    ret.array_depth = 0;
    return ret;
}

Type ionTypeFunc(CKG_Vector(Parameter) parameters, Type return_type) {
    Type ret = {0};
    ret.mask = return_type.mask | (InfoMask)(ION_TYPE_FUNC);
    ret.array_depth = 0;
    ret.parameters = parameters;

    return ret;
}

Type ionGetReturnType(Type t) {
    ckg_assert(t.mask & ION_TYPE_FUNC);

    t.mask &= ~ION_TYPE_FUNC;
    t.parameters = NULLPTR;

    return t;
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
    if ((a.mask & ION_TYPE_UNRESOLVED) || (b.mask & ION_TYPE_UNRESOLVED)) {
        return ionTypeUnresolved();
    }

    // same type → return it
    if ((a.mask & ION_TYPE_INT) && (b.mask & ION_TYPE_INT))  {
        return ionTypeInt();
    } else if ((a.mask & ION_TYPE_FLOAT) && (b.mask & ION_TYPE_FLOAT)) {
        return ionTypeFloat();
    } else if ((a.mask & ION_TYPE_BOOL) && (b.mask & ION_TYPE_BOOL)) { 
        return ionTypeBool();
    } else if ((a.mask & ION_TYPE_STRING) && (b.mask & ION_TYPE_STRING)) {
        return ionTypeString();
    }

    if (op.kind == ION_TS_PLUS && ((a.mask & ION_TYPE_STRING) || (b.mask & ION_TYPE_STRING))) {
        return ionTypeString();
    }

    if ((a.mask & ION_TYPE_INT && b.mask & ION_TYPE_FLOAT) || (a.mask & ION_TYPE_FLOAT && b.mask & ION_TYPE_INT)) {
        return ionTypeFloat();
    }

    // anything else: unresolved
    return ionTypeUnresolved();
}

Type ionTypeAddArrayDepth(Type t) {
    t.mask |= ION_TYPE_ARRAY;
    t.array_depth += 1;

    return t;
}

Type ionTypeRemoveArrayDepth(Type t) {
    ckg_assert(t.mask & ION_TYPE_ARRAY);

    t.array_depth -= 1;
    if (t.array_depth == 0) {
        t.mask = t.mask & ~ION_TYPE_ARRAY;
    }

    return t;
}
*/