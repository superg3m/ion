#pragma once
#include <ckg.h>

typedef enum SPL_TokenType {
    SPL_TOKEN_ILLEGAL_TOKEN,
    SPL_TOKEN_EOF,
    SPL_TOKEN_NOT,                        // "!"
    SPL_TOKEN_BITWISE_NOT,                // "~"
    SPL_TOKEN_GENERIC,                    // "$"
    SPL_TOKEN_AMPERSAND,                  // "&"
    SPL_TOKEN_BITWISE_OR,                 // "|"
    SPL_TOKEN_BITWISE_XOR,                // "^"
    SPL_TOKEN_LEFT_PAREN,                 // "("
    SPL_TOKEN_RIGHT_PAREN,                // ")"
    SPL_TOKEN_STAR,                       // "*"
    SPL_TOKEN_PLUS,                       // "+"
    SPL_TOKEN_COMMA,                      // ","
    SPL_TOKEN_MINUS,                      // "-"
    SPL_TOKEN_DOT,                        // "."
    SPL_TOKEN_DIVISION,                   // "/"
    SPL_TOKEN_SEMI_COLON,                 // ";"
    SPL_TOKEN_LESS_THAN,                  // "<"
    SPL_TOKEN_ASSIGNMENT,                 // "="
    SPL_TOKEN_GREATER_THAN,               // ">"
    SPL_TOKEN_LEFT_BRACKET,               // "["
    SPL_TOKEN_RIGHT_BRACKET,              // "]"
    SPL_TOKEN_LEFT_CURLY,                 // "{"
    SPL_TOKEN_RIGHT_CURLY,                // "}"
    SPL_TOKEN_MODULUS,                    // "%"
    SPL_TOKEN_COLON,                      // ":"
    SPL_TOKEN_INCREMENT,                  // "++"
    SPL_TOKEN_DECREMENT,                  // "--"
    SPL_TOKEN_EQUALS,                     // "=="
    SPL_TOKEN_NOT_EQUALS,                 // "!="
    SPL_TOKEN_PLUS_EQUALS,                // "+="
    SPL_TOKEN_MINUS_EQUALS,               // "-="
    SPL_TOKEN_MULTIPLICATION_EQUALS,      // "*="
    SPL_TOKEN_DIVISION_EQUALS,            // "/="
    SPL_TOKEN_GREATER_THAN_EQUALS,        // ">="
    SPL_TOKEN_LESS_THAN_EQUALS,           // "<="
    SPL_TOKEN_BITWISE_LEFT_SHIFT,         // "<<"
    SPL_TOKEN_BITWISE_RIGHT_SHIFT,        // ">>"
    SPL_TOKEN_BITWISE_LEFT_SHIFT_EQUALS,  // "<<="
    SPL_TOKEN_BITWISE_RIGHT_SHIFT_EQUALS, // ">>="
    SPL_TOKEN_UNINITIALIZED,              // "---"
    SPL_TOKEN_DYNAMIC_ARRAY,              // "[..]"
    SPL_TOKEN_AND,                        // "&&"
    SPL_TOKEN_OR,                         // "||"
    SPL_TOKEN_COMMENT,                    // "//"
    SPL_TOKEN_IDENTIFIER,                 // any_word
    SPL_TOKEN_STRING_LITERAL,             // "TESTING"
    SPL_TOKEN_INTEGER_LITERAL,            // 6
    SPL_TOKEN_FLOAT_LITERAL,              // 2.523
    SPL_TOKEN_CHARACTER_LITERAL,          // 'a'
    SPL_TOKEN_IF,                         // if
    SPL_TOKEN_ELSE,                       // else
    SPL_TOKEN_FOR,                        // for
    SPL_TOKEN_WHILE,                      // while
    SPL_TOKEN_TRUE,                       // true
    SPL_TOKEN_FALSE,                      // false
    SPL_TOKEN_NULL,                       // null
    SPL_TOKEN_RETURN,                     // return
    SPL_TOKEN_PRINT,                      // print(<Expression>)
    SPL_TOKEN_PRINTLN,                    // println(<Expression>)
    SPL_TOKEN_COUNT
} SPL_TokenType;

