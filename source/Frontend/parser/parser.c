#include "parser.h"

#include <string.h>

typedef struct IonParser {
    CKG_Vector(IonToken) tokens;
    int current;
} IonParser;

IonToken ionParserConsumeNextToken(IonParser* parser) {
	return parser->tokens[parser->current++];
}

IonToken ionParserPeekNthToken(IonParser* parser, int n) {
	return parser->tokens[parser->current + n];
}

IonToken ionParserPreviousToken(IonParser* parser) {
	return parser->tokens[parser->current - 1];
}

void ionParserReportError(IonParser* parser, const char* fmt, ...) {
    IonToken token = ionParserPeekNthToken(parser, 0);

    va_list args;
    va_start(args, fmt);
    CKG_LOG_ERROR("String: %s\n", ionTokenKindGetString(token.kind));
    CKG_LOG_ERROR("Error Line: %d | %s\n", token.line, ckg_str_va_sprint(NULLPTR, fmt, args));
    va_end(args);

    ckg_assert(false);
}

IonToken ionParserExpect(IonParser* parser, IonTokenKind expected_kind) {
    IonToken got_token = ionParserPeekNthToken(parser, 0);

    const char* expected_str = ionTokenKindGetString(expected_kind);
    const char* got_str = ionTokenKindGetString(got_token.kind);

    if (got_token.kind != expected_kind) {
        ionParserReportError(parser, "Expected: %s | Got: %s\n", expected_str, got_str);
    }

    return ionParserConsumeNextToken(parser);
}

bool ionParserConsumeOnMatch(IonParser* parser, IonTokenKind expected_kind) {
    if (ionParserPeekNthToken(parser, 0).kind == expected_kind) {
        ionParserConsumeNextToken(parser);
        return true;
    }

    return false;
}


int ionParseExpression(IonParser* parser, IonNode* ast, int index);


// PARSING
// <Primary>    ::= <integer> | <float> | <boolean> | <string> | '(' <Expression> ')'
int ionParsePrimaryExpression(IonParser* parser, IonNode* ast, int index) {
    int start = index;
    IonToken current = ionParserPeekNthToken(parser, 0);

    if (ionParserConsumeOnMatch(parser, ION_TL_INTEGER)) {
        IonNode node = ionNodeCreate(ION_NK_INTEGER_EXPR, current);
        char* buffer = ckg_str_alloc(current.lexeme.data, current.lexeme.length);
        node.data.i = atoi(buffer);
        ast[index] = node;
        ckg_free(buffer);

        return index + 1;
    } else if (ionParserConsumeOnMatch(parser, ION_TL_BOOLEAN)) {
        IonNode node = ionNodeCreate(ION_NK_BOOLEAN_EXPR, current);
        node.data.b = ckg_str_equal(current.lexeme.data, current.lexeme.length,  "true", sizeof("true") - 1);
        ast[index] = node;

        return index + 1;
    } else if (ionParserConsumeOnMatch(parser, ION_TL_FLOAT)) {
        IonNode node = ionNodeCreate(ION_NK_FLOAT_EXPR, current);
        char* buffer = ckg_str_alloc(current.lexeme.data, current.lexeme.length);
        node.data.f = atof(buffer);
        ast[index] = node;
        ckg_free(buffer);

        return index + 1;
    } else if (ionParserConsumeOnMatch(parser, ION_TL_STRING)) {
        IonNode node = ionNodeCreate(ION_NK_STRING_EXPR, current);
        node.data.s = current.lexeme;
        ast[index] = node;

        return index + 1;
    } else if (ionParserConsumeOnMatch(parser, ION_TS_LEFT_PAREN)) {
        IonNode node = ionNodeCreate(ION_NK_GROUPING_EXPR, current);
        ast[start] = node;

        index = ionParseExpression(parser, ast, index + 1); // parse inside parens
        ionParserExpect(parser, ION_TS_RIGHT_PAREN);
        ast[start].data.desc_count = index - (start + 1);

        return index;
    }

    return -1;
}

