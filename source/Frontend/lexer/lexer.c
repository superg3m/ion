#include "lexer.h"

typedef struct Lexer {
    u64 left_pos;
    u64 right_pos;
    CKG_Vector(IonToken) tokens;
    int line;

    CKG_StringView source;
    char c;
} Lexer;

static bool ionIsWhitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\0';
}

static bool ionLexerIsEOF(Lexer* lexer) {
    return lexer->right_pos >= lexer->source.length;
}

static void ionLexerConsumeNextChar(Lexer* lexer) {
    lexer->c = lexer->source.data[lexer->right_pos];

    if (lexer->c == '\n') {
        lexer->line += 1;
    }

    lexer->right_pos += 1;
}

static char ionLexerPeekNthChar(Lexer* lexer, u64 n) {
    if ((lexer->right_pos + n) >= lexer->source.length) {
        return '\0';
    }

    return lexer->source.data[lexer->right_pos + n];
}

static CKG_StringView ionLexerGetScratchBuffer(Lexer* lexer) {
    return ckg_sv_create(lexer->source.data + lexer->left_pos, lexer->right_pos - lexer->left_pos);
}

static void ionLexerReportError(Lexer* lexer, char* msg) {
    CKG_StringView scratch = ionLexerGetScratchBuffer(lexer);
    CKG_LOG_ERROR("String: %.*s\n", (int)scratch.length, scratch.data); 
    CKG_LOG_ERROR("Bytes: ");
    for (u64 i = 0; i < scratch.length; i++) {
        if (i == scratch.length - 1) {
            CKG_LOG_PRINT("%d\n", scratch.data[i]);
        } else {
            CKG_LOG_PRINT("%d, ", scratch.data[i]);
        }

    }

    CKG_LOG_ERROR("[LEXER ERROR] line: %d | %s", lexer->line, msg);
    exit(-1);
}

static bool ionLexerConsumeOnMatch(Lexer* lexer, char expected) {
    if (ionLexerPeekNthChar(lexer, 0) != expected) {
        return false;
    }

    ionLexerConsumeNextChar(lexer);
    return true;
}


static void ionLexerAddToken(Lexer* lexer, IonTokenKind kind) {
    CKG_StringView lexeme = ionLexerGetScratchBuffer(lexer);
    ckg_vector_push(lexer->tokens, ionTokenCreate(kind, lexeme, lexer->line));
}

static void ionLexerConsumeNumberLiteral(Lexer* lexer) {
    IonTokenKind kind = ION_TL_INTEGER;

    ionLexerConsumeOnMatch(lexer, '-');
    while (ckg_char_is_digit(ionLexerPeekNthChar(lexer, 0)) || ionLexerPeekNthChar(lexer, 0) == '.') {
        if (lexer->c == '.') {
            kind = ION_TL_FLOAT;
        }

        ionLexerConsumeNextChar(lexer);
    }

    ionLexerAddToken(lexer, kind);
}

static void ionLexerConsumeStringLiteral(Lexer* lexer) {
    while (ionLexerPeekNthChar(lexer, 0) != '\"') {
        if (ionLexerIsEOF(lexer)) {
            ionLexerReportError(lexer, "String literal doesn't have a closing double quote!\n");
        }
        
        ionLexerConsumeNextChar(lexer);
    }

    ionLexerConsumeNextChar(lexer);
    ionLexerAddToken(lexer, ION_TL_STRING);
}

static void ionLexerConsumeCharacterLiteral(Lexer* lexer) {
    if (ionLexerConsumeOnMatch(lexer, '\'')) {
        ionLexerReportError(lexer, "character literal doesn't have any ascii data in between\n");
    }

    while (ionLexerPeekNthChar(lexer, 0) != '\'') {
        if (ionLexerIsEOF(lexer)) {
            ionLexerReportError(lexer, "String literal doesn't have a closing double quote!\n");
        }
        
        ionLexerConsumeNextChar(lexer);
    }

    ionLexerConsumeNextChar(lexer);
    ionLexerAddToken(lexer, ION_TL_CHARACTER);
}

static bool ionLexerConsumeLiteral(Lexer* lexer) {
    if (ckg_char_is_digit(lexer->c) || (lexer->c == '-' && ckg_char_is_digit(ionLexerPeekNthChar(lexer, 0)))) {
        ionLexerConsumeNumberLiteral(lexer);
        return true;
    } else if (lexer->c == '\"') {
        ionLexerConsumeStringLiteral(lexer);
        return true;
    } else if (lexer->c == '\'') {
        ionLexerConsumeCharacterLiteral(lexer);
        return true;
    }

    return false;
}


static bool ionLexerTryConsumeWord(Lexer* lexer) {
    if (!ckg_char_is_alpha(lexer->c) && lexer->c != '_') {
        return false;
    }

    while (ckg_char_is_alpha_numeric(ionLexerPeekNthChar(lexer, 0)) || ionLexerPeekNthChar(lexer, 0) == '_') {
        if (ionLexerIsEOF(lexer)) {
            break;
        }

        ionLexerConsumeNextChar(lexer);
    }

    return true;
}

