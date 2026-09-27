#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"

// Helper to trim leading and trailing whitespace
char* trim(char* str) {
    while(isspace((unsigned char)*str)) str++;
    if(*str == 0) return str;
    char* end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;
    end[1] = '\0';
    return str;
}

// Parse and execute a single line of code
Environment* parse_and_eval_line(char* line, Environment* env) {
    line = trim(line);
    
    // Skip empty lines and comments
    if (strlen(line) == 0 || line[0] == '#') return env;

    // 1. Match: var.new[name, value]
    if (strncmp(line, "var.new[", 8) == 0) {
        char* start = line + 8;
        char* bracket = strchr(start, ']');
        if (!bracket) {
            printf("Syntax Error: Missing closing bracket ']'\n");
            return env;
        }
        *bracket = '\0'; // Null-terminate inside brackets

        char* comma = strchr(start, ',');
        if (!comma) {
            printf("Syntax Error: Missing comma in var.new\n");
            return env;
        }
        *comma = '\0';

        char* name = trim(start);
        char* val_str = trim(comma + 1);

        Value val;
        // Check if string literal
        if (val_str[0] == '"' && val_str[strlen(val_str)-1] == '"') {
            val_str[strlen(val_str)-1] = '\0'; // Strip trailing quote
            val.type = VAL_STRING;
            val.as.str_val = strdup(val_str + 1); // Strip leading quote
        } else {
            // Otherwise treat as integer
            val.type = VAL_INT;
            val.as.int_val = atoi(val_str);
        }

        return env_bind(env, name, val);
    }

    // 2. Match: printIn(argument)
    else if (strncmp(line, "printIn(", 8) == 0) {
        char* start = line + 8;
        char* paren = strrchr(start, ')');
        if (!paren) {
            printf("Syntax Error: Missing closing parenthesis '()'\n");
            return env;
        }
        *paren = '\0';
        char* arg = trim(start);

        Value val;
        if (arg[0] == '"' && arg[strlen(arg)-1] == '"') {
            arg[strlen(arg)-1] = '\0';
            val.type = VAL_STRING;
            val.as.str_val = arg + 1;
            eval_print(val);
        } else if (env_lookup(env, arg, &val)) {
            eval_print(val);
        } else if (isdigit(arg[0])) {
            val.type = VAL_INT;
            val.as.int_val = atoi(arg);
            eval_print(val);
        } else {
            printf("Runtime Error: Undefined variable or invalid syntax '%s'\n", arg);
        }
    }

    // 3. Match: pkg.install[module_name]
    else if (strncmp(line, "pkg.install[", 12) == 0) {
        char* start = line + 12;
        char* bracket = strchr(start, ']');
        if (!bracket) {
            printf("Syntax Error: Missing closing bracket ']'\n");
            return env;
        }
        *bracket = '\0';
        char* mod_name = trim(start);

        // Strip quotes if passed as a string literal
        if (mod_name[0] == '"' && mod_name[strlen(mod_name)-1] == '"') {
            mod_name[strlen(mod_name)-1] = '\0';
            mod_name++;
        }

        // Build file path (.fn)
        char filepath[512];
        FILE* mod_file = NULL;

        // Try 1: Look in the current local directory first
        snprintf(filepath, sizeof(filepath), "%s.fn", mod_name);
        mod_file = fopen(filepath, "r");

        // Try 2: If not found locally, look in the global system directory
        if (!mod_file) {
            snprintf(filepath, sizeof(filepath), "/usr/local/share/lang/%s.fn", mod_name);
            mod_file = fopen(filepath, "r");
        }

        if (!mod_file) {
            fprintf(stderr, "Runtime Error: Could not find package '%s'\n", mod_name);
            return env;
        }

        // Read and evaluate package line by line
        char mod_line[256];
        while (fgets(mod_line, sizeof(mod_line), mod_file) != NULL) {
            mod_line[strcspn(mod_line, "\r\n")] = 0;
            env = parse_and_eval_line(mod_line, env);
        }

        fclose(mod_file);
    } 
    
    else {
        printf("Syntax Error: Unknown command '%s'\n", line);
    }

    return env;
}

// Opens and executes an entire script file line by line
void run_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Could not open file '%s'\n", filename);
        return;
    }

    Environment* env = NULL;
    char line[256];

    while (fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\r\n")] = 0;
        env = parse_and_eval_line(line, env);
    }

    fclose(file);
    env_free(env); // Clean up memory when script completes
}

