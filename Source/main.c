#include <Frontend/token.h>
#include <Frontend/lexer.h>

#define CKG_IMPL
#define CKG_OS_INCLUDE
#include <Core/ckg.h>

int main(int argc, char** argv) {
	// Lexer -> Parser -> AST -> Interpret (AST Walk)
	CKG_Error err = CKG_ERROR_SUCCESS;
	size_t file_size = 0;
	u8* data = ckg_io_read_entire_file("../../ion_source/test.ion", &file_size, &err);
	if (err != CKG_ERROR_SUCCESS) {
		CKG_LOG_ERROR("Error: %s\n", ckg_error_str(err));

		return 1;
	}

	Lexer lexer = lexer_create();
	CKG_Vector(IonToken) token_stream = lexer_consume_token_stream(&lexer, data, file_size);

	for (int i = 0; i < ckg_vector_count(token_stream); i++) {
		IonToken token = token_stream[i];
		CKG_LOG_DEBUG("%s: (%.*s)\n", token_strings[token.type], token.name.length, token.name.data);
	}

	return 0;
}