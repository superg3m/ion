#include <ckg.h>
#include "../ast/ast.h"

#include "scope.h"


Scope global_scope;
CKG_HashMap(CKG_StringView, IonDeclaration*)* global_function = NULLPTR;

// NOTE(Jovanni) this is where it gets hairy because you want to not 
// have to heap allocate these. But the way its structured makes this impossible because stack lifetimes aren't long enough...
IonExpression* ionEvaluateIntegers(IonToken token, int lhs, int rhs) {
    IonExpression* ret = ckg_alloc(sizeof(IonExpression));
    ret->token = token;
	switch (token.kind) {
        case ION_TS_PLUS: {
            ret->kind = ION_NK_INTEGER_EXPR;
            ret->data.i = lhs + rhs;

            return ret;
        } break;

        case ION_TS_MINUS: {
            ret->kind = ION_NK_INTEGER_EXPR;
            ret->data.i = lhs - rhs;

            return ret;
        } break;

        case ION_TS_STAR: {
            ret->kind = ION_NK_INTEGER_EXPR;
            ret->data.i = lhs * rhs;

            return ret;
        } break;
 
        case ION_TS_DIVISION: {
            ret->kind = ION_NK_INTEGER_EXPR;
            ret->data.i = lhs / rhs;

            return ret;
        } break;

        case ION_TS_EQUALS_EQUALS: {
            ret->kind = ION_NK_BOOLEAN_EXPR;
            ret->data.b = lhs == rhs;

            return ret;
        } break;

        case ION_TS_NOT_EQUALS: {
            ret->kind = ION_NK_BOOLEAN_EXPR;
            ret->data.b = lhs != rhs;

            return ret;
        } break;

        case ION_TS_LT: {
            ret->kind = ION_NK_BOOLEAN_EXPR;
            ret->data.b = lhs < rhs;

            return ret;
        } break;

        case ION_TS_LT_OR_EQUAL: {
            ret->kind = ION_NK_BOOLEAN_EXPR;
            ret->data.b = lhs <= rhs;

            return ret;
        } break;

        case ION_TS_GT: {
            ret->kind = ION_NK_BOOLEAN_EXPR;
            ret->data.b = lhs > rhs;

            return ret;
        } break;

        case ION_TS_GT_OR_EQUAL: {
            ret->kind = ION_NK_BOOLEAN_EXPR;
            ret->data.b = lhs >= rhs;

            return ret;
        } break;

        default: {
            ckg_assert(false);
        } break;
	}

	ckg_assert(false);
    return NULLPTR;
}

// NOTE(Jovanni) this is where it gets hairy because you want to not 
// have to heap allocate these. But the way its structured makes this impossible because stack lifetimes aren't long enough...
IonExpression* ionEvaluateFloats(IonToken token, float lhs, float rhs) {
    IonExpression* ret = ckg_alloc(sizeof(IonExpression));
    ret->token = token;
	switch (token.kind) {
        case ION_TS_PLUS: {
            ret->kind = ION_NK_FLOAT_EXPR;
            ret->data.f = lhs + rhs;

            return ret;
        } break;

        case ION_TS_MINUS: {
            ret->kind = ION_NK_FLOAT_EXPR;
            ret->data.f = lhs - rhs;

            return ret;
        } break;

        case ION_TS_STAR: {
            ret->kind = ION_NK_FLOAT_EXPR;
            ret->data.f = lhs * rhs;

            return ret;
        } break;
 
        case ION_TS_DIVISION: {
            ret->kind = ION_NK_FLOAT_EXPR;
            ret->data.f = lhs / rhs;

            return ret;
        } break;

        case ION_TS_EQUALS_EQUALS: {
            ret->kind = ION_NK_BOOLEAN_EXPR;
            ret->data.b = lhs == rhs;

            return ret;
        } break;

        case ION_TS_NOT_EQUALS: {
            ret->kind = ION_NK_BOOLEAN_EXPR;
            ret->data.b = lhs != rhs;

            return ret;
        } break;

        case ION_TS_LT: {
            ret->kind = ION_NK_BOOLEAN_EXPR;
            ret->data.b = lhs < rhs;

            return ret;
        } break;

        case ION_TS_LT_OR_EQUAL: {
            ret->kind = ION_NK_BOOLEAN_EXPR;
            ret->data.b = lhs <= rhs;

            return ret;
        } break;

        case ION_TS_GT: {
            ret->kind = ION_NK_BOOLEAN_EXPR;
            ret->data.b = lhs > rhs;

            return ret;
        } break;

        case ION_TS_GT_OR_EQUAL: {
            ret->kind = ION_NK_BOOLEAN_EXPR;
            ret->data.b = lhs >= rhs;

            return ret;
        } break;

        default: {
            ckg_assert(false);
        } break;
	}

	ckg_assert(false);
    return NULLPTR;
}

