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

// @TODO: be able to pretty print types! (since we need to change type system, it's a todo for later)
#define ionTypeToStr(...) ""

u32 ionNodePrintSubtree(IonNode* node, char* dest_str, u32 w, u32 depth) {
    #define writefmt(...) __builtin_sprintf(&dest_str[w], __VA_ARGS__)

    u32 write_offset = w;

    // indent if on a new line
    if (w > 0 && dest_str[w-1] == '\n') {
        w += writefmt("%*s", 2*depth, "");
    }

    switch (node->kind) {
    default: { ckg_assert_msg(false, "unhandled node kind: %d", node->kind); } break;
    // leaf nodes (no descendants)
    case ION_NK_INTEGER_EXPR: { ckg_assert_msg(node->desc_count == 0, "integer node must have no descendants (leaf)!");
        w += writefmt("Int<%i>", node->data.i);
    } break;
    case ION_NK_FLOAT_EXPR: { ckg_assert_msg(node->desc_count == 0, "float node must have no descendants (leaf)!");
        w += writefmt("Float<%.4g>", node->data.f);
    } break;
    case ION_NK_BOOLEAN_EXPR: { ckg_assert_msg(node->desc_count == 0, "boolean node must have no descendants (leaf)!");
        w += writefmt("Bool<%s>", node->data.b ? "true" : "false");
    } break;
    case ION_NK_STRING_EXPR: { ckg_assert_msg(node->desc_count == 0, "string node must have no descendants (leaf)!");
        w += writefmt("String<\"%.*s\">", (int)node->data.s.length, node->data.s.data);
    } break;
    case ION_NK_IDENTIFIER_EXPR: { ckg_assert_msg(node->desc_count == 0, "identifier node must have no descendants (leaf)!");
        w += writefmt("Ident<%.*s>", (int)node->token.lexeme.length, node->token.lexeme.data);
    } break;
    // expressions
    case ION_NK_UNARY_EXPR: {
        w += writefmt("Un.%.*s[", (int)node->token.lexeme.length, node->token.lexeme.data);
        IonNode* operand = ionNodeGetUnaryOperand(node);
        ckg_assert_msg((node + 1) + node->desc_count == (operand + 1) + operand->desc_count, "unary node must have exactly one operand as descendant!");

        w += ionNodePrintSubtree(operand, dest_str, w, depth + 1);
        w += (dest_str[w-1] == '\n') ? writefmt("%*s] //Un.%.*s\n", 2*depth, "", (int)node->token.lexeme.length, node->token.lexeme.data) : writefmt("]");
    } break;
    case ION_NK_BINARY_EXPR: {
        w += writefmt("Bin.%.*s[", (int)node->token.lexeme.length, node->token.lexeme.data);
        IonNode* lhs = ionNodeGetLHS(node);
        IonNode* rhs = ionNodeGetRHS(node);
        ckg_assert_msg((node + 1) + node->desc_count == (rhs + 1) + rhs->desc_count, "binary node must have exactly one lhs and rhs as descendants!");

        w += ionNodePrintSubtree(lhs, dest_str, w, depth + 1); bool is_nl = (dest_str[w-1] == '\n'); w -= is_nl; w += (is_nl) ? writefmt(",\n") : writefmt(", ");
        w += ionNodePrintSubtree(rhs, dest_str, w, depth + 1);
        w += (dest_str[w-1] == '\n') ? writefmt("%*s] // Bin.%.*s\n", 2*depth, "", (int)node->token.lexeme.length, node->token.lexeme.data) : writefmt("]");
    } break;
    case ION_NK_GROUPING_EXPR: {
        w += writefmt("Group[");
        IonNode* expr = ionNodeGetExpr(node);
        ckg_assert_msg((node + 1) + node->desc_count == (expr + 1) + expr->desc_count, "group node must have exactly one expr as descendant!");

        w += ionNodePrintSubtree(expr, dest_str, w, depth + 1);
        w += (dest_str[w-1] == '\n') ? writefmt("%*s] // Group\n", 2*depth, "") : writefmt("]");
    } break;
    // statements
    case ION_NK_ASSIGNMENT_STMT: {
        w += writefmt("Assign[ ");
        IonNode* lhs = ionNodeGetLHS(node);
        IonNode* rhs = ionNodeGetRHS(node);
        ckg_assert_msg((node + 1) + node->desc_count == (rhs + 1) + rhs->desc_count, "assign node must have exactly one lhs and rhs as descendants!");

        w += ionNodePrintSubtree(lhs, dest_str, w, depth + 1); bool is_nl = (dest_str[w-1] == '\n'); w -= is_nl; w += (is_nl) ? writefmt(",\n") : writefmt(", ");
        w += ionNodePrintSubtree(rhs, dest_str, w, depth + 1);
        w += (dest_str[w-1] == '\n') ? writefmt("%*s] // Assign\n", 2*depth, "") : writefmt(" ]\n");
    } break;
    case ION_NK_BLOCK_STMT: {
        w += writefmt("Block[\n");
        IonNode* block_end = (node + 1) + node->desc_count;
        for (IonNode* child_node = (node + 1); child_node < block_end; child_node += (1 + child_node->desc_count)) {
            w += ionNodePrintSubtree(child_node, dest_str, w, depth + 1);
            w -= (dest_str[w-1] == '\n'); w += writefmt(",\n");
        }
        w -= (dest_str[w-1] <= ' ' && dest_str[w-2] == ',') ? 2 : 0;
        w += writefmt("\n%*s] // Block\n", 2*depth, "");
    } break;
    case ION_NK_PRINT_STMT: {
        w += writefmt("Print%s[ ", node->data.new_line ? "ln" : "");
        IonNode* expr = ionNodeGetExpr(node);
        ckg_assert_msg((node + 1) + node->desc_count == (expr + 1) + expr->desc_count, "print node must have exactly one expr as descendant!");

        w += ionNodePrintSubtree(expr, dest_str, w, depth + 1);
        w += (dest_str[w-1] == '\n') ? writefmt("%*s] // Print%s\n", 2*depth, "", node->data.new_line ? "ln" : "") : writefmt(" ]\n");
    } break;
    case ION_NK_POST_INCREMENT_SE:
    case ION_NK_PRE_INCREMENT_SE:
    case ION_NK_POST_DECREMENT_SE:
    case ION_NK_PRE_DECREMENT_SE: {
        char* op_prefix_str = (node->kind == ION_NK_POST_INCREMENT_SE || node->kind == ION_NK_POST_DECREMENT_SE) ? "Post" : "Pre";
        char* op_token_str = (node->kind == ION_NK_POST_INCREMENT_SE || node->kind == ION_NK_PRE_INCREMENT_SE) ? "++" : "--";

        w += writefmt("%s.%s[ ", op_prefix_str, op_token_str);
        IonNode* expr = ionNodeGetExpr(node);
        ckg_assert_msg((node + 1) + node->desc_count == (expr + 1) + expr->desc_count, "increment/decrement node must have exactly one expr as descendant!");

        w += ionNodePrintSubtree(expr, dest_str, w, depth + 1);
        w += (dest_str[w-1] == '\n') ? writefmt("%*s] // %s.%s\n", 2*depth, "", op_prefix_str, op_token_str) : writefmt(" ]\n");
    } break;
    case ION_NK_FUNC_CALL_SE: {
        w += writefmt("Call[ ");

        // @FIXME: CallExpr should have arbitrary lvalue not just identifier
        // @TODO: proper LHS printing for callee-expr
        w += writefmt("Func[%.*s], ", (int)node->token.lexeme.length, node->token.lexeme.data);

        // @FIXME: currently call expr stores the args in data.arguments
        // @TODO: arg list should just be descendant nodes of call-expr
        for (u32 i = 0; i < node->desc_count; i++) {
            w += ionNodePrintSubtree(node->data.arguments[i], dest_str, w, depth + 1);
            w -= (dest_str[w-1] == '\n'); w += writefmt(",\n");
        }
        bool is_nl = (dest_str[w-1] == '\n');
        w -= (dest_str[w-1] <= ' ' && dest_str[w-2] == ',') ? 2 : 0;
        w += (is_nl) ? writefmt("\n%*s] // Call\n", 2*depth, "") : writefmt(" ]\n");
    } break;
    // declarations
    case ION_NK_VAR_DECL: {
        w += writefmt("VarDecl[ ");

        // @FIXME: var name and type should not be special cased and should be descendant nodes of var-decl (not embedded in node!)
        w += writefmt("Name<%.*s>, ", (int)node->token.lexeme.length, node->token.lexeme.data);
        w += writefmt("VarType[%s], ", ionTypeToStr(node->type));
        
        for (IonNode* child_node = (node + 1); child_node < (node + 1) + node->desc_count; child_node += (1 + child_node->desc_count)) {
            w += ionNodePrintSubtree(child_node, dest_str, w, depth + 1);
            bool is_nl = (dest_str[w-1] == '\n'); w -= is_nl; w += (is_nl) ? writefmt(",\n") : writefmt(", ");
        }
        bool is_nl = (dest_str[w-1] == '\n');
        w -= (dest_str[w-1] <= ' ' && dest_str[w-2] == ',') ? 2 : 0;
        w += (is_nl) ? writefmt("\n%*s] // VarDecl\n", 2*depth, "") : writefmt(" ]\n");
    } break;
    case ION_NK_FUNC_DECL: {
        w += writefmt("FuncDecl[\n");
        
        w += writefmt("%*sName<%.*s>,\n", 2*(depth+1), "", (int)node->token.lexeme.length, node->token.lexeme.data);
        // @FIXME: param list should not be special cased and should be descendant nodes of func-decl (not embedded in node!)
        // @TODO: param list printing
        w += writefmt("%*sFuncProtoType[%s]\n", 2*(depth+1), "", ionTypeToStr(node->type));

        // body
        for (IonNode* child_node = (node + 1); child_node < (node + 1) + node->desc_count; child_node += (1 + child_node->desc_count)) {
            w += ionNodePrintSubtree(child_node, dest_str, w, depth + 1);
            w -= (dest_str[w-1] == '\n'); w += writefmt(",\n");
        }
        w -= (dest_str[w-1] <= ' ' && dest_str[w-2] == ',') ? 2 : 0;
        w += writefmt("\n%*s] // FuncDecl\n", 2*depth, "");
    } break;
    }
    u32 n_bytes_written = w - write_offset;
    
    #undef writefmt
    return n_bytes_written;
}

void ionAstPrint(IonNode* ast_root) {
    char subtree_str_buffer[64*1024];

    for (IonNode* child_node = ast_root; child_node->kind != ION_NK_END; child_node += (1 + child_node->desc_count)) {
        u32 subtree_str_len = ionNodePrintSubtree(child_node, subtree_str_buffer, 0, 0);
        // ensure toplevel (depth = 1 from ast root) end up on a new line
        if (subtree_str_buffer[subtree_str_len-1] != '\n') {
            subtree_str_buffer[subtree_str_len++] = '\n';
        }

        CKG_LOG_PRINT("%.*s", subtree_str_len, subtree_str_buffer);
    }
}
