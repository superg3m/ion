#include <ckg.h>
#include "../ast/ast.h"

typedef struct Scope {
	struct Scope* parent   ;
	CKG_HashMap(CKG_StringView, IonNode)* variables;
} Scope;

Scope ionScopeCreate(Scope* parent) {
    Scope ret;
    ret.parent = parent;
    ckg_hashmap_init_string_view_hash(ret.variables, CKG_StringView, IonNode);

    return ret;
}

bool ionScopeHas(Scope* self, CKG_StringView key) {
    Scope* current = self;
    while (current != NULLPTR) {
        if (ckg_hashmap_has(current->variables, key)) {
            return true;
        }

        current = current->parent;
    }

    return false;
}

IonNode ionScopeGet(Scope* self, CKG_StringView key) {
    Scope* current = self;
    while (current != NULLPTR) {
        if (ckg_hashmap_has(current->variables, key)) {
            return ckg_hashmap_get(current->variables, key);
        }

        current = current->parent;
    }

    ckg_assert(false);
    return (IonNode){};
}

void ionScopeSet(Scope* self, CKG_StringView key, IonNode value) {
    Scope* current = self;
    while (current != NULLPTR) {
        if (ckg_hashmap_has(current->variables, key)) {
            ckg_hashmap_put(current->variables, key, value);
            return;
        }

        current = current->parent;
    }

    ckg_hashmap_put(self->variables, key, value);
}

Scope global_scope;
CKG_HashMap(CKG_StringView, IonNode*)* global_function = NULLPTR;

void ionInterpretExpression(IonNode* expr, Scope* scope) {
    switch (expr->kind) {
        case ION_NK_VAR_DECL: {

        } break;

        case ION_NK_FUNC_DECL: {
            
        } break;
        
        default: {
            ckg_assert(false);
        } break;
    }
}

void printExpression(IonNode* expr, Scope* scope) {
    ckg_assert(ionNodeIsExpression(expr));

    switch (expr->kind) {
        case ION_NK_INTEGER_EXPR: {
            CKG_LOG_PRINT("%d", expr->data.i);
        } break;

        case ION_NK_FLOAT_EXPR: {
            CKG_LOG_PRINT("%f", expr->data.f);
        } break;

        case ION_NK_BOOLEAN_EXPR: {
            CKG_LOG_PRINT("%s", expr->data.b ? "true" : "else");
        } break;

        case ION_NK_STRING_EXPR: {
            // fixNewLineCode()
            // CKG_LOG_PRINT("%.*s", expr->data.s);
        } break;

        default: {
            ckg_assert(false);
        } break;
    }
}

void ionInterpretStatement(IonNode* stmt, Scope* scope) {
    switch (stmt->kind) {
        case ION_NK_ASSIGNMENT_STMT: {

        } break;

        case ION_NK_FUNC_CALL_SE: {
            IonNode* func_decl = ckg_hashmap_get(global_function, stmt->token.lexeme);

            int arg_count = ckg_vector_count(stmt->data.arguments);
            int param_count = ckg_vector_count(func_decl->type.parameters);

            if (param_count != arg_count) {
                // panic(fmt.Sprintf("expected %d parameter(s), got %d", argCount, paramCount))
            }

            /*
            functionScope := CreateScope(&globalScope)
            for i := 0; i < argCount; i++ {
                param := functionDeclaration.DeclType.Parameters[i]
                arg := v.Arguments[i]
                functionScope.set(param.Tok, interpretExpression(arg, scope))
            }

            return interpretExpression(interpretNodes(functionDeclaration.Block.Body, &functionScope), &functionScope)
            */
        } break;

        case ION_NK_PRINT_STMT: {
            printExpression(ionNodeGetExpr(stmt), scope);
            CKG_LOG_PRINT("\n");
        } break;
        
        default: {
            ckg_assert(false);
        } break;
    }
}

IonNode* ionInterpretDeclaration(IonNode* decl, Scope* scope) {
    switch (decl->kind) {
        case ION_NK_VAR_DECL: {

        } break;

        case ION_NK_FUNC_DECL: {

        } break;
        
        default: {
            ckg_assert(false);
        } break;
    }

   return decl + 1 + decl->desc_count;
}

void interpretNode(IonNode* node, Scope* scope) {
    if (ionNodeIsDeclaration(node)) {
        ionInterpretDeclaration(node, scope);
    } else if (ionNodeIsStatement(node)) {
        ionInterpretStatement(node, scope);
    } else {
        ckg_assert(false);
    }
}

void ionInterpretProgram(CKG_Vector(IonNode) ast) {
    global_scope = ionScopeCreate(NULLPTR);
	ckg_hashmap_init_string_view_hash(global_function, CKG_StringView, IonNode*);

    IonNode* program = ast;
    while (program->kind != ION_NK_END) {
        program = ionInterpretDeclaration(program, &global_scope);
    }

    if (ckg_hashmap_has(global_function, ckg_sv_create("main", sizeof("main") - 1))) {
        IonNode* mainDecl = ckg_hashmap_get(global_function, ckg_sv_create("main", sizeof("main") - 1));
        IonNode mainCall = ionNodeCreate(ION_NK_FUNC_CALL_SE, mainDecl->token);

		ionInterpretStatement(&mainCall, &global_scope);
    } else {
        ckg_assert_msg(false, "main function not found\n");
    }

    /*
	if mainDecl, ok := globalFunctions["main"]; ok {

	} else {
		panic()
	}
    */
}