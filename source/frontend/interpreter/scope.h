#pragma once

#include "../ast/ast.h"

typedef struct Scope {
	struct Scope* parent   ;
	CKG_HashMap(CKG_StringView, IonNode*)* variables;
} Scope;

Scope ionScopeCreate(Scope* parent);
void ionScopeFree(Scope* self);
bool ionScopeHas(Scope* self, CKG_StringView key);
IonNode* ionScopeGet(Scope* self, CKG_StringView key);
void ionScopeSet(Scope* self, CKG_StringView key, IonNode* value);