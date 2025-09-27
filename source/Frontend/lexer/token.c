#include <Frontend/token.h>

void token_print(IonToken token, char* indent) {
    CKG_LOG_PRINT("%s%s(%.*s) | line: %d\n", indent, token_strings[token.type], (int)token.name.length, token.name.data, token.line);
}

IonToken ion_token_from_string(IonTokenType token_type, CKG_StringView sv, int line) {
    IonToken token = { .type = token_type, .name = sv, .line = line };
    
    if (token.type == ION_TOKEN_STRING_LITERAL) {
        return token;
    } else if (token.type == ION_TOKEN_CHARACTER_LITERAL) {
        token.c = token.name.data[1];

        return token;
    }

    if (token.type == ION_TOKEN_TRUE) {
        token.b = true;

        return token;
    } else if (token.type == ION_TOKEN_FALSE) {
        token.b = false;

        return token;
    }


    char* buffer = ckg_alloc(sv.length + 1);
    ckg_memory_copy(buffer, sv.length, sv.data, sv.length);
    buffer[sv.length] = '\0';
    
    char* endptr_int = NULL;
    long i = strtol(buffer, &endptr_int, 10);
    if ((size_t)(endptr_int - buffer) == sv.length) {
        token.type = ION_TOKEN_INTEGER_LITERAL;
        token.i = (int)i;
        ckg_free(buffer);

        return token;
    }
    
    char* endptr = NULL;
    float f = strtof(buffer, &endptr);
    if ((size_t)(endptr - buffer) == sv.length) {
        token.type = ION_TOKEN_FLOAT_LITERAL;
        token.f = f;
        ckg_free(buffer);

        return token;
    }
    
    ckg_free(buffer);
    return token;
}

// Date: May 13, 2025
// NOTE(Jovanni): REALISTICALLY THIS SHOULD BE A HASHTABLE...
IonTokenType token_get_keyword(const char* str, u64 str_length) {
    if (ckg_str_equal(str, str_length, CKG_LIT_ARG("if"))) {
        return ION_TOKEN_IF;
    } else if (ckg_str_equal(str, str_length, CKG_LIT_ARG("else"))) {
        return ION_TOKEN_ELSE;
    } else if (ckg_str_equal(str, str_length, CKG_LIT_ARG("for"))) {
        return ION_TOKEN_FOR;
    } else if (ckg_str_equal(str, str_length, CKG_LIT_ARG("while"))) {
        return ION_TOKEN_WHILE;
    } else if (ckg_str_equal(str, str_length, CKG_LIT_ARG("true"))) {
        return ION_TOKEN_TRUE;
    } else if (ckg_str_equal(str, str_length, CKG_LIT_ARG("false"))) {
        return ION_TOKEN_FALSE;
    } else if (ckg_str_equal(str, str_length, CKG_LIT_ARG("null"))) {
        return ION_TOKEN_NULL;
    } else if (ckg_str_equal(str, str_length, CKG_LIT_ARG("return"))) {
        return ION_TOKEN_RETURN;
    } else if (ckg_str_equal(str, str_length, CKG_LIT_ARG("var"))) {
        return ION_TOKEN_VAR;
    } else if (ckg_str_equal(str, str_length, CKG_LIT_ARG("func"))) {
        return ION_TOKEN_FUNC;
    }

    return ION_TOKEN_ILLEGAL_TOKEN;
}

IonTokenType token_get_type_primitive(const char* str, u64 str_length) {
   bool signed_primitives = 
   ckg_str_equal(str, str_length, CKG_LIT_ARG("i8")) || ckg_str_equal(str, str_length, CKG_LIT_ARG("i16")) ||
   ckg_str_equal(str, str_length, CKG_LIT_ARG("i32")) || ckg_str_equal(str, str_length, CKG_LIT_ARG("i64"));

   bool unsigned_primitives = 
   ckg_str_equal(str, str_length, CKG_LIT_ARG("u8")) || ckg_str_equal(str, str_length, CKG_LIT_ARG("u16")) ||
   ckg_str_equal(str, str_length, CKG_LIT_ARG("u32")) || ckg_str_equal(str, str_length, CKG_LIT_ARG("u4"));

    if (signed_primitives || unsigned_primitives) {
        return ION_TOKEN_PRIMITIVE_TYPE;
    }

    return ION_TOKEN_ILLEGAL_TOKEN;
}

