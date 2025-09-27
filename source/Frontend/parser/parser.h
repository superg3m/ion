#include <Core/ckg.h>
#include <Frontend/token.h>

typedef struct Parser {
    CKG_Vector(IonToken) tokens;
    int current;
} Parser;

// IonAst ion_parse_program();