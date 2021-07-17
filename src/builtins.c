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

#include <errno.h>
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

    if (strcmp(command, "set") == 0) {
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
    else if (strcmp(args->array[0], "set") == 0) {
        set_variable(args);
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
            fprintf(stderr, "cd: %s \"%s\"\n", strerror(errno), args->array[1]);
        }
        else {
            char *cwd = get_working_directory();
            set_array_list(variables, "PWD", cwd);
            if (cwd != NULL) {
                free(cwd);
                cwd = NULL;
            }
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

void set_variable(StringArray *args) {
    if (args->size < 4) {
        fprintf(stderr, "Too few arguments!\n");
        return;
    }

    if (strcmp(args->array[2], "=") != 0) {
        fprintf(stderr, "Invalid arguments!\n");
        return;
    }

    if (args->array[1][0] != '$') {
        fprintf(stderr, "Invalid variable argument!\n");
        return;
    }

    char *variable = malloc((strlen(args->array[1])) * sizeof(char *));
    if (variable == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    memset(variable, 0, strlen(args->array[1]));

    // Remove the $ symbol from the string
    for (size_t i = 0; i < strlen(args->array[1]); i++) {
        variable[i] = args->array[1][i + 1];
    }

    // Check variable name for invalid characters
    for (size_t i = 0; i < strlen(variable); i++) {
        if (!((variable[i] >= 48 && variable[i] <= 57) || 
                (variable[i] >= 65 && variable[i] <= 90) ||
                (variable[i] >= 97 && variable[i] <= 122))) {
            fprintf(stderr, "Invalid character \"%d\" in variable name!\n", variable[i]);
            return;
        }
    }

    size_t value_length = 1;
    for (size_t i = 3; i < args->size; i++) {
        if (i != 3) {
            value_length++;
        }
        value_length += strlen(args->array[i]);
    }

    char *value = malloc(value_length * sizeof(char *));
    if (value == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    memset(value, 0, value_length);

    for (size_t i = 3; i < args->size; i++) {
        if (i != 3) {
            strcat(value, " ");
        }
        strcat(value, args->array[i]);
    }

    if (strcmp(variable, "PWD") == 0) {
        StringArray *chdir_args = create_string_array();
        insert_string_array(chdir_args, "cd");
        insert_string_array(chdir_args, args->array[3]);
        change_directory(chdir_args);
        free_string_array(chdir_args);
        free(variable);
        free(value);
        // return without setting the variable because change directory will set it on success.
        return;
    }

    set_array_list(variables, variable, value);

    free(variable);
    free(value);
}
