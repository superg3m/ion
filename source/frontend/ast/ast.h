#pragma once

#include "../lexer/token.h"
#include "../ts/type_system.h"

// |00000|000 00000000 00000000 00000000
// bit 27: LEAF_NODE
// bit 28: DEFERRABLE
// bit 29: EPXRESSION
// bit 30: STATEMENT
// bit 31: DECLARATION

typedef enum IonNodeClass {
    ION_CLASS_LEAF_NODE = (1u << 27),
    ION_CLASS_DEFERRABLE = (1u << 28),
    ION_CLASS_EXPRESSION = (1u << 29),
    ION_CLASS_STATEMENT = (1u << 30),
    ION_CLASS_DECLARATION = (1u << 31)
} IonNodeClass;

#define X_NK_LEAF_NODES       \
    X(ION_NK_INTEGER_EXPR)    \
    X(ION_NK_FLOAT_EXPR)      \
    X(ION_NK_BOOLEAN_EXPR)    \
    X(ION_NK_STRING_EXPR)     \
    X(ION_NK_IDENTIFIER_EXPR) \


#define X_NK_EXPRESSION       \
    X(ION_NK_UNARY_EXPR)      \
    X(ION_NK_BINARY_EXPR)     \
    X(ION_NK_GROUPING_EXPR)   \


#define X_NK_STATEMENT        \
    X(ION_NK_ASSIGNMENT_STMT) \
    X(ION_NK_BLOCK_STMT)      \


#define X_NK_DEFFERABLE_STATEMENTS  \
    X(ION_NK_PRINT_STMT) \


#define X_NK_SE                 \
    X(ION_NK_POST_INCREMENT_SE) \
    X(ION_NK_PRE_INCREMENT_SE)  \
    X(ION_NK_POST_DECREMENT_SE) \
    X(ION_NK_PRE_DECREMENT_SE)  \


#define X_NK_DEFFERABLE_SES  \
    X(ION_NK_FUNC_CALL_SE) \


#define X_NK_DECLARATIONS  \
    X(ION_NK_VAR_DECL)     \
    X(ION_NK_FUNC_DECL)    \

typedef enum NodeKind {
    ION_NK_END = 0,
    ION_NK_LIST,
    ION_NK_TYPE_REF,
    ION_NK_PARAM,

    // LEAF NODES
    __LEAF_NODES__ = ION_CLASS_LEAF_NODE|ION_CLASS_EXPRESSION,
    #define X(kind) kind,
        X_NK_LEAF_NODES
    #undef X

    // EXPRESSIONS
    __EXPRESSIONS__ = ION_CLASS_EXPRESSION,
    #define X(kind) kind,
        X_NK_EXPRESSION
    #undef X

    // STATEMENTS
    __STATEMENTS__ = ION_CLASS_STATEMENT,
    #define X(kind) kind,
        X_NK_STATEMENT
    #undef X

    // DEFERRABLE_STATTEMENTS
    __DEFFERABLE_STMTS__ = ION_CLASS_DEFERRABLE|ION_CLASS_STATEMENT,
    #define X(kind) kind,
        X_NK_DEFFERABLE_STATEMENTS
    #undef X

    // STATEMENT EXPRESSIONS
    __SE__ = ION_CLASS_EXPRESSION|ION_CLASS_STATEMENT,
    #define X(kind) kind,
        X_NK_SE
    #undef X

    // DEFERRABLE_SE
    __DEFFERABLE_SES__ = ION_CLASS_DEFERRABLE|ION_CLASS_EXPRESSION|ION_CLASS_STATEMENT,
    #define X(kind) kind,
        X_NK_DEFFERABLE_SES
    #undef X

    // DECLARATIONS
    __DECLARATIONS__ = ION_CLASS_DECLARATION,
    #define X(kind) kind,
        X_NK_DECLARATIONS
    #undef X
} IonNodeKind;

typedef struct IonNode {
    IonNodeKind kind;
    u32 desc_count; // every node knows its number of descendants (so we can skip to sibling)
    
    IonToken token; // every node is associated with a specific token and lexeme (from source)
    union {
        // literal values
        int   i;
        float f;
        bool  b;
        CKG_StringView s;

        // support both print & println using single "print" ast node kind
        bool new_line;

        // Used for things such as param_list or arg_list or even arrays
        int list_count;
    
        // Used for type refs
        IonType type;
    } data;
} IonNode;

void ionAstPrettyPrint(CKG_Vector(IonNode) ast);

IonNode ionNodeCreate(IonNodeKind kind, IonToken token);
bool ionNodeIsLeaf(IonNode* node);
bool ionNodeIsDeclaration(IonNode* node);
bool ionNodeIsStatement(IonNode* node);
bool ionNodeIsExpression(IonNode* node);
const char* ionNodeKindToString(IonNodeKind kind);

IonNode* ionNodeGetLHS(IonNode* node);
IonNode* ionNodeGetRHS(IonNode* node);
IonNode* ionNodeGetExpr(IonNode* node);
IonNode* ionNodeGetUnaryOperand(IonNode* node);

IonNode* ionNodeGetIndex(IonNode* node, int index);

IonNode* ionNodeGetFuncDeclParams(IonNode* node);
IonNode* ionNodeGetFuncDeclReturnType(IonNode* node);
IonNode* ionNodeGetFuncDeclBlock(IonNode* node);

IonNode* ionNodeGetVarDeclType(IonNode* node);
IonNode* ionNodeGetVarDeclRHS(IonNode* node);

IonNode* ionNodeGetParamIdent(IonNode* node);
IonNode* ionNodeGetParamTypeExpr(IonNode* node);

IonNode* ionNodeGetFuncCallArgs(IonNode* node);