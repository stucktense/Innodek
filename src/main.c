#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

void install_package(const char* pkg_name) {
    char lib_dir[256];
    const char* prefix = getenv("PREFIX");

    if (prefix) {
        snprintf(lib_dir, sizeof(lib_dir), "%s/lib/lang", prefix);
    } else {
        snprintf(lib_dir, sizeof(lib_dir), "/usr/local/lib/lang");
    }

    char command[512];
    snprintf(command, sizeof(command),
        "mkdir -p %s && curl -sSL https://raw.githubusercontent.com/stucktense/innodek-pkgs/main/%s.fn -o %s/%s.fn",
        lib_dir, pkg_name, lib_dir, pkg_name);

    printf("Fetching package '%s'...\n", pkg_name);
    
    int result = system(command);

    if (result == 0) {
        printf("Successfully installed '%s'!\n", pkg_name);
    } else {
        fprintf(stderr, "Error: Failed to install package '%s'. Make sure curl is installed (pkg install curl).\n", pkg_name);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage:\n");
        printf("  lang <script.fn>                 Run a script file\n");
        printf("  lang pkg install <package_name>  Install a package from remote repo\n");
        return 1;
    }

    if (argc >= 4 && strcmp(argv[1], "pkg") == 0 && strcmp(argv[2], "install") == 0) {
        install_package(argv[3]);
        return 0;
    }

    run_file(argv[1]);
    return 0;
}

