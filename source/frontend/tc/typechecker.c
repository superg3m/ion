#include "../ast/ast.h"
#include "type_env.h"

CKG_HashMap(CKG_StringView, IonNode*)* global_function_declarations = NULLPTR;

void ionTypecheckNode(IonNode* node, IonTypeEnv* env);

IonType ionTypecheckerIntersectTypes(IonType ty_act, IonType ty_exp) {
    IonType ty_res = ionTypeIntersectInternal(ty_act, ty_exp);
    if (ionTypeIsSignalingPoison(ty_res)) {
        /* @TODO diagnostic for type mismatch */
        ty_res = ionTypePoison();
    }
    return ty_res;
}

IonType ionTypecheckExpression(IonNode* expr, IonTypeEnv* env) {
    ckg_assert_msg(ionNodeIsExpression(expr), "Node is not of type expression\n");

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
            ckg_assert_msg(decl->kind == ION_NK_VAR_DECL, "NON-Binary identifier\n");
            return ionNodeGetDeclType(decl)->data.type;
        } break;


        case ION_NK_UNARY_EXPR: {
            IonType operand_type = ionTypecheckExpression(ionNodeGetUnaryOperand(expr), env);

            IonType unary_type = ionTypeUnaryCheck(expr->token.kind, operand_type);
            if (ionTypeIsPoison(unary_type)) {
                // @TODO: complain about undeclared identifier
                ckg_assert(false);
                return ionTypePoison();
            }

            return unary_type;
        } break;

        case ION_NK_BINARY_EXPR: {
            IonType left_type = ionTypecheckExpression(ionNodeGetLHS(expr), env);
            IonType right_type = ionTypecheckExpression(ionNodeGetRHS(expr), env);

            IonType promoted_type = ionTypeBinaryPromote(expr->token.kind, left_type, right_type);
            if (ionTypeIsPoison(promoted_type)) {
                // @TODO: complain about undeclared identifier
                ckg_assert(false);
                return ionTypePoison();
            }

            return promoted_type;
        } break;
        
        default: {
            ckg_assert_msg(false, "Expression kind: %s not handled!\n", ionNodeKindToString(expr->kind));
        } break; 
    }

    return (IonType){};
}

void ionTypecheckStatement(IonNode* stmt, IonTypeEnv* env) {
    ckg_assert_msg(ionNodeIsStatement(stmt), "Node is not of type statement\n");

    switch (stmt->kind) {
        case ION_NK_ASSIGNMENT_STMT: {
            // right now we assume ASSIGN lhs is a simple ident
            // @TODO: support e.g. `*(addr + offset) = ...`
            CKG_StringView var_name = stmt->token.lexeme;
            
            IonNode* var_decl = ionTypeEnvGet(env, var_name);
            IonType lhs_type = ionNodeGetDeclType(var_decl)->data.type;

            IonNode* rhs = ionNodeGetRHS(stmt);
            IonType rhs_type = ionTypecheckExpression(rhs, env);

            ionTypecheckerIntersectTypes(lhs_type, rhs_type);
        } break;

        case ION_NK_BLOCK_STMT: {
            for (int i = 0; i < stmt->data.list_count; i++) {
                ionTypecheckNode(ionNodeGetIndex(stmt, i), env);
            }
        } break;

        case ION_NK_PRINT_STMT: {
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
                ckg_assert_msg(false, "Trying to redeclare identifer: %.*s\n", decl->token.lexeme.length, decl->token.lexeme.data);
            }

            ionTypeEnvSet(env, decl->token.lexeme, decl);

            IonNode* type_node = ionNodeGetDeclType(decl);
            IonNode* rhs = ionNodeGetVarDeclRHS(decl);
            IonType rhs_type = ionTypecheckExpression(rhs, env);

            if (ionTypeIsPlaceholder(type_node->data.type)) {
                type_node->data.type = rhs_type;
            }

            // We might want to actually assign poison 
            // but maybe we want to perfer the decl ??? who knows man
            // var a: int = "int";

            ionTypecheckerIntersectTypes(type_node->data.type, rhs_type);
        } break;

        case ION_NK_FUNC_DECL: {
            if (ckg_hashmap_has(global_function_declarations, decl->token.lexeme)) {
                ckg_assert_msg(false, "Trying to redeclare identifer: %.*s\n", decl->token.lexeme.length, decl->token.lexeme.data);
            }

            ckg_hashmap_put(global_function_declarations, decl->token.lexeme, decl);
            
            IonTypeEnv func_env = ionTypeEnvCreate(env);
            IonNode* param_list = ionNodeGetFuncDeclParams(decl);
            for (int i = 0; i < param_list->data.list_count; i++) {
                IonNode* param_decl = ionNodeGetIndex(param_list, i);
                ionTypeEnvSet(&func_env, param_decl->token.lexeme, param_decl);
            }

            // IonNode* return_type = ionNodeGetFuncDeclReturnType(decl);
            IonNode* block = ionNodeGetFuncDeclBlock(decl);
            ionTypecheckStatement(block, &func_env);
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

void ionTypecheckProgram(CKG_Vector(IonNode) ast) {
    ckg_hashmap_init_string_view_hash(global_function_declarations, CKG_StringView, IonNode*);
    IonTypeEnv global_type_env = ionTypeEnvCreate(NULLPTR);

    for (IonNode* decl = ast; decl->kind != ION_NK_END; decl += (1 + decl->desc_count)) {
        ionTypecheckDeclaration(decl, &global_type_env);
    }
}