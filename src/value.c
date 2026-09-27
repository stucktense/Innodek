#include <stdio.h>
#include "value.h"

void eval_print(Value val) {
    if (val.type == VAL_INT) {
        printf("%d\n", val.as.int_val);
    } else if (val.type == VAL_STRING) {
        printf("%s\n", val.as.str_val);
    }
}
