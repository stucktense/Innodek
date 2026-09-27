#include <stdio.h>
#include "parser.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: ./lang <script.fn>\n");
        return 1;
    }

    run_file(argv[1]);
    return 0;
}
