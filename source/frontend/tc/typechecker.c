#include "../ast/ast.h"

IonType ionTypecheckExpression(IonNode* expr) {
    ckg_assert_msg(ionNodeIsExpression(expr), "Node is not of type expression");

    switch (expr->kind) {
        case ION_NK_INTEGER_EXPR: {
            return ionTypeInt32();
        } break;

        case ION_NK_FLOAT_EXPR: {
            return ionTypeFloat32();
        } break;

        case ION_NK_BOOLEAN_EXPR: {
            return ionTypeBool();
        } break;

        case ION_NK_STRING_EXPR: {
            return ionTypeStr();
        } break;
        
        default: {
            ckg_assert_msg(false, "Expression kind: %s not handled!\n", ionNodeKindToString(expr->kind));
        } break; 
    }
}

IonType ionTypecheckStatement(IonNode* stmt) {
    switch (stmt->kind) {
        case ION_NK_ASSIGNMENT_STMT: {
            // IonNode* var_decl = ionTypeEnvGet(env, stmt->token.lexeme);

            // IonNode* rhs = ionNodeGetRHS(stmt);
            // IonType rhs_type = ion

            // if (ionTypeIntersect() == false) {
                // error
            // }

            // return var_decl_type
        } break;
        
        default: {
            ckg_assert_msg(false, "Statement kind: %s not handled!\n", ionNodeKindToString(stmt->kind));
        } break; 
    }
}

IonType ionTypecheckDeclaration() {}


IonType ionTypecheckNode(IonNode* node) {
    if (ionNodeIsExpression(node)) {
        return ionTypecheckExpression(node);
    } else if (ionNodeIsStatement(node)) {
        return ionTypecheckStatement(node);
    } else if (ionNodeIsDeclaration(node)) {
        return ionTypecheckDeclaration(node);
    } else {
        ckg_assert_msg(false, "Node category: %s not handled!\n", ionNodeKindToString(node->kind));
    }
}