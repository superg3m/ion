#pragma once

#include "../lexer/token.h"

// |00000|000 00000000 00000000 00000000
// bit 27: LEAF_NODE
// bit 28: DEFERRABLE
// bit 29: EPXRESSION
// bit 30: STATEMENT
// bit 31: DECLARATION

#define ION_LEAF_NODE_BIT ((unsigned int)(1 << 27))
#define ION_DEFERRABLE_BIT ((unsigned int)(1 << 28))
#define ION_EXPRESSION_BIT ((unsigned int)(1 << 29))
#define ION_STATEMENT_BIT ((unsigned int)(1 << 30))
#define ION_DECLARATION_BIT ((unsigned int)(1 << 31))

typedef enum NodeKind {
    ION_NK_UNARY_EXPR = ION_EXPRESSION_BIT,
    ION_NK_BINARY_EXPR,
    ION_NK_GROUPING_EXPR,

    // LEAF NODES
    ION_NK_INTEGER_EXPR = ION_LEAF_NODE_BIT|ION_EXPRESSION_BIT,
    ION_NK_FLOAT_EXPR,
    ION_NK_BOOLEAN_EXPR,
    ION_NK_STRING_EXPR,
    ION_NK_IDENTIFIER_EXPR,


    ION_NK_ASSIGNMENT_STMT = ION_STATEMENT_BIT,
    ION_NK_BLOCK_STMT,
    ION_NK_PRINT_STMT = ION_DEFERRABLE_BIT|(ION_NK_BLOCK_STMT + 1),
    ION_NK_PRINTLN_STMT,

    ION_NK_POST_INCREMENT_SE = ION_EXPRESSION_BIT|ION_STATEMENT_BIT, // i++
    ION_NK_PRE_INCREMENT_SE = ION_EXPRESSION_BIT|ION_STATEMENT_BIT,  // ++i
    ION_NK_POST_DECREMENT_SE = ION_EXPRESSION_BIT|ION_STATEMENT_BIT, // i--
    ION_NK_PRE_DECREMENT_SE = ION_EXPRESSION_BIT|ION_STATEMENT_BIT,  // --i
    ION_NK_FUNC_CALL_SE = ION_DEFERRABLE_BIT|ION_EXPRESSION_BIT|ION_STATEMENT_BIT,


    ION_NK_VAR_DECL = ION_DECLARATION_BIT,
    ION_NK_FUNC_DECL,
} IonNodeKind;

typedef struct IonNode {
    IonNodeKind kind;
    IonToken token; // return, binary: operator, int, float, string, bool, identifer
    union {
        u32 desc_count;

        // if leaf
        int i;
        float f;
        bool b;
        CKG_StringView s;
    } data;
} IonNode;

IonNode ionNodeCreate(IonNodeKind kind, IonToken token) {
    IonNode ret;
    ret.kind = kind;
    ret.token = token;
    ret.data = {0};

    return ret;
}

bool ionNodeIsLeaf(IonNode node) {
    return node.kind & ION_DECLARATION_BIT;
}

bool ionNodeIsDeclaration(IonNode node) {
    return node.kind & ION_DECLARATION_BIT;
}

bool ionNodeIsStatement(IonNode node) {
    return node.kind & ION_STATEMENT_BIT;
}

bool ionNodeIsExpression(IonNode node) {
    return node.kind & ION_EXPRESSION_BIT;
}

// PARSING

// <Primary>    ::= <integer> | <float> | <boolean> | <string> | '(' <Expression> ')'
int ionParsePrimaryExpression(IonParser* parser, IonNode* ast, int index) {
    int start = index;
    IonToken current = ionParserPeekNthToken(parser, 0);

    if (ionParserConsumeOnMatch(parser, Token.INTEGER_LITERAL)) {
        IonNode node = ionNodeCreate(ION_NK_INTEGER_EXPR, current);
        node.data.i = atoi(current.lexeme);
        ast[index] = node;
        return index + 1;
    } else if (ionParserConsumeOnMatch(parser, Token.BOOLEAN_LITERAL)) {
        IonNode node = ionNodeCreate(ION_NK_BOOLEAN_EXPR, current);
        node.data.b = ckg_str_equal(current.lexeme.data, current.lexeme.length,
                                    "true", sizeof("true") - 1);
        ast[index] = node;
        return index + 1;
    } else if (ionParserConsumeOnMatch(parser, Token.FLOAT_LITERAL)) {
        IonNode node = ionNodeCreate(ION_NK_FLOAT_EXPR, current);
        node.data.f = atof(current.lexeme);
        ast[index] = node;
        return index + 1;
    } else if (ionParserConsumeOnMatch(parser, Token.STRING_LITERAL)) {
        IonNode node = ionNodeCreate(ION_NK_STRING_EXPR, current);
        node.data.s = current.lexeme;
        ast[index] = node;
        return index + 1;
    } else if (ionParserConsumeOnMatch(parser, Token.LEFT_PAREN)) {
        IonNode node = ionNodeCreate(ION_NK_GROUPING_EXPR, current);
        ast[index] = node;

        index = ionParseExpression(parser, ast, index); // parse inside parens
        ionParserExpect(parser, Token.RIGHT_PAREN);

        ast[index].data.desc_count = index - start;

        /*
        memmove(&ast[start + 1], &ast[start], desc_count * sizeof(IonNode));
        */

        return start + 1 + ast[index].data.desc_count;
    }

    return -1;
}

// 3 + 6 * 2

// [bin(+), 3, bin(*), 6, 4]

// <additive> ::= <Factor> (('+'|'-') <Factor>)*
int ionParseAdditiveExpression(Parser* parser, Node* ast, int index) {
    int start = index; // index where this additive expression begins
    index = ionParseMultiplicativeExpression(parser, ast, index); // parse left operand

    while (parser.consumeOnMatch(Token.PLUS) || parser.consumeOnMatch(Token.MINUS)) {
        IonToken op = parser.previousToken();

        index = ionParseMultiplicativeExpression(parser, ast, index);

        IonNode node = ionNodeCreate(ION_NK_BINARY_EXPR, op);
        node.data.desc_count = index - start;

        memmove(&ast[left_start + 1], &ast[start], (node.data.desc_count - 1) * sizeof(IonNode));
        ast[start] = node;

        index = start + node.data.desc_count;
    }

    return index;
}

int ionParseExpression(Parser* parser, Node* ast, int index) {
    return ionParsePrimaryExpression(parser, ast, index);
}