#pragma once

#include "../ast/ast.h"
#include <ckg.h>

typedef struct IonTypeEnv {
	struct IonTypeEnv* parent;
	CKG_HashMap(CKG_StringView, IonNode*)* variables;
} IonTypeEnv;

IonTypeEnv ionTypeEnvCreate(IonTypeEnv* parent);
void ionTypeEnvFree(IonTypeEnv* self);
bool ionTypeEnvHas(IonTypeEnv* self, CKG_StringView key);
IonNode* ionTypeEnvGet(IonTypeEnv* self, CKG_StringView key);
void ionTypeEnvSet(IonTypeEnv* self, CKG_StringView key, IonNode* value);