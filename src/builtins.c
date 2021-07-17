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

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "array.h"
#include "builtins.h"
#include "console_line.h"

bool is_builtin(char *command) {
    if (strcmp(command, "exit") == 0) {
        return true;
    }

    if (strcmp(command, "cd") == 0) {
        return true;
    }

    if (strcmp(command, "environ") == 0) {
        return true;
    }

    return false;
}

void run_builtin(StringArray *args) {
    if (strcmp(args->array[0], "exit") == 0) {
        exit_shell(args);
    }
    else if (strcmp(args->array[0], "cd") == 0) {
        change_directory(args);
    }
    else if (strcmp(args->array[0], "environ") == 0) {
        print_environ(args);
    }
    else {
        fprintf(stderr, "Builtin %s does not exist!\n", args->array[0]);
    }
}

void exit_shell(StringArray *args) {
    free_string_array(args);
    exit(EXIT_SUCCESS);
}

void change_directory(StringArray *args) {
    if (args->size > 2) {
        fprintf(stderr, "Too many arguments!\n");
        return;
    }
    else if (args->size == 1) {
        char *cwd = get_working_directory();
        fprintf(stdout, "%s\n", cwd);
        if (cwd != NULL) {
            free(cwd);
            cwd = NULL;
        }
    }
    else {
        if (chdir(args->array[1]) != 0) {
            perror("cd");
        }
        else {
            set_array_list(variables, "PWD", get_working_directory());
        }
    }
}

void print_environ(StringArray *args) {
    if (args->size > 1) {
        fprintf(stderr, "Too many arguments!\n");
        return;
    }
    for (size_t i = 0; i < variables->size; i++) {
        fprintf(stdout, "%s=%s\n", variables->keys->array[i], variables->values->array[i]);
    }
}
