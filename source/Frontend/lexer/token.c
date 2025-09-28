#include "token.h"
#include <ckg.h>

IonToken ionTokenCreate(IonTokenKind kind, CKG_StringView lexeme, int line) {
    IonToken ret;
    ret.kind = kind;
    ret.lexeme = lexeme;
    ret.line = line;

    return ret;
}

const char* ionTokenKindGetString(IonTokenKind kind) {
    ckg_assert(kind >= 0 && kind < ION_TOKEN_COUNT);

    static char* token_strings[ION_TOKEN_COUNT] = {
        stringify(ION_TOKEN_ILLEGAL_TOKEN),
        stringify(ION_TOKEN_EOF),
        
        #define X(name, str) stringify(name),
            X_SYNTAX_TOKENS
        #undef X   

        #define X(name) stringify(name),
            X_LITERAL_TOKENS
        #undef X

        #define X(name, str) stringify(name),
            X_KEYWORD_TOKENS
        #undef X

        stringify(ION_TOKEN_IDENTIFIER),
    };

    return token_strings[kind];
}

void ionTokenPrint(IonToken token) {
    CKG_LOG_PRINT("%s(%.*s) | line: %d\n", ionTokenKindGetString(token.kind), (int)token.lexeme.length, token.lexeme.data, token.line);
}

IonTokenKind ionTokenGetKeyword(CKG_StringView sv) {
    CKG_HashMap(CKG_StringView, IonTokenKind)* keyword_map = NULL;
    ckg_hashmap_init_string_view_hash(keyword_map, CKG_StringView, IonTokenKind);
    #define X(token, str) ckg_hashmap_put(keyword_map, ckg_sv_create(str, sizeof(str) - 1), token);
        X_KEYWORD_TOKENS
    #undef X

    ckg_hashmap_put(keyword_map, ckg_sv_create("true", sizeof("true") - 1), ION_TL_BOOLEAN);
    ckg_hashmap_put(keyword_map, ckg_sv_create("false", sizeof("false") - 1), ION_TL_BOOLEAN);

    if (!ckg_hashmap_has(keyword_map, sv)) {
        return ION_TOKEN_ILLEGAL_TOKEN;
    }

    return ckg_hashmap_get(keyword_map, sv);
}

IonTokenKind ionTokenGetSyntax(CKG_StringView sv) {
    CKG_HashMap(CKG_StringView, IonTokenKind)* keyword_map = NULL;
    ckg_hashmap_init_string_view_hash(keyword_map, CKG_StringView, IonTokenKind);
    #define X(token, str) ckg_hashmap_put(keyword_map, ckg_sv_create(str, sizeof(str) - 1), token);
        X_SYNTAX_TOKENS
    #undef X

    if (!ckg_hashmap_has(keyword_map, sv)) {
        CKG_LOG_ERROR("WHAT: %.*s\n", sv.length, sv.data);
        return ION_TOKEN_ILLEGAL_TOKEN;
    }

    return ckg_hashmap_get(keyword_map, sv);
}