// Date: May 13, 2025
// NOTE(Jovanni): REALISTICALLY THIS SHOULD BE A HASHTABLE...
IonTokenType token_get_syntax(const char* str, u64 str_length) {
    if (ckg_str_equal(str, str_length, CKG_LIT_ARG("="))) {
        return ION_TOKEN_ASSIGNMENT;
    } else if (ckg_str_equal(str, str_length, CKG_LIT_ARG("%"))) {
        return ION_TOKEN_MODULUS;
    } else if (ckg_str_equal(str, str_length, CKG_LIT_ARG(":"))) {
        return ION_TOKEN_COLON;
    } else if (ckg_str_equal(str, str_length, CKG_LIT_ARG("~"))) {
        return ION_TOKEN_BITWISE_NOT;
    } else if (ckg_str_equal(str, str_length, CKG_LIT_ARG("+"))) {
        return ION_TOKEN_PLUS;
    } else if (ckg_str_equal(str, str_length, CKG_LIT_ARG("-"))) {
        return ION_TOKEN_MINUS;
    } else if (ckg_str_equal(str, str_length, CKG_LIT_ARG("->"))) {
        return ION_TOKEN_RIGHT_ARROW;
    } else if (ckg_str_equal(str, str_length, CKG_LIT_ARG("/"))) {
        return ION_TOKEN_DIVISION;
    } else if (ckg_str_equal(str, str_length, CKG_LIT_ARG("*"))) {
        return ION_TOKEN_STAR;
    } else if (ckg_str_equal(str, str_length, CKG_LIT_ARG("&"))) {
        return ION_TOKEN_AMPERSAND;
    } else if (ckg_str_equal(str, str_length, CKG_LIT_ARG("|"))) {
        return ION_TOKEN_BITWISE_OR;
    } else if (ckg_str_equal(str, str_length, CKG_LIT_ARG("^"))) {
        return ION_TOKEN_BITWISE_XOR;
    } else if (ckg_str_equal(str, str_length, CKG_LIT_ARG("<"))) {
        return ION_TOKEN_LESS_THAN;
    } else if (ckg_str_equal(str, str_length, CKG_LIT_ARG(">"))) {
        return ION_TOKEN_GREATER_THAN;
    } else if (ckg_str_equal(str, str_length, CKG_LIT_ARG("!"))) {
        return ION_TOKEN_NOT;
    } else if (ckg_str_equal(str, str_length, CKG_LIT_ARG("("))) {
        return ION_TOKEN_LEFT_PAREN;
    } else if (ckg_str_equal(str, str_length, CKG_LIT_ARG(")"))) {
        return ION_TOKEN_RIGHT_PAREN;
    } else if (ckg_str_equal(str, str_length, CKG_LIT_ARG(","))) {
        return ION_TOKEN_COMMA;
    } else if (ckg_str_equal(str, str_length, CKG_LIT_ARG("."))) {
        return ION_TOKEN_DOT;
    } else if (ckg_str_equal(str, str_length, CKG_LIT_ARG(";"))) {
        return ION_TOKEN_SEMI_COLON;
    } else if (ckg_str_equal(str, str_length, CKG_LIT_ARG("["))) {
        return ION_TOKEN_LEFT_BRACKET;
    } else if (ckg_str_equal(str, str_length, CKG_LIT_ARG("]"))) {
        return ION_TOKEN_RIGHT_BRACKET;
    } else if (ckg_str_equal(str, str_length, CKG_LIT_ARG("{"))) {
        return ION_TOKEN_LEFT_CURLY;
    } else if (ckg_str_equal(str, str_length, CKG_LIT_ARG("}"))) {
        return ION_TOKEN_RIGHT_CURLY;
    } else if (ckg_str_equal(str, str_length, CKG_LIT_ARG("++"))) {
        return ION_TOKEN_INCREMENT;
    } else if (ckg_str_equal(str, str_length, CKG_LIT_ARG("--"))) {
        return ION_TOKEN_DECREMENT;
    } else if (ckg_str_equal(str, str_length, CKG_LIT_ARG("=="))) {
        return ION_TOKEN_EQUALS;
    } else if (ckg_str_equal(str, str_length, CKG_LIT_ARG("!="))) {
        return ION_TOKEN_NOT_EQUALS;
    } else if (ckg_str_equal(str, str_length, CKG_LIT_ARG("+="))) {
        return ION_TOKEN_PLUS_EQUALS;
    } else if (ckg_str_equal(str, str_length, CKG_LIT_ARG("-="))) {
        return ION_TOKEN_MINUS_EQUALS;
    } else if (ckg_str_equal(str, str_length, CKG_LIT_ARG("*="))) {
        return ION_TOKEN_MULTIPLICATION_EQUALS;
    } else if (ckg_str_equal(str, str_length, CKG_LIT_ARG("/="))) {
        return ION_TOKEN_DIVISION_EQUALS;
    } else if (ckg_str_equal(str, str_length, CKG_LIT_ARG(">="))) {
        return ION_TOKEN_GREATER_THAN_EQUALS;
    } else if (ckg_str_equal(str, str_length, CKG_LIT_ARG("<="))) {
        return ION_TOKEN_LESS_THAN_EQUALS;
    } else if (ckg_str_equal(str, str_length, CKG_LIT_ARG("<<"))) {
        return ION_TOKEN_BITWISE_LEFT_SHIFT;
    } else if (ckg_str_equal(str, str_length, CKG_LIT_ARG(">>"))) {
        return ION_TOKEN_BITWISE_RIGHT_SHIFT;
    } else if (ckg_str_equal(str, str_length, CKG_LIT_ARG("<<="))) {
        return ION_TOKEN_BITWISE_LEFT_SHIFT_EQUALS;
    } else if (ckg_str_equal(str, str_length, CKG_LIT_ARG(">>="))) {
        return ION_TOKEN_BITWISE_RIGHT_SHIFT_EQUALS;
    } else if (ckg_str_equal(str, str_length, CKG_LIT_ARG(":="))) {
        return ION_TOKEN_COLON_EQUALS;
    } else if (ckg_str_equal(str, str_length, CKG_LIT_ARG("---"))) {
        return ION_TOKEN_UNINITIALIZED;
    } else if (ckg_str_equal(str, str_length, CKG_LIT_ARG("[..]"))) {
        return ION_TOKEN_DYNAMIC_ARRAY;
    } else if (ckg_str_equal(str, str_length, CKG_LIT_ARG("&&"))) {
        return ION_TOKEN_AND;
    } else if (ckg_str_equal(str, str_length, CKG_LIT_ARG("||"))) {
        return ION_TOKEN_OR;
    } else if (ckg_str_equal(str, str_length, CKG_LIT_ARG("//"))) {
        return ION_TOKEN_COMMENT;
    }

    return ION_TOKEN_ILLEGAL_TOKEN;
}