IonExpression* ionInterpretBinaryExpression(IonToken token, IonExpression* left, IonExpression* right) {
    switch (token.kind) {
        case ION_TS_PLUS:
        case ION_TS_MINUS:
        case ION_TS_STAR:
        case ION_TS_DIVISION:
        case ION_TS_LT:
        case ION_TS_LT_OR_EQUAL:
        case ION_TS_GT:
        case ION_TS_GT_OR_EQUAL:
        case ION_TS_EQUALS_EQUALS:
        case ION_TS_NOT_EQUALS: {
            if (left->kind == ION_NK_INTEGER_EXPR && right->kind == ION_NK_INTEGER_EXPR) {
                return ionEvaluateIntegers(token, left->data.i, right->data.i);
            }

            if (left->kind == ION_NK_FLOAT_EXPR && right->kind == ION_NK_FLOAT_EXPR) {
                return ionEvaluateFloats(token, left->data.f, right->data.f);
            }

            if (left->kind == ION_NK_INTEGER_EXPR && right->kind == ION_NK_FLOAT_EXPR) {
                return ionEvaluateFloats(token, (float)left->data.i, right->data.f);
            }

            if (left->kind == ION_NK_FLOAT_EXPR && right->kind == ION_NK_INTEGER_EXPR) {
                return ionEvaluateFloats(token, left->data.f, (float)right->data.i);
            }

            /*
            if (left->kind == ION_NK_STRING_EXPR) {
                if (right->kind == ION_NK_INTEGER_EXPR) {
                    char buf[32];
                    snprintf(buf, sizeof(buf), "%d", right->data.i);
                    return ionEvaluateStrings(token, left->data.s, buf);
                } else if (right->kind == ION_NK_FLOAT_EXPR) {
                    char buf[32];
                    snprintf(buf, sizeof(buf), "%.5g", right->data.f);
                    return ionEvaluateStrings(token, left->data.s, buf);
                } else if (right->kind == ION_NK_STRING_EXPR) {
                    return ionEvaluateStrings(token, left->data.s, right->data.s);
                }
            }


            if (right->kind == ION_NK_STRING_EXPR) {
                if (left->kind == ION_NK_INTEGER_EXPR) {
                    char buf[32];
                    snprintf(buf, sizeof(buf), "%d", left->data.i);
                    return ionEvaluateStrings(token, buf, right->data.s);
                } else if (left->kind == ION_NK_FLOAT_EXPR) {
                    char buf[32];
                    snprintf(buf, sizeof(buf), "%.5g", left->data.f);
                    return ionEvaluateStrings(token, buf, right->data.s);
                } else if (left->kind == ION_NK_STRING_EXPR) {
                    return ionEvaluateStrings(token, left->data.s, right->data.s);
                }
            }
            */

            fprintf(stderr, "invalid operands for token %d\n", token.kind);
            exit(1);
        } break;

        case ION_TS_LOGICAL_AND:
        case ION_TS_LOGICAL_OR: {
            if (left->kind != ION_NK_BOOLEAN_EXPR || right->kind != ION_NK_BOOLEAN_EXPR) {
                fprintf(stderr, "expected booleans for token %d\n", token.kind);
                exit(1);
            }

            IonExpression* ret = malloc(sizeof(IonExpression));
            ret->token = token;
            ret->kind = ION_NK_BOOLEAN_EXPR;

            if (token.kind == ION_TS_LOGICAL_AND) {
                ret->data.b = left->data.b && right->data.b;
            } else {
                ret->data.b = left->data.b || right->data.b;
            }

            return ret;
        } break;

        default: {
            fprintf(stderr, "unhandled operator: %d\n", token.kind);
            exit(1);
        } break;
    }
}

IonNode* ionInterpretNode(IonNode* node, Scope* scope);
IonNode* ionInterpretNodes(IonNode* node, Scope* scope) {
    int inital_desc_count = node->desc_count;
    int current_desc_count = 0;

    node += 1;
    while (inital_desc_count != current_desc_count) {
        current_desc_count += (1 + node->desc_count);
		node = ionInterpretNode(node, scope);
	}

    return node;
}

IonExpression* ionInterpretExpression(IonExpression* expr, Scope* scope) {
    switch (expr->kind) {
        case ION_NK_INTEGER_EXPR:
        case ION_NK_BOOLEAN_EXPR:
        case ION_NK_FLOAT_EXPR:
        case ION_NK_STRING_EXPR: {
            return expr;
        } break;

        case ION_NK_BINARY_EXPR: {
            IonNode* left = ionInterpretExpression(ionNodeGetLHS(expr), scope);
            IonNode* right = ionInterpretExpression(ionNodeGetRHS(expr), scope);

            return ionInterpretBinaryExpression(expr->token, left, right);
        } break;

        case ION_NK_IDENTIFIER_EXPR: {
            return ionScopeGet(scope, expr->token.lexeme);
        } break;
        
        default: {
            ckg_assert(false);
        } break;
    }

    return NULLPTR;
}