// <unary> ::= (("-" | "+") <unary>) | <primary>
int ionParseUnaryExpression(IonParser* parser, IonNode* ast, int index) {
    int start = index;
    if (
        ionParserConsumeOnMatch(parser, ION_TS_PLUS) || 
        ionParserConsumeOnMatch(parser, ION_TS_MINUS)
    ) {
        IonToken op = ionParserPreviousToken(parser);
        IonNode node = ionNodeCreate(ION_NK_UNARY_EXPR, op);
        ast[start] = node;

        index = ionParseUnaryExpression(parser, ast, index + 1);
        ast[start].data.desc_count = index - (start + 1);

        return index;
    }

    return ionParsePrimaryExpression(parser, ast, index);
}

// <multiplicative> ::= <unary> (("*" | "/" | "%") <unary>)*
int ionParseMultiplicativeExpression(IonParser* parser, IonNode* ast, int index) {
    int start = index; // index where this additive expression begins
    index = ionParseUnaryExpression(parser, ast, index); // parse left operand

    while (
        ionParserConsumeOnMatch(parser, ION_TS_STAR) || 
        ionParserConsumeOnMatch(parser, ION_TS_DIVISION) ||
        ionParserConsumeOnMatch(parser, ION_TS_MODULUS)
    ) {
        IonToken op = ionParserPreviousToken(parser);

        index = ionParseUnaryExpression(parser, ast, index);

        IonNode node = ionNodeCreate(ION_NK_BINARY_EXPR, op);
        node.data.desc_count = index - start;

        memmove(&ast[start + 1], &ast[start], node.data.desc_count * sizeof(IonNode));
        ast[start] = node;

        index = start + 1 + node.data.desc_count;
    }

    return index;
}

// <additive> ::= <Factor> (('+'|'-') <Factor>)*
int ionParseAdditiveExpression(IonParser* parser, IonNode* ast, int index) {
    int start = index; // index where this additive expression begins
    index = ionParseMultiplicativeExpression(parser, ast, index); // parse left operand

    while (ionParserConsumeOnMatch(parser, ION_TS_PLUS) || ionParserConsumeOnMatch(parser, ION_TS_MINUS)) {
        IonToken op = ionParserPreviousToken(parser);

        index = ionParseMultiplicativeExpression(parser, ast, index);

        IonNode node = ionNodeCreate(ION_NK_BINARY_EXPR, op);
        node.data.desc_count = index - start;

        memmove(&ast[start + 1], &ast[start], node.data.desc_count * sizeof(IonNode));
        ast[start] = node;

        index = start + 1 + node.data.desc_count;
    }

    return index;
}

int ionParseExpression(IonParser* parser, IonNode* ast, int index) {
    return ionParseAdditiveExpression(parser, ast, index);
}

CKG_Vector(IonNode) ionParseProgram(CKG_Vector(IonToken) tokens) {
    IonParser parser;
    parser.current = 0;
    parser.tokens = tokens;

    int ast_upperbound = ckg_vector_count(tokens);
    CKG_Vector(IonNode) ast = ckg_vector_grow(NULLPTR, sizeof(IonNode), ast_upperbound);

    ionParseExpression(&parser, ast, 0);

    return ast;
}

