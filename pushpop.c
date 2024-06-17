#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>

#define MAX_STACK_SIZE 100

char *dir_stack[MAX_STACK_SIZE];
int stack_top = -1;

void pushd(const char *dir) {
    if (stack_top >= MAX_STACK_SIZE - 1) {
        fprintf(stderr, "Directory stack is full. Cannot push directory.\n");
        return;
    }

    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd");
        return;
    }

    dir_stack[++stack_top] = strdup(cwd);
    if (chdir(dir) != 0) {
        perror("chdir");
        // Pop the last pushed directory in case of failure
        free(dir_stack[stack_top--]);
    } else {
        printf("Changed directory to %s\n", dir);
    }
}

void popd() {
    if (stack_top < 0) {
        fprintf(stderr, "Directory stack is empty. Cannot pop directory.\n");
        return;
    }

    if (chdir(dir_stack[stack_top]) != 0) {
        perror("chdir");
    } else {
        printf("Changed directory to %s\n", dir_stack[stack_top]);
        free(dir_stack[stack_top--]);
    }
}

void print_usage(const char *prog_name) {
    fprintf(stderr, "Usage: %s pushd <directory> | popd\n", prog_name);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "pushd") == 0) {
        if (argc != 3) {
            print_usage(argv[0]);
            return EXIT_FAILURE;
        }
        pushd(argv[2]);
    } else if (strcmp(argv[1], "popd") == 0) {
        if (argc != 2) {
            print_usage(argv[0]);
            return EXIT_FAILURE;
        }
        popd();
    } else {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

