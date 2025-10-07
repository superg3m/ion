#include "../ast/ast.h"
#include "type_env.h"

IonType ionTypecheckExpression(IonNode* expr, IonTypeEnv* env) {
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

    return (IonType){};
}

void ionTypecheckStatement(IonNode* stmt, IonTypeEnv* env) {
    ckg_assert_msg(ionNodeIsStatement(stmt), "Node is not of type statement");


    switch (stmt->kind) {
        case ION_NK_ASSIGNMENT_STMT: {
            IonNode* var_decl = ionTypeEnvGet(env, stmt->token.lexeme);
            IonType var_decl_type = ionNodeGetVarDeclType(var_decl)->data.type;

            IonNode* rhs = ionNodeGetRHS(stmt);
            IonType rhs_type = ionTypecheckExpression(rhs, env);

            if (ionTypeIntersect(var_decl_type, rhs_type).builtin_type_id == ION_BTYPE_POISON) {
                ckg_assert_msg(false, "Var Decl Type mismatch\n");
            }

            return var_decl_type;
        } break;
        
        default: {
            ckg_assert_msg(false, "Statement kind: %s not handled!\n", ionNodeKindToString(stmt->kind));
        } break; 
    }
}

void ionTypecheckDeclaration(IonNode* decl, IonTypeEnv* env) {
    ckg_assert_msg(ionNodeIsDeclaration(decl), "Node is not of type declaration\n");


    switch (decl->kind) {
        case ION_NK_VAR_DECL: {
            if (ionTypeEnvHas(env, decl->token.lexeme)) {
                ckg_assert_msg(false, "Trying to redeclare identifer: %.*\n", decl->token.lexeme.length, decl->token.lexeme.data);
            }

            ionTypeEnvSet(env, decl->token.lexeme, decl);

            IonNode* ref = ionNodeGetVarDeclType(decl);
            IonNode* rhs = ionNodeGetVarDeclRHS(decl);
            IonType rhs_type = ionTypecheckExpression(rhs, env);

            if (ref->data.type._bits == 0) {
                ref->data.type = rhs_type;
            }

            if (ionTypeIntersect(ref->data.type, rhs_type).builtin_type_id == ION_BTYPE_POISON) {
                ckg_assert_msg(false, "Var Decl Type mismatch\n");
            }
        } break;
        
        default: {
            ckg_assert_msg(false, "Declaration kind: %s not handled!\n", ionNodeKindToString(decl->kind));
        } break; 
    }
}


void ionTypecheckNode(IonNode* node, IonTypeEnv* env) {
    if (ionNodeIsStatement(node)) {
        return ionTypecheckStatement(node, env);
    } else if (ionNodeIsDeclaration(node)) {
        return ionTypecheckDeclaration(node, env);
    } else {
        ckg_assert_msg(false, "Node category: %s not handled!\n", ionNodeKindToString(node->kind));
    }
}