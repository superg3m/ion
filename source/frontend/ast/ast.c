#include "ast.h"
#include <cj.h>

IonNode ionNodeCreate(IonNodeKind kind, IonToken token) {
    IonNode ret;
    ret.kind = kind;
    ret.token = token;
    ret.desc_count = 0;
    ckg_memory_zero(&ret.data, sizeof(ret.data));
   
    return ret;
}

bool ionNodeIsLeaf(IonNode* node) {
    return node->kind & ION_CLASS_LEAF_NODE;
}

bool ionNodeIsExpression(IonNode* node) {
    return node->kind & ION_CLASS_EXPRESSION;
}

bool ionNodeIsStatement(IonNode* node) {
    return node->kind & ION_CLASS_STATEMENT;
}

bool ionNodeIsDeclaration(IonNode* node) {
    return node->kind & ION_CLASS_DECLARATION;
}

const char* ionNodeKindToString(IonNodeKind kind) {
    static CKG_HashMap(IonNodeKind, const char*)* kind_map = NULLPTR;

    if (kind_map == NULLPTR) {
        ckg_hashmap_init_siphash(kind_map, IonNodeKind, const char*);

        #define X(kind) ckg_hashmap_put(kind_map, kind, stringify(kind));
            X_NK_LEAF_NODES
        #undef X

        #define X(kind) ckg_hashmap_put(kind_map, kind, stringify(kind));
            X_NK_EXPRESSION
        #undef X

        #define X(kind) ckg_hashmap_put(kind_map, kind, stringify(kind));
            X_NK_STATEMENT
        #undef X

        #define X(kind) ckg_hashmap_put(kind_map, kind, stringify(kind));
            X_NK_DEFFERABLE_STATEMENTS
        #undef X

        #define X(kind) ckg_hashmap_put(kind_map, kind, stringify(kind));
            X_NK_SE
        #undef X

        #define X(kind) ckg_hashmap_put(kind_map, kind, stringify(kind));
            X_NK_DEFFERABLE_SES
        #undef X

        #define X(kind) ckg_hashmap_put(kind_map, kind, stringify(kind));
            X_NK_DECLARATIONS
        #undef X
    }

    return ckg_hashmap_get(kind_map, kind);
}

IonNode* ionNodeGetLHS(IonNode* node) {
    ckg_assert(node->kind == ION_NK_ASSIGNMENT_STMT || node->kind == ION_NK_BINARY_EXPR);

    return node + 1;
}

IonNode* ionNodeGetRHS(IonNode* node) {
    ckg_assert(node->kind == ION_NK_ASSIGNMENT_STMT || node->kind == ION_NK_BINARY_EXPR);

    IonNode* left = ionNodeGetLHS(node);

    return left + 1 + left->desc_count;
}

IonNode* ionNodeGetExpr(IonNode* node) {
    ckg_assert(
        node->kind == ION_NK_GROUPING_EXPR ||
        node->kind == ION_NK_PRINT_STMT ||
        node->kind == ION_NK_POST_INCREMENT_SE ||
        node->kind == ION_NK_PRE_INCREMENT_SE ||
        node->kind == ION_NK_POST_DECREMENT_SE ||
        node->kind == ION_NK_PRE_DECREMENT_SE
    );

    return node + 1;
}

IonNode* ionNodeGetUnaryOperand(IonNode* node) {
    ckg_assert(node->kind == ION_NK_UNARY_EXPR);

    return node + 1;
}

IonNode* ionNodeGetIndex(IonNode* node, int index) {
    ckg_assert(
        index >= 0 &&
        (node->kind == ION_NK_LIST || node->kind ==ION_NK_BLOCK_STMT)
    );

    IonNode* ret = node + 1;
    while (index != 0) {
        ret += (1 + ret->desc_count);
        index -= 1;
    }

    return ret;
}

IonNode* ionNodeGetFuncDeclParams(IonNode* node) {
    ckg_assert(node->kind == ION_NK_FUNC_DECL);

    return node + 1;
}
IonNode* ionNodeGetFuncDeclReturnType(IonNode* node) {
    IonNode* params = ionNodeGetFuncDeclParams(node);

    return params + (1 + params->desc_count);
}
IonNode* ionNodeGetFuncDeclBlock(IonNode* node){
    IonNode* return_type = ionNodeGetFuncDeclReturnType(node);

    return return_type + (1 + return_type->desc_count);
}

IonNode* ionNodeGetVarDeclType(IonNode* node) {
    ckg_assert(node->kind == ION_NK_VAR_DECL);

    return node + 1;
}
IonNode* ionNodeGetVarDeclRHS(IonNode* node) {
    IonNode* decl_type = ionNodeGetVarDeclType(node);

    return decl_type + (1 + decl_type->desc_count);
}

