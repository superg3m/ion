#include "../ast/ast.h"
#include "type_env.h"


IonType ionTypecheckerIntersectTypes(IonType ty_act, IonType ty_exp) {
    IonType ty_res = ionTypeIntersectInternal(ty_act, ty_exp);
    if (ionTypeIsSignalingPoison(ty_res)) {
        /* @TODO diagnostic for type mismatch */
        ty_res = ionTypePoison();
    }
    return ty_res;
}

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

        case ION_NK_IDENTIFIER_EXPR: {
            CKG_StringView identifier_str = expr->token.lexeme;
            if (!ionTypeEnvHas(env, identifier_str)) {
                // @TODO: complain about undeclared identifier
                return ionTypePoison();
            }

            IonNode* decl = ionTypeEnvGet(env, identifier_str);
            ckg_assert_msg(decl->kind == ION_NK_VAR_DECL);
            return ionNodeGetVarDeclType(decl)->data.type;
        }
        
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
            // right now we assume ASSIGN lhs is a simple ident
            // @TODO: support e.g. `*(addr + offset) = ...`
            CKG_StringView var_name = stmt->token.lexeme;
            
            IonNode* var_decl = ionTypeEnvGet(env, var_name);
            IonType lhs_type = ionNodeGetVarDeclType(var_decl)->data.type;

            IonNode* rhs = ionNodeGetRHS(stmt);
            IonType rhs_type = ionTypecheckExpression(rhs, env);

            ionTypecheckerIntersectTypes(var_decl_type, rhs_type);
            return;
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
            // right now we only support `var <name> = <rvalue_expr>`
            // let's consider "de-structuring" and possibly "refutable bindings"
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

            ionTypecheckerIntersectTypes(ref->data.type, rhs_type);
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