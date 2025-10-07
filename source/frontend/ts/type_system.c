#include "type_system.h"

bool ionTypeIsConcrete(IonType ty) { 
    return ty._bits != 0; 
}

bool ionTypeIsPlaceholder(IonType ty) {
    return (ty.compat_set == 0) && (ty.builtin_type_id == ION_BTYPE_PLACEHOLDER);
}

bool ionTypeIsBuiltin(IonType ty) { 
    return ty.compat_set != 0; 
}

bool ionTypeIsPoison(IonType ty) { 
    return ty.compat_set == ION_TYPE_COMPAT_ALL; 
}

bool ionTypeIsSignalingPoison(IonType ty) {
    return ionTypeIsPoison(ty) && (ty.builtin_type_id == ION_BTYPE_SIGNALING_POISON);
}

bool ionTypeWrapperEq(IonTypeWrapper ty1, IonTypeWrapper ty2) {
    if (ty1.kind != ty2.kind) {
        return false;
    }
    return ty1.kind != ION_TYPE_WRAPPER_ARRAY || (ty1.arr_item_count == ty2.arr_item_count);
}

bool ionTypeWrappersEq(IonType ty1, IonType ty2) {
    if (ty1.n_wrappers != ty2.n_wrappers) {
        return false;
    }
    
    for (u32 i = 0; i < ty1.n_wrappers; i++) {
        if (!ionTypeWrapperEq(ty1.wrappers[i], ty2.wrappers[i])) {
            return false;
        }
    }
    
    return true;
}

IonTypeCompatSet ionTypeToCompatSet(IonType ty) {
    ckg_assert_msg(ionTypeIsConcrete(ty), "type must be concrete!\n");

    if (!ionTypeIsBuiltin(ty)) { // userdef types
        return ION_TYPE_COMPAT_NONE;
    }

    switch (ty.builtin_type_id) {
        case ION_BTYPE_void:return ION_TYPE_COMPAT_VOID;
        case ION_BTYPE_bool:return ION_TYPE_COMPAT_BOOL;
        case ION_BTYPE_u8:  return ION_TYPE_COMPAT_UINT;
        case ION_BTYPE_u16: return ION_TYPE_COMPAT_UINT;
        case ION_BTYPE_u32: return ION_TYPE_COMPAT_UINT;
        case ION_BTYPE_u64: return ION_TYPE_COMPAT_UINT;
        case ION_BTYPE_i8:  return ION_TYPE_COMPAT_SINT;
        case ION_BTYPE_i16: return ION_TYPE_COMPAT_SINT;
        case ION_BTYPE_i32: return ION_TYPE_COMPAT_SINT;
        case ION_BTYPE_i64: return ION_TYPE_COMPAT_SINT;
        case ION_BTYPE_f32: return ION_TYPE_COMPAT_FLOAT;
        case ION_BTYPE_f64: return ION_TYPE_COMPAT_FLOAT;
        case ION_BTYPE_str: return ION_TYPE_COMPAT_STR;
        default: ckg_assert_msg(false, "invalid builtin type id!\n");
    }

    return ION_TYPE_COMPAT_ALL;
}

IonType ionTypePoison(void) {
    IonType ret = {0};
    ret.compat_set = ION_TYPE_COMPAT_ALL;
    return ret;
}

IonType ionTypeSignalingPoison(void) {
    IonType poison = ionTypePoison();
    poison.builtin_type_id = ION_BTYPE_SIGNALING_POISON;
    return poison;
}

IonType ionTypeVoid(void) {
    IonType ret = {0};
    ret.builtin_type_id = ION_BTYPE_void;
    ret.compat_set = ION_TYPE_COMPAT_VOID;
    return ret;
}

IonType ionTypeInt32(void) {
    IonType ret = {0};
    ret.builtin_type_id = ION_BTYPE_i32;
    ret.compat_set = ION_TYPE_COMPAT_SINT;
    return ret;
}

