#pragma once
#include <ckg.h>

#define X_SYNTAX_TOKENS              \
    X(ION_TS_PLUS, "+")              \
    X(ION_TS_MINUS, "-")             \
    X(ION_TS_STAR, "*")              \
    X(ION_TS_DIVISION, "/")          \
    X(ION_TS_MODULUS, "%")           \
    X(ION_TS_LEFT_CURLY, "{")        \
    X(ION_TS_RIGHT_CURLY, "}")       \
    X(ION_TS_LEFT_PAREN, "(")        \
    X(ION_TS_RIGHT_PAREN, ")")       \
    X(ION_TS_LEFT_BRACKET, "[")      \
    X(ION_TS_RIGHT_BRACKET, "]")     \
    X(ION_TS_COLON, ":")             \
    X(ION_TS_COMMA, ",")             \
    X(ION_TS_SEMI_COLON, ";")        \
    X(ION_TS_RIGHT_ARROW, "->")      \
                                     \
    X(ION_TS_EQUALS, "=")           \
    X(ION_TS_PLUS_EQUALS, "+=")     \
    X(ION_TS_MINUS_EQUALS, "-=")    \
    X(ION_TS_STAR_EQUALS, "*=")     \
    X(ION_TS_DIVISION_EQUALS, "/=") \
                                     \
    X(ION_TS_LOGICAL_OR, "||")       \
    X(ION_TS_LOGICAL_AND, "&&")      \
                                     \
    X(ION_TS_EQUALS_EQUALS, "==")    \
    X(ION_TS_NOT_EQUALS, "!=")       \
    X(ION_TS_GT, ">")                \
    X(ION_TS_GT_OR_EQUAL, ">=")      \
    X(ION_TS_LT, "<")                \
    X(ION_TS_LT_OR_EQUAL, "<=")      \

#define X_LITERAL_TOKENS \
    X(ION_TL_INTEGER)    \
    X(ION_TL_FLOAT)      \
    X(ION_TL_BOOLEAN)    \
    X(ION_TL_CHARACTER)  \
    X(ION_TL_STRING)     \


#define X_KEYWORD_TOKENS            \
    X(ION_TKW_IF, "if")             \
    X(ION_TKW_ELSE, "else")         \
    X(ION_TKW_FOR, "for")           \
    X(ION_TKW_WHILE, "while")       \
    X(ION_TKW_TRUE, "true")         \
    X(ION_TKW_FALSE, "false")       \
    X(ION_TKW_FN, "fn")             \
    X(ION_TKW_VAR, "var")           \
    X(ION_TKW_NULL, "null")         \
    X(ION_TKW_RETURN, "return")     \
    X(ION_TKW_CONTINUE, "continue") \
    X(ION_TKW_BREAK, "break")       \
    X(ION_TKW_PRINT, "print")       \
    X(ION_TKW_PRINTLN, "println")   \

typedef enum IonTokenKind {
    ION_TOKEN_ILLEGAL_TOKEN,
    ION_TOKEN_EOF,
    
    #define X(name, str) name,
        X_SYNTAX_TOKENS
    #undef X

    #define X(name) name,
        X_LITERAL_TOKENS
    #undef X

    #define X(name, str) name,
        X_KEYWORD_TOKENS
    #undef X

    ION_TOKEN_IDENTIFIER,

    ION_TOKEN_COUNT
} IonTokenKind;

typedef struct IonToken {
    IonTokenKind kind;
    CKG_StringView lexeme;
    int line;
} IonToken;

IonToken ionTokenCreate(IonTokenKind kind, CKG_StringView lexeme, int line);
IonToken ionTokenCreateFake();

const char* ionTokenKindGetString(IonTokenKind kind);
void ionTokenPrint(IonToken token);
IonTokenKind ionTokenGetKeyword(CKG_StringView lexeme);
IonTokenKind ionTokenGetSyntax(CKG_StringView lexeme);

