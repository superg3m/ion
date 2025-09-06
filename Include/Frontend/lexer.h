#pragma once
#include <Core/ckg.h>
#include <Frontend/token.h>

typedef struct Lexer {
    u64 left_pos;
    u64 right_pos;
    CKG_Vector(IonToken) tokens;
    int line;

    CKG_StringView source;
    char c;
} Lexer;

Lexer lexer_create();
CKG_Vector(IonToken) lexer_consume_token_stream(Lexer* lexer, u8* source, u64 source_length);