#include "ast.h"

IonNode ionNodeCreate(IonNodeKind kind, IonToken token) {
    IonNode ret;
    ret.kind = kind;
    ret.token = token;
    ret.desc_count = 0;
    ckg_memory_zero(&ret.data, sizeof(ret.data));
   
    return ret;
}

bool ionNodeIsLeaf(IonNode* node) {
    return node->kind & ION_LEAF_NODE_BIT;
}


bool ionNodeIsExpression(IonNode* node) {
    return node->kind & ION_EXPRESSION_BIT;
}

bool ionNodeIsStatement(IonNode* node) {
    return node->kind & ION_STATEMENT_BIT;
}

bool ionNodeIsDeclaration(IonNode* node) {
    return node->kind & ION_DECLARATION_BIT;
}

IonNode* ionNodeGetExpr(IonNode* node) {
    ckg_assert(
        node->kind == ION_NK_GROUPING_EXPR ||
        node->kind == ION_NK_PRINT_STMT
    );

    return node + 1;
}

/*
// This doesn't wokr yet because parsing is incorrect...
IonNode* ionNodeGetRHS(IonNode* node) {
    ckg_assert(node->kind == ION_NK_ASSIGNMENT_STMT);

    // return node + 1;
}
*/

IonNode* ionNodeGetRHS(IonNode* node) {
    ckg_assert(node->kind == ION_NK_ASSIGNMENT_STMT);

    return node + 1;
}

IonNode* ionNodeGetOperand(IonNode* node) {
    ckg_assert(node->kind == ION_NK_UNARY_EXPR);

    return node + 1;
}

IonNode* ionNodeGetLeft(IonNode* node) {
    ckg_assert(node->kind == ION_NK_BINARY_EXPR);

    return node + 1;
}

IonNode* ionNodeGetRight(IonNode* node) {
    ckg_assert(node->kind == ION_NK_BINARY_EXPR);
    IonNode* left = ionNodeGetLeft(node);

    return left + 1 + left->desc_count;
}