void ionPrintExpression(IonExpression* expr, Scope* scope) {
    ckg_assert(ionNodeIsExpression(expr));

    switch (expr->kind) {
        case ION_NK_INTEGER_EXPR: {
            CKG_LOG_PRINT("%d", expr->data.i);
        } break;

        case ION_NK_FLOAT_EXPR: {
            CKG_LOG_PRINT("%.4g", expr->data.f);
        } break;

        case ION_NK_BOOLEAN_EXPR: {
            CKG_LOG_PRINT("%s", expr->data.b ? "true" : "else");
        } break;

        case ION_NK_STRING_EXPR: {
            // fixNewLineCode()
            // CKG_LOG_PRINT("%.*s", expr->data.s);
        } break;

        case ION_NK_IDENTIFIER_EXPR: {
            ionPrintExpression(ionInterpretExpression(expr, scope), scope);
        } break;

        default: {
            ckg_assert(false);
        } break;
    }
}

IonNode* ionInterpretStatement(IonStatement* stmt, Scope* scope) {
    switch (stmt->kind) {
        case ION_NK_ASSIGNMENT_STMT: {
            IonExpression* lhs = ionNodeGetLHS(stmt);

            ckg_assert_msg(
                ionScopeHas(scope, lhs->token.lexeme), 
                "Line: %d | Undeclared Identifier %.*s\n", 
                lhs->token.line,
                lhs->token.lexeme.length, lhs->token.lexeme.data
            );

            IonNode* rhs = ionInterpretExpression(ionNodeGetRHS(stmt), scope);

            ionScopeSet(scope, lhs->token.lexeme, rhs);
        } break;

        case ION_NK_FUNC_CALL_SE: {
            /*
            IonNode* func_decl = ckg_hashmap_get(global_function, stmt->token.lexeme);

            int arg_count = stmt->data.arguments ? ckg_vector_count(stmt->data.arguments) : 0;
            int param_count = func_decl->type.parameters ? ckg_vector_count(func_decl->type.parameters) : 0;

            if (param_count != arg_count) {
                ckg_assert_msg(false, "expected %d parameter(s), got %d\n", arg_count, param_count);
            }

            Scope function_scope = ionScopeCreate(&global_scope);

            for i := 0; i < argCount; i++ {
                param := functionDeclaration.DeclType.Parameters[i]
                arg := v.Arguments[i]
                functionScope.set(param.Tok, interpretExpression(arg, scope))
            }


            IonNode* block = func_decl + 1;
            IonNode* ret = ionInterpretNodes(block, &function_scope);
            ionScopeFree(&function_scope);

            return ret;
            */
        } break;

        case ION_NK_PRINT_STMT: {
            ionPrintExpression(ionNodeGetExpr(stmt), scope);
            if (stmt->data.new_line) {
                CKG_LOG_PRINT("\n");
            }
        } break;
        
        default: {
            ckg_assert(false);
        } break;
    }

    return stmt + 1 + stmt->desc_count;
}

IonNode* ionInterpretDeclaration(IonDeclaration* decl, Scope* scope) {
    switch (decl->kind) {
        case ION_NK_VAR_DECL: {
            IonNode* RHS = ionNodeGetVarDeclRHS(decl);
            ionScopeSet(scope, decl->token.lexeme, RHS);
        } break;

        case ION_NK_FUNC_DECL: {
            ckg_hashmap_put(global_function, decl->token.lexeme, decl);
        } break;
        
        default: {
            ckg_assert(false);
        } break;
    }

   return decl + 1 + decl->desc_count;
}

IonNode* ionInterpretNode(IonNode* node, Scope* scope) {
    if (ionNodeIsDeclaration(node)) {
        return ionInterpretDeclaration(node, scope);
    } else if (ionNodeIsStatement(node)) {
        return ionInterpretStatement(node, scope);
    } else {
        ckg_assert(false);
    }

    return NULLPTR;
}

void ionInterpretProgram(CKG_Vector(IonNode) ast) {
    global_scope = ionScopeCreate(NULLPTR);
	ckg_hashmap_init_string_view_hash(global_function, CKG_StringView, IonNode*);

    IonNode* program = ast;
    while (program->kind != ION_NK_END) {
        program = ionInterpretDeclaration(program, &global_scope);
    }

    if (ckg_hashmap_has(global_function, ckg_sv_create("main", sizeof("main") - 1))) {
        IonDeclaration* mainDecl = ckg_hashmap_get(global_function, ckg_sv_create("main", sizeof("main") - 1));
        IonStatement mainCall = ionNodeCreate(ION_NK_FUNC_CALL_SE, mainDecl->token);

		ionInterpretStatement(&mainCall, &global_scope);
    } else {
        ckg_assert_msg(false, "main function not found\n");
    }
}