#include <Core/ckg.h>

typedef enum IonAstNodeKind {
    ION_NK_INT_LIT = 0,
    ION_NK_FLOAT_LIT,
    ION_NK_CHAR_LIT,
    ION_NK_STRING_LIT,

    ION_NK_ADD,
    ION_NK_MUL,

    ION_NK_ENUM_COUNT
} IonAstNodeKind;
// static_assert(ION_NK_ENUM_COUNT < 256, "IonAstNodeKind must fit in u8");

static b32 ion_ast_nk_is_leaf(IonAstNodeKind nk) { return nk <= ION_NK_STRING_LIT; }
static b32 ion_ast_nk_is_list(IonAstNodeKind nk) { return !ion_ast_nk_is_leaf(nk); }

typedef struct IonAstNode {
    IonAstNodeKind kind;
    CKG_StringView source_view;
    int line;
    union {
        struct { u32 descendant_count; };
        i64 as_i64;
        f64 as_f64;
        u8 as_char;
    };

#if ION_CC_DEBUG
    char* debug_str;
#endif
} IonAstNode;

typedef struct IonAst {
    IonAstNode* node_arr;
    u32 node_len;
    u32 node_cap;
} IonAst;
 
typedef struct { u32 index; } IonAstNodeH;

static IonAstNodeH ion_ast_push(IonAst* ast, IonAstNode node) {
    IonAstNodeH h = { ast->node_len };
    ckg_assert_msg(h.index < ast->node_cap, "ast boom");
    ast->node_arr[h.index++] = node;
    return h;
}

static IonAstNode* ion_ast_get(IonAst* ast, IonAstNodeH nodeH) {
    IonAstNode* node = &ast->node_arr[nodeH.index];
    return node;
}

/*

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
static IonAstNodeIndex parse_logical_expression(Parser* parser) {
    initial_len = ast.n_len;
    logical_or = push/reserve();
    
    IonAstNodeIndex expression = parse_comparison_expression(parser);
    while (parser_consume_on_match(parser, ION_TOKEN_OR) || parser_consume_on_match(parser, ION_TOKEN_AND)) {
        IonToken op = parser_previous_token(parser);
        IonAstNodeIndex right = parse_comparison_expression(parser);

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
    if (parser_consume_on_match(parser, ION_TOKEN_INTEGER_LITERAL)) {
        IonToken tok = parser_previous_token(parser);
        push_ast()
        return integer_expression_create(parser->tok.i, tok.line);
    }
}

static IonAstNodeIndex parse_binary_expression(Parser* parser) {
    IonAstNodeIndex binary_opH = ast_reserve_node(&p->ast);

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
        IonToken op = parser_previous_token(parser);
        Expression* right = parse_primary_expression(parser);

        expression = binary_expression_create(op, expression, right, op.line);
    } 

    return expression;
}

Expression: 1
// + 2 + 3
static IonAstNodeIndex parse_expression(Parser* parser) {
    return parse_binary_expression(parser);
}

// Expression
// └── Binary (+)
//     └── Primary (literals, identifiers, grouping)

*/