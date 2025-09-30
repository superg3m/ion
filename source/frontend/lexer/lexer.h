#pragma once

#include <ckg.h>
#include "token.h"

CKG_Vector(IonToken) ionLexerGenerateTokenStream(u8* source, u64 source_length);