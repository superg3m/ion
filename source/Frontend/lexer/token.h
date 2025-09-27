#pragma once
#include <Core/ckg.h>

typedef enum IonTokenType {
    ION_TOKEN_ILLEGAL_TOKEN,
    ION_TOKEN_EOF,
    ION_TOKEN_NOT,                        // "!"
    ION_TOKEN_BITWISE_NOT,                // "~"
    ION_TOKEN_GENERIC,                    // "$"
    ION_TOKEN_AMPERSAND,                  // "&"
    ION_TOKEN_BITWISE_OR,                 // "|"
    ION_TOKEN_BITWISE_XOR,                // "^"
    ION_TOKEN_LEFT_PAREN,                 // "("
    ION_TOKEN_RIGHT_PAREN,                // ")"
    ION_TOKEN_STAR,                       // "*"
    ION_TOKEN_PLUS,                       // "+"
    ION_TOKEN_COMMA,                      // ","
    ION_TOKEN_MINUS,                      // "-"
    ION_TOKEN_RIGHT_ARROW,                // "->"
    ION_TOKEN_DOT,                        // "."
    ION_TOKEN_DIVISION,                   // "/"
    ION_TOKEN_SEMI_COLON,                 // ";"
    ION_TOKEN_LESS_THAN,                  // "<"
    ION_TOKEN_ASSIGNMENT,                 // "="
    ION_TOKEN_GREATER_THAN,               // ">"
    ION_TOKEN_LEFT_BRACKET,               // "["
    ION_TOKEN_RIGHT_BRACKET,              // "]"
    ION_TOKEN_LEFT_CURLY,                 // "{"
    ION_TOKEN_RIGHT_CURLY,                // "}"
    ION_TOKEN_MODULUS,                    // "%"
    ION_TOKEN_COLON,                      // ":"
    ION_TOKEN_INCREMENT,                  // "++"
    ION_TOKEN_DECREMENT,                  // "--"
    ION_TOKEN_EQUALS,                     // "=="
    ION_TOKEN_NOT_EQUALS,                 // "!="
    ION_TOKEN_PLUS_EQUALS,                // "+="
    ION_TOKEN_MINUS_EQUALS,               // "-="
    ION_TOKEN_MULTIPLICATION_EQUALS,      // "*="
    ION_TOKEN_DIVISION_EQUALS,            // "/="
    ION_TOKEN_GREATER_THAN_EQUALS,        // ">="
    ION_TOKEN_LESS_THAN_EQUALS,           // "<="
    ION_TOKEN_BITWISE_LEFT_SHIFT,         // "<<"
    ION_TOKEN_BITWISE_RIGHT_SHIFT,        // ">>"
    ION_TOKEN_BITWISE_LEFT_SHIFT_EQUALS,  // "<<="
    ION_TOKEN_BITWISE_RIGHT_SHIFT_EQUALS, // ">>="
    ION_TOKEN_COLON_EQUALS,               // ":="
    ION_TOKEN_UNINITIALIZED,              // "---"
    ION_TOKEN_DYNAMIC_ARRAY,              // "[..]"
    ION_TOKEN_AND,                        // "&&"
    ION_TOKEN_OR,                         // "||"
    ION_TOKEN_COMMENT,                    // "//"
    ION_TOKEN_IDENTIFIER,                 // any_word
    ION_TOKEN_UNDERSCORE_IDENTIFIER,       // _
    ION_TOKEN_STRING_LITERAL,             // "TESTING"
    ION_TOKEN_INTEGER_LITERAL,            // 6
    ION_TOKEN_FLOAT_LITERAL,              // 2.523
    ION_TOKEN_CHARACTER_LITERAL,          // 'a'
    ION_TOKEN_IF,                         // if
    ION_TOKEN_ELSE,                       // else
    ION_TOKEN_FOR,                        // for
    ION_TOKEN_WHILE,                      // while
    ION_TOKEN_TRUE,                       // true
    ION_TOKEN_FALSE,                      // false
    ION_TOKEN_NULL,                       // null
    ION_TOKEN_RETURN,                     // return
    ION_TOKEN_VAR,                        // var
    ION_TOKEN_FUNC,                       // func
    ION_TOKEN_PRIMITIVE_TYPE,             // any primitive type
    ION_TOKEN_COUNT
} IonTokenType;