/*
#include <frontend/parser/parser.h>
#include <frontend/ast.h>

static IonToken parser_peek_nth_token(Parser* parser, int n) {
    if (parser->current + n > ckg_vector_count(parser->tokens) - 1) {
        return ION_TOKEN_CREATE_CUSTOM(ION_TOKEN_ILLEGAL_TOKEN, "", -1);
    }

    return parser->tokens[parser->current + n];
}

static IonToken parser_previous_token(Parser* parser) {
    return parser->tokens[parser->current - 1];
}

// TODO(Jovanni): make the error handling better
static void parser_report_error(Parser* parser, char* fmt, ...) {
    IonToken token = parser_peek_nth_token(parser, 0);

    va_list args;
    va_start(args, fmt);
    CKG_LOG_ERROR("String: %s\n", token_strings[token.type]);
    CKG_LOG_ERROR("Error Line: %d | %s", token.line, ckg_str_va_sprint(NULLPTR, fmt, args));
    va_end(args);

    exit(-1);
}

static IonToken parser_consume_next_token(Parser* parser) {
    IonToken ret = parser->tokens[parser->current];
    parser->current += 1;

    return ret;
}

static void parser_expect(Parser* parser, IonTokenType expected_type) {
    if (expected_type && parser_peek_nth_token(parser, 0).type != expected_type) {
        parser_report_error(parser, "Expected: %s | Got: %s\n", token_strings[expected_type], token_strings[parser_peek_nth_token(parser, 0).type]);
    }

    parser_consume_next_token(parser);
}

static bool parser_consume_on_match(Parser* parser, IonTokenType expected_type) {
    if (parser_peek_nth_token(parser, 0).type == expected_type) {
        parser_consume_next_token(parser);
        return true;
    }

    return false;
}

// <primary> ::= INTEGER | FLOAT | TRUE | FALSE | STRING | PRIMITIVE_TYPE | IDENTIFIER | "(" <expression> ")"
Expression* Parser::parse_primary_expression() {
    Token current_token = this->peek_nth_token();

    if (this->consume_on_match(TOKEN_INTEGER_LITERAL)) {
        return Expression::Integer(this->allocator, current_token.i, current_token.line);
    } else if (this->consume_on_match(TOKEN_FLOAT_LITERAL)) {
        return Expression::Float(this->allocator, current_token.f, current_token.line);
    } else if (this->consume_on_match(TOKEN_LEFT_PAREN)) {
        Expression* expression = this->parse_expression();
        expect(TOKEN_RIGHT_PAREN);

        return Expression::Grouping(this->allocator, expression, previous_token().line);
    }

    return nullptr;
}

// <multiplicative> ::= <primary> (("*" | "/" | "%") <primary>)*
Expression* Parser::parse_multiplicative_expression() {

    

    Expression* expression = this->parse_primary_expression();

    while (this->consume_on_match(TOKEN_STAR) || this->consume_on_match(TOKEN_DIVISION) || this->consume_on_match(TOKEN_MODULUS)) {
        Token op = this->previous_token();
        Expression* right = this->parse_primary_expression();

        expression = Expression::Binary(this->allocator, op, expression, right, op.line);
    }

    return expression;
}

// <additive> ::= <multiplicative> (("+" | "-") <multiplicative>)*
Expression* Parser::parse_additive_expression() {
    Expression* expression = this->parse_multiplicative_expression();
    while (this->consume_on_match(TOKEN_PLUS) || this->consume_on_match(TOKEN_MINUS)) {

        Token op = this->previous_token();
        Expression* right = this->parse_multiplicative_expression();

        expression = Expression::Binary(this->allocator, op, expression, right, op.line);
    }

    return expression;
}

Expression* Parser::parse_expression() {
    return this->parse_additive_expression();
}

IonAstNodeH parse_statement(Parser* parser, bool requires_semi_colon) {
    IonTokenType next_token_type = parser_peek_nth_token(parser, 0).type;

    switch (next_token_type) {
        case ION_TOKEN_VAR: {
            return parse_variable_decleration(parser);
        } break;

        case ION_TOKEN_FUNC: {
            return parse_function_decleration(parser);
        } break;

        case ION_TOKEN_IF: {
            return parse_if_statement(parser);
        } break;

        case ION_TOKEN_FOR: {
            return parse_for_statement(parser);
        } break;

        case ION_TOKEN_WHILE: {
            return parse_while_statement(parser);
        } break;

        case ION_TOKEN_IDENTIFIER: {
            if (parser_peek_nth_token(parser, 1).type == ION_TOKEN_ASSIGNMENT) {
                return parse_assignment_statement(parser);
            }
        } break;
    }

    IonAstNodeH ret;
    ret.index = -1;
    parser_report_error(parser, "No statements to interpret; this is not a valid program.\n");
    return ret;
}
*/




/*
def tokens_to_postfix()
    case "var":
        eat("var")
        eat(IDENT)
        eat("=")
        expr_tokens_pf = sy_expr()

        ast.push(  VarDeff( ())


output_pf = []  # SY converts to pf
op_stack = []
state = LUNARY | TERM


def disambiguate(op, state)
    if op == '-':
        if sate & LUNNARY:
            return UNARY_NEG
        else:
            reteurrn BINARY_SUB

while token != eof {
    if is_term(token):  // ident, literals
        output_pf.append((token, metadata... source_line))
    elif is_op(token):
        op = disambiguate(token, state)
        # emit_higher_prec_ops
        prec = op.prec + op.r_assoc
        while (op_stack.len > 0 && (op_stack[-1].prec > prec):
            output_pf.append((op, metadata...))
        op_stack.append(op)
    else:
        complain and ignore


*/