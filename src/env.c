#include <stdlib.h>
#include <string.h>
#include "env.h"

Environment* env_bind(Environment* env, const char* name, Value val) {
    Environment* new_binding = malloc(sizeof(Environment));
    new_binding->name = strdup(name);
    new_binding->value = val;
    new_binding->next = env;
    return new_binding;
}

int env_lookup(Environment* env, const char* name, Value* out_val) {
    Environment* current = env;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            *out_val = current->value;
            return 1;
        }
        current = current->next;
    }
    return 0;
}

void env_free(Environment* env) {
    while (env != NULL) {
        Environment* next = env->next;
        free(env->name);
        if (env->value.type == VAL_STRING) {
            free(env->value.as.str_val);
        }
        free(env);
        env = next;
    }
}
