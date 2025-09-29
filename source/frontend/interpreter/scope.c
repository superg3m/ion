#include "scope.h"

Scope ionScopeCreate(Scope* parent) {
    Scope ret;
    ret.parent = parent;
    ckg_hashmap_init_string_view_hash(ret.variables, CKG_StringView, IonNode*);

    return ret;
}

void ionScopeFree(Scope* self) {
    ckg_hashmap_free(self->variables);
}

bool ionScopeHas(Scope* self, CKG_StringView key) {
    Scope* current = self;
    while (current != NULLPTR) {
        if (ckg_hashmap_has(current->variables, key)) {
            return true;
        }

        current = current->parent;
    }

    return false;
}

IonNode* ionScopeGet(Scope* self, CKG_StringView key) {
    Scope* current = self;
    while (current != NULLPTR) {
        if (ckg_hashmap_has(current->variables, key)) {
            return ckg_hashmap_get(current->variables, key);
        }

        current = current->parent;
    }

    ckg_assert(false);
    return NULLPTR;
}

void ionScopeSet(Scope* self, CKG_StringView key, IonNode* value) {
    Scope* current = self;
    while (current != NULLPTR) {
        if (ckg_hashmap_has(current->variables, key)) {
            ckg_hashmap_put(current->variables, key, value);
            return;
        }

        current = current->parent;
    }

    ckg_hashmap_put(self->variables, key, value);
}