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

#define EXPRESSION_FOLDER
#if defined(EXPRESSION_FOLDER)
    int ionParsePrimaryExpression(IonParser* parser, IonNode* ast, int index);
    int ionParseUnaryExpression(IonParser* parser, IonNode* ast, int index);
    int ionParseMultiplicativeExpression(IonParser* parser, IonNode* ast, int index) ;
    int ionParseAdditiveExpression(IonParser* parser, IonNode* ast, int index);
    int ionParseComparisonExpression(IonParser* parser, IonNode* ast, int index);
    int ionParseLogicalExpression(IonParser* parser, IonNode* ast, int index);
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
            ast[start].desc_count = index - (start + 1);

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
            ast[start].desc_count = index - (start + 1);

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
            index = ionParseMultiplicativeExpression(parser, ast, index);
            IonNode node = ionNodeCreate(ION_NK_BINARY_EXPR, op);
            node.desc_count = index - start;

            memmove(&ast[start + 1], &ast[start], node.desc_count * sizeof(IonNode));
            ast[start] = node;

            index = start + 1 + node.desc_count;
        }

        return index;
    }

    // <additive> ::= <Factor> (('+'|'-') <Factor>)*
    int ionParseAdditiveExpression(IonParser* parser, IonNode* ast, int index) {
        int start = index;
        index = ionParseMultiplicativeExpression(parser, ast, index);

        while (ionParserConsumeOnMatch(parser, ION_TS_PLUS) || ionParserConsumeOnMatch(parser, ION_TS_MINUS)) {
            IonToken op = ionParserPreviousToken(parser);
            index = ionParseMultiplicativeExpression(parser, ast, index);
            IonNode node = ionNodeCreate(ION_NK_BINARY_EXPR, op);
            node.desc_count = index - start;

            memmove(&ast[start + 1], &ast[start], node.desc_count * sizeof(IonNode));
            ast[start] = node;

            index = start + 1 + node.desc_count;
        }

        return index;
    }

    // <comparison> ::= <additive> (('=='|'!='|<'|'<='|'>='|'>'} <additive>)*
    int ionParseComparisonExpression(IonParser* parser, IonNode* ast, int index) {
        int start = index;
        index = ionParseAdditiveExpression(parser, ast, index);

        while (
            ionParserConsumeOnMatch(parser, ION_TS_EQUALS_EQUALS) ||
            ionParserConsumeOnMatch(parser, ION_TS_NOT_EQUALS) ||
            ionParserConsumeOnMatch(parser, ION_TS_LT) ||
            ionParserConsumeOnMatch(parser, ION_TS_LT_OR_EQUAL) ||
            ionParserConsumeOnMatch(parser, ION_TS_GT_OR_EQUAL) ||
            ionParserConsumeOnMatch(parser, ION_TS_GT)
        ) {
            IonToken op = ionParserPreviousToken(parser);
            index = ionParseAdditiveExpression(parser, ast, index);
            IonNode node = ionNodeCreate(ION_NK_BINARY_EXPR, op);
            node.desc_count = index - start;

            memmove(&ast[start + 1], &ast[start], node.desc_count * sizeof(IonNode));
            ast[start] = node;

            index = start + 1 + node.desc_count;
        }

        return index;
    }

    // <logical> ::= <comparison> (('&&'|'||') <comparison>)*
    int ionParseLogicalExpression(IonParser* parser, IonNode* ast, int index) {
        int start = index;
        index = ionParseComparisonExpression(parser, ast, index);

        while (ionParserConsumeOnMatch(parser, ION_TS_LOGICAL_AND) || ionParserConsumeOnMatch(parser, ION_TS_LOGICAL_OR)) {
            IonToken op = ionParserPreviousToken(parser);
            index = ionParseComparisonExpression(parser, ast, index);
            IonNode node = ionNodeCreate(ION_NK_BINARY_EXPR, op);
            node.desc_count = index - start;

            memmove(&ast[start + 1], &ast[start], node.desc_count * sizeof(IonNode));
            ast[start] = node;

            index = start + 1 + node.desc_count;
        }

        return index;
    }

    int ionParseExpression(IonParser* parser, IonNode* ast, int index) {
        return ionParseLogicalExpression(parser, ast, index);
    }
#endif


#define STATEMENT_FOLDER
#if defined(STATEMENT_FOLDER)
    int ionParseStatement(IonParser* parser, IonNode* ast, int index);


    int ionParseStatement(IonParser* parser, IonNode* ast, int index) {
        IonToken current = ionParserPeekNthToken(parser, 0);

        if (current.kind == ION_TOKEN_IDENTIFIER) {
            return ionParseAssignmentStatement(parser, ast, index);
        }
    
        ckg_assert(false);
        return -1;
    }
#endif

#define DECLARATION_FOLDER
#if defined(DECLARATION_FOLDER)
    int ionParseDeclaration(IonParser* parser, IonNode* ast, int index);

    int ionParseVarDecl(IonParser* parser, IonNode* ast, int index) {
        int start = index;

        ionParserExpect(parser, ION_TKW_VAR);
        IonToken ident = ionParserExpect(parser, ION_TOKEN_IDENTIFIER);
        ionParserExpect(parser, ION_TS_COLON);
        // var dataType *TS.Type
        if (!ionParserPeekNthToken(parser, 0).kind == ION_TS_EQUALS) {
            // dataType = parser.parseType();
        }

        IonNode node = ionNodeCreate(ION_TKW_VAR, ident);
        ast[start] = node;

        ionParserExpect(parser, ION_TS_EQUALS);
        index = ionParseExpression(parser, ast, index + 1);
        ast[start].desc_count = index - (start + 1);
        ionParserExpect(parser, ION_TS_SEMI_COLON);

        return index;
    }

    int ionParseDeclaration(IonParser* parser, IonNode* ast, int index) {
        IonToken current = ionParserPeekNthToken(parser, 0);

        if (current.kind == ION_TKW_VAR) {
            return ionParseVarDecl(parser, ast, index);
        } else if (current.kind == ION_TKW_FUNC) {
            return ionParseFuncDecl(parser, ast, index);
        }
    
        ckg_assert(false);
        return -1;
    }
#endif



CKG_Vector(IonNode) ionParseProgram(CKG_Vector(IonToken) tokens) {
    IonParser parser;
    parser.current = 0;
    parser.tokens = tokens;

    int ast_upperbound = ckg_vector_count(tokens);
    CKG_Vector(IonNode) ast = ckg_vector_grow(NULLPTR, sizeof(IonNode), ast_upperbound);

    ionParseExpression(&parser, ast, 0);

    return ast;
}