static bool ionLexerConsumeWord(Lexer* lexer) {
    if (!ionLexerTryConsumeWord(lexer)) {
        return false;
    }

    CKG_StringView scratch = ionLexerGetScratchBuffer(lexer);

    IonTokenKind kind = ionTokenGetKeyword(scratch);
    if (kind != ION_TOKEN_ILLEGAL_TOKEN) {
        ionLexerAddToken(lexer, kind);
        return true;
    }

    ionLexerAddToken(lexer, ION_TOKEN_IDENTIFIER);
    return true;
}

static bool ionLexerConsumeSyntax(Lexer* lexer) {
    if (lexer->c == '/') {
        if (ionLexerConsumeOnMatch(lexer, '/')) {
            while (!ionLexerIsEOF(lexer) && ionLexerPeekNthChar(lexer, 0) != '\n') {
                ionLexerConsumeNextChar(lexer);
            }
            // ionLexerAddToken(lexer, ION_TOKEN_COMMENT);
            return true;
        } else if (ionLexerConsumeOnMatch(lexer, '*')) {
            while (!(ionLexerPeekNthChar(lexer, 0) == '*' && ionLexerPeekNthChar(lexer, 1) == '/')) {
                if (ionLexerIsEOF(lexer)) {
                    ionLexerReportError(lexer, "Multiline comment doesn't terminate\n");
                }
                ionLexerConsumeNextChar(lexer);
            }

            ionLexerConsumeNextChar(lexer); // consume '*'
            ionLexerConsumeNextChar(lexer); // consume '/'
            // ionLexerAddToken(lexer, ION_TOKEN_COMMENT);
            return true;
        }
    } else if (lexer->c == '[') {
        if (ionLexerPeekNthChar(lexer, 0) == '.' &&
            ionLexerPeekNthChar(lexer, 1) == '.' &&
            ionLexerPeekNthChar(lexer, 2) == ']') {
            ionLexerConsumeNextChar(lexer); // consume '.'
            ionLexerConsumeNextChar(lexer); // consume '.'
            ionLexerConsumeNextChar(lexer); // consume ']'
        }
    } else if (lexer->c == '<') {
        ionLexerConsumeOnMatch(lexer, '<');
        ionLexerConsumeOnMatch(lexer, '=');
    } else if (lexer->c == '>') {
        ionLexerConsumeOnMatch(lexer, '>');
        ionLexerConsumeOnMatch(lexer, '=');
    } else if (lexer->c == '-') {
        if (ionLexerConsumeOnMatch(lexer, '>')) {} 
        else if (ionLexerConsumeOnMatch(lexer, '-')) {}
        else if (ionLexerConsumeOnMatch(lexer, '=')) {}
    } else if (lexer->c == '+') {
        if (ionLexerConsumeOnMatch(lexer, '+')) {}
        else if (ionLexerConsumeOnMatch(lexer, '=')) {}
    } else if (lexer->c == '|') {
        if (ionLexerConsumeOnMatch(lexer, '|')) {}
        else if (ionLexerConsumeOnMatch(lexer, '=')) {}
    } else if (lexer->c == '&') {
        if (ionLexerConsumeOnMatch(lexer, '&')) {}
        else if (ionLexerConsumeOnMatch(lexer, '=')) {}
    } else if (lexer->c == '!' || lexer->c == '^' || lexer->c == '*' || lexer->c == '=') {
        ionLexerConsumeOnMatch(lexer, '=');
    }

    CKG_StringView buffer = ionLexerGetScratchBuffer(lexer);
    IonTokenKind kind = ionTokenGetSyntax(buffer);
    if (kind != ION_TOKEN_ILLEGAL_TOKEN) {
        ionLexerAddToken(lexer, kind);
        return true;
    }

    return false;
}

static void ionLexerConsumeNextToken(Lexer* lexer) {
    lexer->left_pos = lexer->right_pos;
    ionLexerConsumeNextChar(lexer);

    if (ionIsWhitespace(lexer->c)) {}
    else if (ionLexerConsumeLiteral(lexer)) {}
    else if (ionLexerConsumeWord(lexer)) {}
    else if (ionLexerConsumeSyntax(lexer)) {}
    else {
        ionLexerReportError(lexer, "Illegal token found\n");
    }
}

CKG_Vector(IonToken) ionLexerGenerateTokenStream(u8* source, u64 source_length) {
    Lexer lexer = {0};
    lexer.left_pos = 0;
    lexer.right_pos = 0;
    lexer.line = 1;
    lexer.tokens = NULL;
    lexer.source = ckg_sv_create((const char*)source, source_length);
    lexer.c = '\0';

    while (!ionLexerIsEOF(&lexer)) {
        ionLexerConsumeNextToken(&lexer);
    }

    ckg_vector_push(lexer.tokens, ionTokenCreate(ION_TOKEN_EOF, CKG_SV_NULL(), lexer.line));

    return lexer.tokens;
}