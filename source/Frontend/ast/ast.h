#pragma once

#include "../lexer/token.h"
#include "../ts/type_system.h"

// |00000|000 00000000 00000000 00000000
// bit 27: LEAF_NODE
// bit 28: DEFERRABLE
// bit 29: EPXRESSION
// bit 30: STATEMENT
// bit 31: DECLARATION

#define ION_LEAF_NODE_BIT (1u << 27)
#define ION_DEFERRABLE_BIT (1u << 28)
#define ION_EXPRESSION_BIT (1u << 29)
#define ION_STATEMENT_BIT (1u << 30)
#define ION_DECLARATION_BIT (1u << 31)

typedef enum NodeKind {
    ION_NK_END = 0,

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
    u32 desc_count;
    Type type;
    union {
        int i;
        float f;
        bool b;
        CKG_StringView s;

        bool new_line; // for print statement

        CKG_Vector(struct IonNode*) arguments; // only for function calls;
    } data;
} IonNode;

IonNode ionNodeCreate(IonNodeKind kind, IonToken token);
bool ionNodeIsLeaf(IonNode* node);
bool ionNodeIsDeclaration(IonNode* node);
bool ionNodeIsStatement(IonNode* node);
bool ionNodeIsExpression(IonNode* node);

IonNode* ionNodeGetExpr(IonNode* node);
IonNode* ionNodeGetOperand(IonNode* ast);
IonNode* ionNodeGetLeft(IonNode* ast);
IonNode* ionNodeGetRight(IonNode* ast);