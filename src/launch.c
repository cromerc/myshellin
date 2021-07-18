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
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include "array.h"
#include "utils.h"

void launch_program(StringArray *args) {
    pid_t child = 0;

    child = fork();

    if (child == 0) {
        StringArray *new_args = create_string_array();
        insert_string_array(new_args, args->array[0]);
        for (size_t i = 1; i < args->size; i++) {
            if (args->array[i][0] == '$') {
                char *variable = remove_variable_symbol(args->array[i]);

                char *value = get_array_list(variables, variable);
                if (value != NULL) {
                    insert_string_array(new_args, value);
                }

                if (variable != NULL) {
                    free(variable);
                    variable = NULL;
                }
            }
            else {
                insert_string_array(new_args, args->array[i]);
            }
        }

        if (execvp(new_args->array[0], new_args->array) == -1) {
            fprintf(stderr, "%s: command not found\n", new_args->array[0]);
            free_string_array(args);
            free_string_array(new_args);
            exit(EXIT_FAILURE);
        }
    }
    else if (child < 0) {
        perror("fork");
    }
    else {
        int child_status;
        waitpid(child, &child_status, 0);
    }
}
