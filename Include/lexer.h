#pragma once
#include <ckg.h>
#include <token.h>

typedef struct Lexer {
    u64 left_pos;
    u64 right_pos;
    SPL_Token* tokens;
    int line;

    CKG_StringView source;
    char c;
} Lexer;

Lexer lexer_create();
SPL_Token* lexer_consume_token_stream(Lexer* lexer, char* source, u64 source_length);