IonType ionTypeFloat32(void) {
    IonType ret = {0};
    ret.builtin_type_id = ION_BTYPE_f32;
    ret.compat_set = ION_TYPE_COMPAT_FLOAT;
    return ret;
}

IonType ionTypeBool(void) {
    IonType ret = {0};
    ret.builtin_type_id = ION_BTYPE_bool;
    ret.compat_set = ION_TYPE_COMPAT_BOOL;
    return ret;
}

IonType ionTypeStr(void) {
    IonType ret = {0};
    ret.builtin_type_id = ION_BTYPE_str;
    ret.compat_set = ION_TYPE_COMPAT_STR;
    return ret;
}

IonType ionTypeWrap(IonType ty, IonTypeWrapperKind8 wrapper_kind, u32 arr_item_count) {
    ckg_assert_msg(ty.n_wrappers < ArrayCount(ty.wrappers), "support up to %i type wrapper levels", ArrayCount(ty.wrappers));
    ckg_assert_msg((wrapper_kind == ION_TYPE_WRAPPER_ARRAY) == (arr_item_count > 0), "only array wrapper has associated `arr_item_count` payload");

    ty.wrappers[ty.n_wrappers++] = (IonTypeWrapper){.kind = wrapper_kind, .arr_item_count = arr_item_count};

    return ty;
}

IonType ionTypeCreate(CKG_StringView sv) {
    if (ckg_str_equal(sv.data, sv.length, "void", sizeof("void") - 1)) {
        return ionTypeVoid();
    } else if (ckg_str_equal(sv.data, sv.length, "int", sizeof("int") - 1)) {
        return ionTypeInt32();
    } else if (ckg_str_equal(sv.data, sv.length, "float", sizeof("float") - 1)) {
        return ionTypeFloat32();
    } else if (ckg_str_equal(sv.data, sv.length, "bool", sizeof("bool") - 1)) {
        return ionTypeBool();
    } else if (ckg_str_equal(sv.data, sv.length, "string", sizeof("string") - 1)) {
        return ionTypeStr();
    }
    return ionTypePoison();
}

IonType ionTypeIntersectInternal(IonType ty_act, IonType ty_exp) {
    // two types are compatible if their intersection is non-empty (non poison)
    // (conversely, they are incompatible if this returns poison)

    if (ionTypeIsPoison(ty_act) || ionTypeIsPoison(ty_exp)) {
        return ionTypePoison();
    }

    // if "type wrappers" mismatch, then return poison (types are not compatible)
    bool wrappers_match = ionTypeWrappersEq(ty_act, ty_exp);
    if (!wrappers_match) {
        return ionTypeSignalingPoison();
    }

    // if compat_set mismatch, then return poison (types are not compatible)
    IonTypeCompatSet intersection_compat_set = ty_act.compat_set & ty_exp.compat_set;
    if (intersection_compat_set == 0) {
        return ionTypeSignalingPoison();
    }

    IonType ty_res = ty_act; // copy wrappers
    ty_res.compat_set = intersection_compat_set; // set compat set to intersection
    ty_res._bits = 0; // reset concrete
    
    // compat_set vs compat_set: return intersection compat set
    bool ty_act_is_concrete = ionTypeIsConcrete(ty_act);
    bool ty_exp_is_concrete = ionTypeIsConcrete(ty_exp);
    
    if (!ty_act_is_concrete && !ty_exp_is_concrete) {
        return ty_res;
    }
    
    // concrete vs concrete: return concrete if they match else poison
    // @TOFO: upcast e.g. int8 (actual) where int32 is expected
    //        downcast is rejected since it may imply truncation (masking low bits)
    if (ty_act_is_concrete && ty_exp_is_concrete) {
        if (ty_act._bits != ty_exp._bits) {
            return ionTypeSignalingPoison();
        }
        
        ty_res._bits = ty_act._bits;
        return ty_res;
    }

    // concrete vs compat_set (*2)
    // return concrete if it belongs to the compat_set (else Poison)
    if (ty_act_is_concrete) {
        IonTypeCompatSet ty_act_compat_set = ionTypeToCompatSet(ty_act);
        if ((ty_exp.compat_set & ty_act_compat_set) == 0) {
            return ionTypeSignalingPoison();
        }
        return ty_exp;
    } else {
        IonTypeCompatSet ty_exp_compat_set = ionTypeToCompatSet(ty_exp);
        if ((ty_act.compat_set & ty_exp_compat_set) == 0) {
            return ionTypeSignalingPoison();
        }
        return ty_act;
    }
}

