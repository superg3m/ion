#include "ast.h"

IonNode ionNodeCreate(IonNodeKind kind, IonToken token) {
    IonNode ret;
    ret.kind = kind;
    ret.token = token;
    ret.desc_count = 0;
    ckg_memory_zero(&ret.data, sizeof(ret.data));
   
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

IonNode* ionNodeGetExpr(CKG_Vector(IonNode) ast, int index) {
    ckg_assert(ast[index].kind == ION_NK_GROUPING_EXPR);

    return &ast[index + 1];
}

IonNode* ionNodeGetOperand(CKG_Vector(IonNode) ast, int index) {
    ckg_assert(ast[index].kind == ION_NK_UNARY_EXPR);

    return &ast[index + 1];
}

IonNode* ionNodeGetLeft(CKG_Vector(IonNode) ast, int index) {
    ckg_assert(ast[index].kind == ION_NK_BINARY_EXPR);

    return &ast[index + 1];
}

// Not yet correct?
IonNode* ionNodeGetRight(CKG_Vector(IonNode) ast, int index) {
    ckg_assert(ast[index].kind == ION_NK_BINARY_EXPR);

    IonNode* left = ionNodeGetLeft(ast, index);

    return &ast[index + 1 + left->desc_count];
}