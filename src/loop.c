/*
 * Copyright 2021 Christopher Cromer
 * Copyright 2021 Raúl Hernandez
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#define _GNU_SOURCE
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "array.h"
#include "builtins.h"
#include "console_line.h"
#include "launch.h"
#include "utils.h"

void add_to_cleanup(void *data) {
    clean.array = realloc(clean.array, (clean.size + 1) * sizeof(void *));
    if (clean.array == NULL) {
        perror("realloc");
        exit(EXIT_FAILURE);
    }
    clean.array[clean.size++] = data;
}

void exit_cleanup() {
    for (size_t i = 0; i < clean.size; i++) {
        if (clean.array[i] != NULL) {
            free(clean.array[i]);
            clean.array[i] = NULL;
        }
    }
    if (clean.array != NULL) {
        free(clean.array);
        clean.array = NULL;
    }
    free_array_list(variables);
}

void loop() {
    clean.size = 0;
    variables = create_array_list();
    atexit(exit_cleanup);
    char *cwd = get_working_directory();
    set_array_list(variables, "PWD", cwd);
    if (cwd != NULL) {
        free(cwd);
        cwd = NULL;
    }

    while (1) {
        print_input_line();

        char *line = get_console_input();

        StringArray *args = create_string_array();

        char *output_file = NULL;
        bool get_output_file = false;
        bool output_append = false;

        char *saveptr = NULL;
        char *token = strtok_r(line, " ", &saveptr);
        while (token) {
            if (get_output_file) {
                get_output_file = false;
                output_file = malloc((strlen(token) +1 ) * sizeof(char *));
                strcpy(output_file, token);
            }
            else if (strcmp(token, ">") == 0) {
                get_output_file = true;
            }
            else if (strcmp(token, ">>") == 0) {
                get_output_file = true;
                output_append = true;
            }
            else {
                insert_string_array(args, token);
            }
            token = strtok_r(NULL, " ", &saveptr);
        }
        if (line != NULL) {
            free(line);
            line = NULL;
        }

        // The user didn't type anything so restart the loop
        if (args->size == 0) {
            free_string_array(args);
            continue;
        }

        int stdout_copy = -1;
        int fd = -1;

        // redirect output to a file instead of stdout
        if (output_file != NULL) {
            stdout_copy = dup(1);
            if (stdout_copy == -1) {
                perror("dup");
            }
            else {
                if (output_append) {
                    fd = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0664);
                }
                else {
                    fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0664);
                }
                if (fd == -1) {
                    fprintf(stderr, "open: Could not open file %s: \"%s\"\n", output_file, strerror(errno));
                    free(output_file);
                    output_file = NULL;
                }
                else {
                    if (close(1) == -1) {
                        perror("close");
                    }
                    if (dup(fd) == -1) {
                        perror("dup");
                    }
                }
            }
        }

        if (is_builtin(args->array[0])) {
            run_builtin(args);
        }
        else {
            launch_program(args);
        }

        if (output_file != NULL) {
            if (close(fd) == -1) {
                perror("close");
            }
            if (dup2(stdout_copy, 1) == -1) {
                perror("dup2");
            }
            free(output_file);
            output_file = NULL;
        }

        free_string_array(args);
    }
}
