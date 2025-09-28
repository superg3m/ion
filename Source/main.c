#include "frontend/lexer/token.h"
#include "frontend/lexer/lexer.h"
#include "frontend/parser/parser.h"
#include "frontend/interpreter/interpreter.h"

#include "frontend/ts/type_system.h"

#define CKG_IMPL
#define CKG_OS_INCLUDE
#include <ckg.h>

int main() {
	// Lexer -> Parser -> AST -> Interpret (AST Walk)
	CKG_Error err = CKG_ERROR_SUCCESS;
	size_t file_size = 0;
	u8* data = ckg_io_read_entire_file("./tests/test.ion", &file_size, &err);
	if (err != CKG_ERROR_SUCCESS) {
		CKG_LOG_ERROR("Error: %s\n", ckg_error_str(err));

		return 1;
	}

	CKG_Vector(IonToken) token_stream = ionLexerGenerateTokenStream(data, file_size);

	for (int i = 0; i < ckg_vector_count(token_stream); i++) {
		IonToken token = token_stream[i];
		ionTokenPrint(token);
	}

	CKG_Vector(IonNode) ast = ionParseProgram(token_stream);

	ionInterpretProgram(ast);

	return 0;
}