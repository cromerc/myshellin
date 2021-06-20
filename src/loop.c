#define _GNU_SOURCE
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "user.h"

void remove_new_line(char* line) {
    line[strcspn(line, "\n")] = 0;
}

void print_input_line() {
    char *name = get_user();
    printf("%s $ ", name);
}

void loop() {
    size_t buffer_size = 0;
    char *line = NULL;

    while (1) {
        print_input_line();
        if (getline(&line, &buffer_size, stdin) == -1) {
            if (feof(stdin)) {
                // the stdin was closed, this usually happens for CTRL-D
                printf("\n");
                exit(EXIT_SUCCESS);
            }
            else  {
                perror("Error: ");
                printf("\n");
                exit(EXIT_FAILURE);
            }
        }

        remove_new_line(line);

        if (strcmp(line, "quit") == 0) {
            break;
        }
    }
}
