#ifndef PARSER_H
#define PARSER_H

#include "env.h"

char* trim(char* str);
Environment* parse_and_eval_line(char* line, Environment* env);
void run_file(const char* filename);

#endif