static JSON* ionAstToJsonHelper(IonNode* node, CJ_Arena* arena) {
    #define TO_CJ_SV(sv) (CJ_StringView){sv.data, 0, sv.length}

    if (ionNodeIsExpression(node)) {
        switch (node->kind) {
            case ION_NK_STRING_EXPR: {
                return JSON_STRING_VIEW(arena, ((CJ_StringView){node->token.lexeme.data, 1, node->token.lexeme.length - 2}));
            } break;

            case ION_NK_INTEGER_EXPR: {
                return JSON_INT(arena, node->data.i);
            } break;

            case ION_NK_FLOAT_EXPR: {
                return JSON_FLOAT(arena, node->data.f);
            } break;

            case ION_NK_BOOLEAN_EXPR: {
                return JSON_BOOL(arena, node->data.b);
            } break;

            case ION_NK_IDENTIFIER_EXPR: {
                return JSON_STRING_VIEW(arena, TO_CJ_SV(node->token.lexeme));
            } break;

            case ION_NK_UNARY_EXPR: {
                JSON* unary_root = cj_create(arena);
                
                JSON* desc = cj_create(arena);
                cj_push(desc, "op", TO_CJ_SV(node->token.lexeme));
                cj_push(desc, "operand", ionAstToJsonHelper(ionNodeGetUnaryOperand(node), arena));

                cj_push(unary_root, "UnaryOp", desc);

                return unary_root;
            } break;

            case ION_NK_BINARY_EXPR: {
                JSON* binary_root = cj_create(arena);

                JSON* desc = cj_create(arena);
                cj_push(desc, "op", TO_CJ_SV(node->token.lexeme));
                cj_push(desc, "left", ionAstToJsonHelper(ionNodeGetLHS(node), arena));
                cj_push(desc, "right", ionAstToJsonHelper(ionNodeGetRHS(node), arena));

                cj_push(binary_root, "BinaryOp", desc);

                return binary_root;
            } break;

            case ION_NK_GROUPING_EXPR: {
                JSON* grouping_root = cj_create(arena);
                cj_push(grouping_root, "Grouping", ionAstToJsonHelper(ionNodeGetExpr(node), arena));

                return grouping_root;
            } break;

            default: {
                ckg_assert_msg(false, "Expression kind: %s not handled!\n", ionNodeKindToString(node->kind));
            } break;
        } 
    } else if (ionNodeIsStatement(node)) {
        switch (node->kind) {
            case ION_NK_PRINT_STMT: {
                JSON* print_root = cj_create(arena);
                cj_push(print_root, "PrintStatement", ionAstToJsonHelper(ionNodeGetExpr(node), arena));

                return print_root;
            } break;

            case ION_NK_ASSIGNMENT_STMT: {
                JSON* assignment_root = cj_create(arena);

                JSON* desc = cj_create(arena);
                cj_push(desc, "left", ionAstToJsonHelper(ionNodeGetLHS(node), arena));
                cj_push(desc, "right", ionAstToJsonHelper(ionNodeGetRHS(node), arena));

                cj_push(assignment_root, "AssignmentStatement", desc);

                return assignment_root;
            } break;

            default: {
                ckg_assert_msg(false, "Expression kind: %s not handled!\n", ionNodeKindToString(node->kind));
            } break;
        }
    } else if (ionNodeIsDeclaration(node)) {
        switch (node->kind) {
            case ION_NK_FUNC_DECL: {
                JSON* func_decl_root = cj_create(arena);

                // IonNode* params = ionNodeGetFuncDeclParams(node);
                // IonNode* return_type = ionNodeGetFuncDeclReturnType(node);

                IonNode* block = ionNodeGetFuncDeclBlock(node);
                JSON* block_array = cj_array_create(arena);
                for (int i = 0; i < block->data.list_count; i++) {
                    cj_array_push(block_array, ionAstToJsonHelper(ionNodeGetIndex(block, i), arena));
                }

                JSON* desc = cj_create(arena);
                cj_push(desc, "params", "NOT IMPLEMENTED YET!");
                cj_push(desc, "return_type", "NOT IMPLEMENTED YET!");
                cj_push(desc, "block", block_array);
                cj_push(func_decl_root, ckg_str_sprint(NULLPTR, "FuncDecl<%.*s>", node->token.lexeme.length, node->token.lexeme.data), desc);

                return func_decl_root;
            } break;

            case ION_NK_VAR_DECL: {
                JSON* var_decl_root = cj_create(arena);

                // IonNode* decl_type = ionNodeGetVarDeclType(node);

                JSON* desc = cj_create(arena);
                cj_push(desc, "decl_type", "NOT IMPLEMENTED YET!");
                cj_push(desc, "rhs", ionAstToJsonHelper(ionNodeGetVarDeclRHS(node), arena));
                cj_push(var_decl_root, ckg_str_sprint(NULLPTR, "VarDecl<%.*s>", node->token.lexeme.length, node->token.lexeme.data), desc);

                return var_decl_root;
            } break;

            default: {
                ckg_assert_msg(false, "Expression kind: %s not handled!\n", ionNodeKindToString(node->kind));
            } break;
        }
    } else {
        ckg_assert(false);
    }

    return NULL;
}

static JSON* ionProgramToJson(CKG_Vector(IonNode) ast, CJ_Arena* arena) {
    JSON* json_root = cj_create(arena);
    JSON* json_declerations = cj_array_create(arena);
    cj_push(json_root, "Declerations", json_declerations);

    for (IonNode* decl = ast; decl->kind != ION_NK_END; decl += (1 + decl->desc_count)) {
        cj_array_push(json_declerations, ionAstToJsonHelper(decl, arena));
    }

    return json_root;
}

void ionAstPrettyPrint(CKG_Vector(IonNode) ast) {
    char* indent = "    ";
    cj_set_context_indent(indent);
    CJ_Arena* arena = cj_arena_create(0);

    JSON* json = ionProgramToJson(ast, arena);
    ckg_assert_msg(json, "Failed to jsonify ast\n");

    char* str = cj_to_string(json);
    printf("%s\n", str);

    cj_arena_free(arena);
}