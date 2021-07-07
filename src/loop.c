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
#include <stdlib.h>
#include <string.h>
#include "array.h"
#include "builtins.h"
#include "console_line.h"
#include "launch.h"

static CleanArray clean;

/**
 * Add memory address to array to be cleaned up later.
 * @param data The data to be cleaned up on exit.
 */
void add_to_cleanup(void *data) {
    clean.array = realloc(clean.array, (clean.size + 1) * sizeof(void *));
    clean.array[clean.size++] = data;
}

/**
 * Cleanup memory when exiting.
 */
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
}

/**
 * This is the loop that checks for user input and acts on it.
 */
void loop() {
    clean.size = 0;
    atexit(exit_cleanup);
    while (1) {
        print_input_line();

        char *line = get_console_input();

        StringArray args;
        create_string_array(&args);

        char *saveptr = NULL;
        char *token = strtok_r(line, " ", &saveptr);
        while (token) {
            insert_string_array(&args, token);
            token = strtok_r(NULL, " ", &saveptr);
        }
        if (line != NULL) {
            free(line);
            line = NULL;
        }

        // The user didn't type anything so restart the loop
        if (args.size == 0) {
            continue;
        }

        if (is_builtin(args.array[0])) {
            run_builtin(&args);
        }
        else {
            launch_program(&args);
        }

        free_string_array(&args);
    }
}
