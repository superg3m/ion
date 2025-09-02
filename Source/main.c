#include <Core/ckg.h>

int main(int argc, char** argv) {
	// Lexer -> Parser -> AST -> Interpret (AST Walk)
	CKG_LOG_PRINT("HELLO WORLD!\n");

	CKG_Error err = CKG_ERROR_SUCCESS;
	size_t file_size = 0;
	u8* data = ckg_io_read_entire_file("../../ion_source/test.ion", &file_size, &err);
	if (err != CKG_ERROR_SUCCESS) {
		CKG_LOG_ERROR("Error: %s\n", ckg_error_str(err));

		return 1;
	}

	return 0;
}