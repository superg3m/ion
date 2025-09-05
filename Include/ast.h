#include "Core/ckg.h"

enum {
    ION_NK_INT_LIT,
    ION_NK_FLOAT_LIT,
    ION_NK_CHAR_LIT,
    ION_NK_STRING_LIT,

    ION_NK_ADD,
    ION_NK_MUL,

    ION_NK_ENUM_COUNT
};
typedef u8 ION_AstNodeKind;
static_assert(ION_NK_ENUM_COUNT < 256, "ION_AstNodeKind must fit in u8");

static b32 ion_ast_nk_is_leaf(ION_AstNodeKind nk) { return nk <= ION_NK_STRING_LIT; }
static b32 ion_ast_nk_is_list(ION_AstNodeKind nk) { return !ion_ast_nk_is_leaf(nk); }

typedef struct ION_AstNode {
    ION_AstNodeKind kind;
    
    CKG_StringView source_view; // @TODO save line_number as well
    union {
        // list nodes have to store how many descendant nodes they contain (excluding the list node itself)
        struct { u32 descendant_count; };
        
        i64 as_i64;
        f64 as_f64;
        u8 as_char;
        // @TODO: figure out how to repr a str lit (source_range enough?)
    };

#if ION_CC_DEBUG
    char* debug_str;
#endif
} ION_AstNode;

typedef struct ION_Ast {
    ION_AstNode* node_arr;
    u32 node_len;
    u32 node_cap;
} ION_Ast;
 
typedef struct { u32 index; } ION_AstNodeH;

static ION_AstNodeH ion_ast_push(ION_Ast* ast, ION_AstNode node)
{
    ION_AstNodeH h = { ast->node_len };
    ckg_assert_msg(h.index < ast->node_cap, "ast boom");
    ast->node_arr[h.index++] = node;
    return h;
}

static ION_AstNode* ion_ast_get(ION_Ast* ast, ION_AstNodeH nodeH)
{
    ION_AstNode* node = &ast->node_arr[nodeH.index];
    return node;
}


// Expression
//  └── Logical (||, &&)
//       └── Comparison (==, !=, <, >, etc.) 4
//            └── Additive (+, -, |, ^) 3
//                 └── Multiplicative (*, /, %, <<, >>) 2
//                      └── Unary (+, -, !, ~, &, *) 1
//                           └── Primary (literals, identifiers, etc.) 0

primary() {
    value = ...
    return ast_push(INT_LIT, value);
}

unary() {
    Node* neg = ast_push(NEG, n_desc=1)
    pri = primary()
    neg.n_desc += IsLeaf(pri) ? 1 : pri.n_desc;
}

// <logical>    ::= <comparison> (("||" | "&&") <comparison>)*
static ION_AstNodeIndex parse_logical_expression(Parser* parser) {
    initial_len = ast.n_len;
    logical_or = push/reserve();
    
    ION_AstNodeIndex expression = parse_comparison_expression(parser);
    while (parser_consume_on_match(parser, ION_TOKEN_OR) || parser_consume_on_match(parser, ION_TOKEN_AND)) {
        ION_Token op = parser_previous_token(parser);
        ION_AstNodeIndex right = parse_comparison_expression(parser);

        expression = logical_expression_create(op, expression, right, op.line);
        return ast_push(INT_LIT, value);
        push_ast()
    }

    logical_or.n_desc = ast.n_len - initial_len;
    return expression;
}

// <expression> ::= <logical_expr>


static Expression* parse_expression(Parser* parser);
// <primary> ::= INTEGER | FLOAT | TRUE | FALSE | STRING | PRIMITIVE_TYPE | IDENTIFIER | "(" <expression> ")"
static Expression* parse_primary_expression(Parser* parser) {
    if (parser_consume_on_match(parser, SPL_TOKEN_INTEGER_LITERAL)) {
        SPL_Token tok = parser_previous_token(parser);
        push_ast()
        return integer_expression_create(parser->tok.i, tok.line);
    }
}

static ION_AstNodeIndex parse_binary_expression(Parser* parser) {
    ION_AstNodeIndex binary_opH = ast_reserve_node(&p->ast);

    // Add<#> Int<1> Int<2> Update<#>
    // Int<1> Int<2> Add<#>

    // BegiBinOp/Add()
        // Primary/1()
        // Primary/2()
    // EndBinOp()

    BinOp
        ->Int<1>
        ->Int<2>

    
    BinOP
    int<1>, 
        
    BinOp Int<1> Int<2>

    +1 + 2

    1 + 2

    -> AstNode[] in postfix
    2nd-pass -> traverse and produce prefix, push in the AST (desired AST layout)

        IntLit<1>
        IntLit<2>
        AddOp<#>

        a = 1
        VarDef a 1
        a 1 VarDef
    




    Expression* expression = parse_primary_expression(parser);
    while (parser_consume_on_match(parser, ION_TOKEN_PLUS)) {
        ION_Token op = parser_previous_token(parser);
        Expression* right = parse_primary_expression(parser);

        expression = binary_expression_create(op, expression, right, op.line);
    } 

    return expression;
}

Expression: 1
// + 2 + 3
static ION_AstNodeIndex parse_expression(Parser* parser) {
    return parse_binary_expression(parser);
}

// Expression
// └── Binary (+)
//     └── Primary (literals, identifiers, grouping)