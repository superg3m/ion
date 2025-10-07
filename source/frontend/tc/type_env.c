#include "type_env.h"

IonTypeEnv ionTypeEnvCreate(IonTypeEnv* parent) {
    IonTypeEnv ret;
    ret.parent = parent;
    ckg_hashmap_init_string_view_hash(ret.variables, CKG_StringView, IonNode*);

    return ret;
}

void ionTypeEnvFree(IonTypeEnv* self) {
    ckg_hashmap_free(self->variables);
}

bool ionTypeEnvHas(IonTypeEnv* self, CKG_StringView key) {
    IonTypeEnv* current = self;
    while (current != NULLPTR) {
        if (ckg_hashmap_has(current->variables, key)) {
            return true;
        }

        current = current->parent;
    }

    return false;
}

IonNode* ionTypeEnvGet(IonTypeEnv* self, CKG_StringView key) {
    IonTypeEnv* current = self;
    while (current != NULLPTR) {
        if (ckg_hashmap_has(current->variables, key)) {
            return ckg_hashmap_get(current->variables, key);
        }

        current = current->parent;
    }

    ckg_assert(false);
    return NULLPTR;
}

void ionTypeEnvSet(IonTypeEnv* self, CKG_StringView key, IonNode* value) {
    IonTypeEnv* current = self;
    while (current != NULLPTR) {
        if (ckg_hashmap_has(current->variables, key)) {
            ckg_hashmap_put(current->variables, key, value);
            return;
        }

        current = current->parent;
    }

    ckg_hashmap_put(self->variables, key, value);
}