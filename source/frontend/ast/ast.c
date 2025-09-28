#include "ast.h"

IonNode ionNodeCreate(IonNodeKind kind, IonToken token) {
    IonNode ret;
    ret.kind = kind;
    ret.token = token;
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