// pretty print a type
void ionTypePrint(IonType ty) {
    for (u32 w = ty.n_wrappers-1; w < ty.n_wrappers; --w) {
        switch (ty.wrappers[w].kind) {
            case ION_TYPE_WRAPPER_POINTER: CKG_LOG_PRINT("*"); break;
            case ION_TYPE_WRAPPER_SLICE: CKG_LOG_PRINT("[]"); break;
            case ION_TYPE_WRAPPER_ARRAY: CKG_LOG_PRINT("[%d]", ty.wrappers[w].arr_item_count); break;
        }
    }

    if (ionTypeIsPoison(ty)) {
        CKG_LOG_PRINT("Poison");
    } else if (ionTypeIsBuiltin(ty)) {
        switch (ty.builtin_type_id) {
            case ION_BTYPE_void: CKG_LOG_PRINT("void"); break;
            case ION_BTYPE_bool: CKG_LOG_PRINT("bool"); break;
            case ION_BTYPE_u8:   CKG_LOG_PRINT("u8"); break;
            case ION_BTYPE_u16:  CKG_LOG_PRINT("u16"); break;
            case ION_BTYPE_u32:  CKG_LOG_PRINT("u32"); break;
            case ION_BTYPE_u64:  CKG_LOG_PRINT("u64"); break;
            case ION_BTYPE_i8:   CKG_LOG_PRINT("i8"); break;
            case ION_BTYPE_i16:  CKG_LOG_PRINT("i16"); break;
            case ION_BTYPE_i32:  CKG_LOG_PRINT("i32"); break;
            case ION_BTYPE_i64:  CKG_LOG_PRINT("i64"); break;
            case ION_BTYPE_f32:  CKG_LOG_PRINT("f32"); break;
            case ION_BTYPE_f64:  CKG_LOG_PRINT("f64"); break;
            case ION_BTYPE_str:  CKG_LOG_PRINT("str"); break;
            default: ckg_assert_msg(false, "invalid builtin type id!\n");
        }
    } else if (ty.type_def != NULLPTR) {
        CKG_LOG_PRINT("userdef[struct|func] @TODO");
    } else {
        // None and All
        if (ty.compat_set == ION_TYPE_COMPAT_NONE) {
            CKG_LOG_PRINT("CompatSet_None");
        } else if (ty.compat_set == ION_TYPE_COMPAT_ALL) {
            CKG_LOG_PRINT("CompatSet_All");
        }
        
        // Compat Set
        const char* compat_set_strings[] = {
            [ION_TYPE_COMPAT_BIT_INDEX(VOID)] = "void",
            [ION_TYPE_COMPAT_BIT_INDEX(BOOL)] = "bool",
            [ION_TYPE_COMPAT_BIT_INDEX(UINT)] = "uint",
            [ION_TYPE_COMPAT_BIT_INDEX(SINT)] = "sint",
            [ION_TYPE_COMPAT_BIT_INDEX(FLOAT)] = "float",
            [ION_TYPE_COMPAT_BIT_INDEX(STR)] = "str",
        };

        for (u32 bit_index = 0; bit_index < 8; bit_index++) {
            if ((ty.compat_set >> bit_index) & 1) {
                CKG_LOG_PRINT("%s", compat_set_strings[bit_index]);
            }
        }
    }
}
