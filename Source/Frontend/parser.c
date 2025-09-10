#include <Frontend/parser.h>
#include <Frontend/ast.h>

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

/*
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





def tokens_to_postfix()
    case "var":
        eat("var")
        eat(IDENT)
        eat("=")
        expr_tokens_pf = sy_expr()

        ast.push(  VarDeff( ())


/*

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