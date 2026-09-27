#ifndef ENV_H
#define ENV_H

#include "value.h"

typedef struct Binding {
    char* name;
    Value value;
    struct Binding* next;
} Environment;

Environment* env_bind(Environment* env, const char* name, Value val);
int env_lookup(Environment* env, const char* name, Value* out_val);
void env_free(Environment* env);

#endif