static char* token_strings[SPL_TOKEN_COUNT] = {
    stringify(SPL_TOKEN_ILLEGAL_TOKEN),
    stringify(SPL_TOKEN_EOF),
    stringify(SPL_TOKEN_NOT),
    stringify(SPL_TOKEN_BITWISE_NOT),
    stringify(SPL_TOKEN_GENERIC),
    stringify(SPL_TOKEN_AMPERSAND),
    stringify(SPL_TOKEN_BITWISE_OR),
    stringify(SPL_TOKEN_BITWISE_XOR),
    stringify(SPL_TOKEN_LEFT_PAREN),
    stringify(SPL_TOKEN_RIGHT_PAREN),
    stringify(SPL_TOKEN_STAR),
    stringify(SPL_TOKEN_PLUS),
    stringify(SPL_TOKEN_COMMA),
    stringify(SPL_TOKEN_MINUS),
    stringify(SPL_TOKEN_DOT),
    stringify(SPL_TOKEN_DIVISION),
    stringify(SPL_TOKEN_SEMI_COLON),
    stringify(SPL_TOKEN_LESS_THAN),
    stringify(SPL_TOKEN_ASSIGNMENT),
    stringify(SPL_TOKEN_GREATER_THAN),
    stringify(SPL_TOKEN_LEFT_BRACKET),
    stringify(SPL_TOKEN_RIGHT_BRACKET),
    stringify(SPL_TOKEN_LEFT_CURLY),
    stringify(SPL_TOKEN_RIGHT_CURLY),
    stringify(SPL_TOKEN_MODULUS),
    stringify(SPL_TOKEN_COLON),
    stringify(SPL_TOKEN_INCREMENT),
    stringify(SPL_TOKEN_DECREMENT),
    stringify(SPL_TOKEN_EQUALS),
    stringify(SPL_TOKEN_NOT_EQUALS),
    stringify(SPL_TOKEN_PLUS_EQUALS),
    stringify(SPL_TOKEN_MINUS_EQUALS),
    stringify(SPL_TOKEN_MULTIPLICATION_EQUALS),
    stringify(SPL_TOKEN_DIVISION_EQUALS),
    stringify(SPL_TOKEN_GREATER_THAN_EQUALS),
    stringify(SPL_TOKEN_LESS_THAN_EQUALS),
    stringify(SPL_TOKEN_BITWISE_LEFT_SHIFT),
    stringify(SPL_TOKEN_BITWISE_RIGHT_SHIFT),
    stringify(SPL_TOKEN_BITWISE_LEFT_SHIFT_EQUALS),
    stringify(SPL_TOKEN_BITWISE_RIGHT_SHIFT_EQUALS),
    stringify(SPL_TOKEN_UNINITIALIZED),
    stringify(SPL_TOKEN_DYNAMIC_ARRAY),
    stringify(SPL_TOKEN_AND),
    stringify(SPL_TOKEN_OR),
    stringify(SPL_TOKEN_COMMENT),
    stringify(SPL_TOKEN_IDENTIFIER),
    stringify(SPL_TOKEN_STRING_LITERAL),
    stringify(SPL_TOKEN_INTEGER_LITERAL),
    stringify(SPL_TOKEN_FLOAT_LITERAL),
    stringify(SPL_TOKEN_CHARACTER_LITERAL),
    stringify(SPL_TOKEN_IF),
    stringify(SPL_TOKEN_ELSE),
    stringify(SPL_TOKEN_FOR),
    stringify(SPL_TOKEN_WHILE),
    stringify(SPL_TOKEN_TRUE),
    stringify(SPL_TOKEN_FALSE),
    stringify(SPL_TOKEN_NULL),
    stringify(SPL_TOKEN_RETURN),
    stringify(SPL_TOKEN_PRINT),
    stringify(SPL_TOKEN_PRINTLN)
};

typedef struct SPL_Token {
    SPL_TokenType type;
    CKG_StringView name;
    union {
        char c;
        int i;
        double f;
        bool b;
    };
    int line;
} SPL_Token;

SPL_Token spl_token_from_string(CKG_StringView sv, int line);

#define SPL_TOKEN_CREATE(value, line) spl_token_from_string(value, line)
#define SPL_TOKEN_CREATE_CUSTOM(token_type, name, line) (SPL_Token){(token_type), (name), 0, (line)}

void token_print(SPL_Token token, char* indent);
SPL_TokenType token_get_keyword(const char* str, u64 str_length);
SPL_TokenType token_get_syntax(const char* str, u64 str_length);