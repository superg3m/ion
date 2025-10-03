#include "parser.h"
#include "../ts/type_system.h"

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

int ionParseType(IonParser* parser, CKG_Vector(IonNode) ast, int index) {
    int start = index;
    ast[index++] = ionNodeCreate(ION_NK_TYPE_EXPR, ionTokenCreateFake());

    int count = 0;
	while (ionParserPeekNthToken(parser, 0).kind == ION_TS_LEFT_BRACKET) {
		IonToken bracket = ionParserExpect(parser, ION_TS_LEFT_BRACKET);
		ionParserExpect(parser, ION_TS_RIGHT_BRACKET);

        ast[index++] = ionNodeCreate(ION_NK_TYPE_MODIFIER, bracket);
        count += 1;
	}

	IonToken ident = ionParserExpect(parser, ION_TOKEN_IDENTIFIER);
    ast[index++] = ionNodeCreate(ION_NK_TYPE_IDENT, ident);
    count += 1;

    ast[start].data.list_count = count;
    ast[start].desc_count = index - (start + 1);

	return index;
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


    int ionParseArguments(IonParser* parser, IonNode* ast, int index) {
        int start = index;

        ast[index++] = ionNodeCreate(ION_NK_LIST, ionTokenCreateFake());

        int arguments = 0;
        ionParserExpect(parser, ION_TS_LEFT_PAREN);
        while (!ionParserConsumeOnMatch(parser, ION_TS_RIGHT_PAREN)) {
            index = ionParseExpression(parser, ast, index);

            if (ionParserPeekNthToken(parser, 0).kind != ION_TS_RIGHT_PAREN) {
                ionParserExpect(parser, ION_TS_COMMA);
            }

            arguments += 1;
        }

        ast[start].desc_count = index - (start + 1);
        ast[start].data.list_count = arguments;

        return index;
    }


    // PARSING
    // <Primary>    ::= <integer> | <float> | <boolean> | <string> | '(' <Expression> ')'
    int ionParsePrimaryExpression(IonParser* parser, IonNode* ast, int index) {
        int start = index;
        IonToken current = ionParserPeekNthToken(parser, 0);

        if (ionParserConsumeOnMatch(parser, ION_TL_INTEGER)) {
            IonNode node = ionNodeCreate(ION_NK_INTEGER_EXPR, current);
            char* buffer = ckg_str_alloc(current.lexeme.data, current.lexeme.length);
            node.data.i = atoi(buffer);
            ast[index++] = node;
            ckg_free(buffer);

            return index;
        } else if (ionParserConsumeOnMatch(parser, ION_TL_BOOLEAN)) {
            IonNode node = ionNodeCreate(ION_NK_BOOLEAN_EXPR, current);
            node.data.b = ckg_str_equal(current.lexeme.data, current.lexeme.length,  "true", sizeof("true") - 1);
            ast[index++] = node;

            return index;
        } else if (ionParserConsumeOnMatch(parser, ION_TL_FLOAT)) {
            IonNode node = ionNodeCreate(ION_NK_FLOAT_EXPR, current);
            char* buffer = ckg_str_alloc(current.lexeme.data, current.lexeme.length);
            node.data.f = atof(buffer);
            ast[index++] = node;
            ckg_free(buffer);

            return index;
        } else if (ionParserConsumeOnMatch(parser, ION_TL_STRING)) {
            IonNode node = ionNodeCreate(ION_NK_STRING_EXPR, current);
            node.data.s = current.lexeme;
            ast[index++] = node;

            return index;
        } else if (ionParserConsumeOnMatch(parser, ION_TS_LEFT_PAREN)) {
            IonNode node = ionNodeCreate(ION_NK_GROUPING_EXPR, current);
            ast[index++] = node;

            index = ionParseExpression(parser, ast, index); // parse inside parens
            ionParserExpect(parser, ION_TS_RIGHT_PAREN);
            ast[start].desc_count = index - (start + 1);

            return index;
        } else if (ionParserConsumeOnMatch(parser, ION_TOKEN_IDENTIFIER)) {
            IonNode node = ionNodeCreate(ION_NK_IDENTIFIER_EXPR, current);
            ast[index++] = node;

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
            ast[index++] = node;

            index = ionParseUnaryExpression(parser, ast, index);
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
    int ionParseDeclaration(IonParser* parser, IonNode* ast, int index);
    int ionParseStatement(IonParser* parser, IonNode* ast, int index);

    int ionParseAssignmentStatement(IonParser* parser, IonNode* ast, int index) {
        int start = index;

        IonNode ident_node = ionNodeCreate(ION_NK_ASSIGNMENT_STMT, ionTokenCreateFake());
        ast[index++] = ident_node;

        index = ionParseExpression(parser, ast, index);
        IonToken equals = ionParserExpect(parser, ION_TS_EQUALS);

        index = ionParseExpression(parser, ast, index);

        ast[start].token = equals;
        ast[start].desc_count = index - (start + 1);

        ionParserExpect(parser, ION_TS_SEMI_COLON);

        return index;
    }

    int ionParseStatementBlock(IonParser* parser, IonNode* ast, int index) {
        int start = index;

        IonToken opening = ionParserExpect(parser, ION_TS_LEFT_CURLY);
        ast[index++] = ionNodeCreate(ION_NK_BLOCK_STMT, opening);
        
        int count = 0;
        while (!ionParserConsumeOnMatch(parser, ION_TS_RIGHT_CURLY)) {
            int possible = ionParseDeclaration(parser, ast, index);
            if (possible != -1) {
                index = possible;
                count += 1;
                continue;
            }

            possible = ionParseStatement(parser, ast, index);
            if (possible != -1) {
                index = possible;
                count += 1;
                continue;
            }

            ckg_assert(false);
        }

        ast[start].desc_count = index - (start + 1);
        ast[start].data.list_count = count;

        return index;
    }

    int ionParseStatement(IonParser* parser, IonNode* ast, int index) {
        int start = index;

        IonToken current = ionParserPeekNthToken(parser, 0);
        if (current.kind == ION_TS_LEFT_CURLY) {
		    return ionParseStatementBlock(parser, ast, index);
        } else if (current.kind == ION_TKW_PRINT || current.kind == ION_TKW_PRINTLN) {
            ionParserExpect(parser, current.kind);
            ionParserExpect(parser, ION_TS_LEFT_PAREN);

            IonNode node = ionNodeCreate(ION_NK_PRINT_STMT, current);
            node.data.new_line = current.kind == ION_TKW_PRINTLN;
            ast[index++] = node;

            index = ionParseExpression(parser, ast, index);
            ast[start].desc_count = index - (start + 1);
            ionParserExpect(parser, ION_TS_RIGHT_PAREN);
            ionParserExpect(parser, ION_TS_SEMI_COLON);

		    return index;
        } else if (current.kind == ION_TOKEN_IDENTIFIER) {
            IonToken next = ionParserPeekNthToken(parser, 1);
            if (next.kind == ION_TS_LEFT_PAREN) {
                IonToken ident = ionParserExpect(parser, ION_TOKEN_IDENTIFIER);

                IonNode node = ionNodeCreate(ION_NK_FUNC_CALL_SE, ident);
                ast[index++] = node;

                index = ionParseArguments(parser, ast, index);
                ast[start].desc_count = index - (start + 1);

                ionParserExpect(parser, ION_TS_SEMI_COLON);

                return index;
            }

            return ionParseAssignmentStatement(parser, ast, index);
        }
    
        return -1;
    }
#endif

#define DECLARATION_FOLDER
#if defined(DECLARATION_FOLDER)
    int ionParseVarDecl(IonParser* parser, IonNode* ast, int index) {
        int start = index;

        ionParserExpect(parser, ION_TKW_VAR);
        IonToken ident = ionParserExpect(parser, ION_TOKEN_IDENTIFIER);
        ast[index++] = ionNodeCreate(ION_NK_VAR_DECL, ident);

        ionParserExpect(parser, ION_TS_COLON);
        if (ionParserPeekNthToken(parser, 0).kind != ION_TS_EQUALS) {
            index = ionParseType(parser, ast, index);
        }

        ionParserExpect(parser, ION_TS_EQUALS);
        index = ionParseExpression(parser, ast, index);
        ast[start].desc_count = index - (start + 1);

        ionParserExpect(parser, ION_TS_SEMI_COLON);

        return index;
    }

    int ionParseParam(IonParser* parser, CKG_Vector(IonNode) ast, int index) {
        int start = index;

        ast[index++] = ionNodeCreate(ION_NK_PARAM, ionTokenCreateFake());

        IonToken token = ionParserExpect(parser, ION_TOKEN_IDENTIFIER);
        ast[index++] = ionNodeCreate(ION_NK_IDENTIFIER_EXPR, token);
        ionParserExpect(parser, ION_TS_COLON);
        index = ionParseType(parser, ast, index);

        if (ionParserPeekNthToken(parser, 0).kind != ION_TS_RIGHT_PAREN) {
            ionParserExpect(parser, ION_TS_COMMA);
        }

        ast[start].desc_count = index - (start + 1);

        return index;
    }

    int ionParseParameterList(IonParser* parser, CKG_Vector(IonNode) ast, int index) {
        int start = index;

        IonToken paren = ionParserExpect(parser, ION_TS_LEFT_PAREN);
        ast[index++] = ionNodeCreate(ION_NK_LIST, paren);

        int param_count = 0;
        while (!ionParserConsumeOnMatch(parser, ION_TS_RIGHT_PAREN)) {
            index = ionParseParam(parser, ast, index);
            param_count += 1;
        }

        ast[start].desc_count = index - (start + 1);
        ast[start].data.list_count = param_count;

        return index;
    }

    int ionParseFuncDecl(IonParser* parser, IonNode* ast, int index) {
        int start = index;

        ionParserExpect(parser, ION_TKW_FN);
        IonToken ident = ionParserExpect(parser, ION_TOKEN_IDENTIFIER);
        IonNode node = ionNodeCreate(ION_NK_FUNC_DECL, ident);
        ast[index++] = node;

        index = ionParseParameterList(parser, ast, index);
        ionParserExpect(parser, ION_TS_RIGHT_ARROW);

        index = ionParseType(parser, ast, index);

        index = ionParseStatementBlock(parser, ast, index);
        ast[start].desc_count = index - (start + 1);

        return index;
    }

    int ionParseDeclaration(IonParser* parser, IonNode* ast, int index) {
        IonToken current = ionParserPeekNthToken(parser, 0);

        if (current.kind == ION_TKW_VAR) {
            return ionParseVarDecl(parser, ast, index);
        } else if (current.kind == ION_TKW_FN) {
            return ionParseFuncDecl(parser, ast, index);
        }
    
        return -1;
    }
#endif



CKG_Vector(IonNode) ionParseProgram(CKG_Vector(IonToken) tokens) {
    IonParser parser;
    parser.current = 0;
    parser.tokens = tokens;

    int ast_upperbound = ckg_vector_count(tokens);
    CKG_Vector(IonNode) ast = ckg_vector_grow(NULLPTR, sizeof(IonNode), ast_upperbound);

    int index = 0;
    while (parser.current < (ckg_vector_count(parser.tokens) - 1)) {
		index = ionParseDeclaration(&parser, ast, index);
		if (index == -1) {
			ionParserReportError(&parser, "Unable to parse declaration");
		}
	}

    return ast;
}