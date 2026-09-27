#ifndef VALUE_H
#define VALUE_H

typedef enum {
    VAL_INT,
    VAL_STRING
} ValueType;

typedef struct {
    ValueType type;
    union {
        int int_val;
        char* str_val;
    } as;
} Value;

void eval_print(Value val);

#endif
