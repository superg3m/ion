#pragma once

#include <ckg.h>
typedef struct IonNode IonNode;
void ionTypecheckProgram(CKG_Vector(IonNode) ast);