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
    ION_NK_PRE_INCREMENT_SE,                                         // ++i
    ION_NK_POST_DECREMENT_SE,                                        // i--
    ION_NK_PRE_DECREMENT_SE,                                         // --i
    ION_NK_FUNC_CALL_SE = ION_DEFERRABLE_BIT|ION_EXPRESSION_BIT|ION_STATEMENT_BIT,


    ION_NK_VAR_DECL = ION_DECLARATION_BIT,
    ION_NK_FUNC_DECL,
} IonNodeKind;

typedef struct IonNode {
    IonNodeKind kind;
    u32 desc_count; // every node knows its number of descendants (so we can skip to sibling)
    
    IonToken token; // every node is associated with a specific token and lexeme (from source)
    Type   type; // every node knows their type (unless they do not have it e.g. Stmts)

    union {
        // literal values
        int   i;
        float f;
        bool  b;
        CKG_StringView s;

        // support both print & println using single "print" ast node kind
        bool new_line;

        // func calls
        CKG_Vector(struct IonNode*) arguments;
    } data;

} IonNode;

typedef IonNode IonDeclaration;
typedef IonNode IonExpression;
typedef IonNode IonStatement;

void ionAstPrint(IonNode* ast_root);

IonNode ionNodeCreate(IonNodeKind kind, IonToken token);
bool ionNodeIsLeaf(IonNode* node);
bool ionNodeIsDeclaration(IonNode* node);
bool ionNodeIsStatement(IonNode* node);
bool ionNodeIsExpression(IonNode* node);

IonNode* ionNodeGetLHS(IonNode* node);
IonNode* ionNodeGetRHS(IonNode* node);
IonNode* ionNodeGetExpr(IonNode* node);
IonNode* ionNodeGetUnaryOperand(IonNode* ast);