static char* token_strings[ION_TOKEN_COUNT] = {
    stringify(ION_TOKEN_ILLEGAL_TOKEN),
    stringify(ION_TOKEN_EOF),
    stringify(ION_TOKEN_NOT),
    stringify(ION_TOKEN_BITWISE_NOT),
    stringify(ION_TOKEN_GENERIC),
    stringify(ION_TOKEN_AMPERSAND),
    stringify(ION_TOKEN_BITWISE_OR),
    stringify(ION_TOKEN_BITWISE_XOR),
    stringify(ION_TOKEN_LEFT_PAREN),
    stringify(ION_TOKEN_RIGHT_PAREN),
    stringify(ION_TOKEN_STAR),
    stringify(ION_TOKEN_PLUS),
    stringify(ION_TOKEN_COMMA),
    stringify(ION_TOKEN_MINUS),
    stringify(ION_TOKEN_RIGHT_ARROW),
    stringify(ION_TOKEN_DOT),
    stringify(ION_TOKEN_DIVISION),
    stringify(ION_TOKEN_SEMI_COLON),
    stringify(ION_TOKEN_LESS_THAN),
    stringify(ION_TOKEN_ASSIGNMENT),
    stringify(ION_TOKEN_GREATER_THAN),
    stringify(ION_TOKEN_LEFT_BRACKET),
    stringify(ION_TOKEN_RIGHT_BRACKET),
    stringify(ION_TOKEN_LEFT_CURLY),
    stringify(ION_TOKEN_RIGHT_CURLY),
    stringify(ION_TOKEN_MODULUS),
    stringify(ION_TOKEN_COLON),
    stringify(ION_TOKEN_INCREMENT),
    stringify(ION_TOKEN_DECREMENT),
    stringify(ION_TOKEN_EQUALS),
    stringify(ION_TOKEN_NOT_EQUALS),
    stringify(ION_TOKEN_PLUS_EQUALS),
    stringify(ION_TOKEN_MINUS_EQUALS),
    stringify(ION_TOKEN_MULTIPLICATION_EQUALS),
    stringify(ION_TOKEN_DIVISION_EQUALS),
    stringify(ION_TOKEN_GREATER_THAN_EQUALS),
    stringify(ION_TOKEN_LESS_THAN_EQUALS),
    stringify(ION_TOKEN_BITWISE_LEFT_SHIFT),
    stringify(ION_TOKEN_BITWISE_RIGHT_SHIFT),
    stringify(ION_TOKEN_BITWISE_LEFT_SHIFT_EQUALS),
    stringify(ION_TOKEN_BITWISE_RIGHT_SHIFT_EQUALS),
    stringify(ION_TOKEN_COLON_EQUALS),
    stringify(ION_TOKEN_UNINITIALIZED),
    stringify(ION_TOKEN_DYNAMIC_ARRAY),
    stringify(ION_TOKEN_AND),
    stringify(ION_TOKEN_OR),
    stringify(ION_TOKEN_COMMENT),
    stringify(ION_TOKEN_IDENTIFIER),
    stringify(ION_TOKEN_UNDERSCORE_IDENTIFIER),
    stringify(ION_TOKEN_STRING_LITERAL),
    stringify(ION_TOKEN_INTEGER_LITERAL),
    stringify(ION_TOKEN_FLOAT_LITERAL),
    stringify(ION_TOKEN_CHARACTER_LITERAL),
    stringify(ION_TOKEN_IF),
    stringify(ION_TOKEN_ELSE),
    stringify(ION_TOKEN_FOR),
    stringify(ION_TOKEN_WHILE),
    stringify(ION_TOKEN_TRUE),
    stringify(ION_TOKEN_FALSE),
    stringify(ION_TOKEN_NULL),
    stringify(ION_TOKEN_RETURN),
    stringify(ION_TOKEN_VAR),
    stringify(ION_TOKEN_FUNC),
    stringify(ION_TOKEN_PRIMITIVE_TYPE),
};

typedef struct IonToken {
    IonTokenType type;
    CKG_StringView name;
    int line;
    union {
        char c;
        int i;
        double f;
        bool b;
    };
} IonToken;

IonToken ion_token_from_string(IonTokenType token_type, CKG_StringView sv, int line);
#define ION_TOKEN_CREATE_CUSTOM(token_type, name, line) ((IonToken){token_type, name, line, {0}})

void token_print(IonToken token, char* indent);
IonTokenType token_get_keyword(const char* str, u64 str_length);
IonTokenType token_get_type_primitive(const char* str, u64 str_length);
IonTokenType token_get_syntax(const char* str, u64 str_length);

