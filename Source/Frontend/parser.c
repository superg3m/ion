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
