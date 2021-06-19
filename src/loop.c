#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

void print_input_line() {
    printf("$ ");
}

void loop() {
    size_t buffer_size = 0;
    char *line = NULL;

    while (1) {
        print_input_line();
        getline(&line, &buffer_size, stdin);
    }
}
