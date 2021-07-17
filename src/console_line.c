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
#include <limits.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "array.h"
#include "color.h"
#include "console_line.h"

/**
 * Remove new line from the end of a string.
 * @param line The string to remove the new line from.
 */
void remove_new_line(char* line) {
    line[strcspn(line, "\n")] = 0;
}

/**
 * Get the logged in user's username.
 * @return Returns the logged in user's username.
 */
char *get_username() {
    struct passwd *pass = NULL;
    pass = getpwuid(getuid());
    if (pass == NULL) {
        perror("getpwuid");
        exit(EXIT_FAILURE);
    }
    return pass->pw_name;
}

/**
 * Get the hostname of the machine.
 * @return Returns the hostname.
 */
char *get_hostname() {
    char hostname[HOST_NAME_MAX + 1];
    gethostname(hostname, HOST_NAME_MAX + 1);
    char *result = malloc((HOST_NAME_MAX + 1) * sizeof(char));
    if (result == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    strcpy(result, hostname);
    return result;
}

/**
 * Get the current working directory of the shell.
 * @return Returns the current working directory.
 */
char *get_working_directory() {
    char *cwd = malloc(PATH_MAX * sizeof(char *));
    getcwd(cwd, PATH_MAX);
    if (cwd == NULL) {
        perror("getcwd");
        exit(EXIT_FAILURE);
    }
    return cwd;
}

/**
 * Print the console line before the user input.
 */
void print_input_line() {
    char *username = get_username();
    char *hostname = get_hostname();
    char *cwd = get_working_directory();
    printf(BRIGHT_GREEN "%s" RESET BRIGHT_MAGENTA "@" RESET BRIGHT_RED "%s" RESET BRIGHT_MAGENTA ":" RESET BRIGHT_BLUE "%s" RESET BRIGHT_MAGENTA "$ " RESET, username, hostname, cwd);
    if (hostname != NULL) {
        free(hostname);
        hostname = NULL;
    }
    if (cwd != NULL) {
        free(cwd);
        cwd = NULL;
    }
}

/**
 * Get input from the console.
 * @return Returns a string input by the user.
 */
char *get_console_input() {
    size_t buffer_size = 0;
    char *line = NULL;

    if (getline(&line, &buffer_size, stdin) == -1) {
        if (feof(stdin)) {
            // the stdin was closed, this usually happens for CTRL-D
            printf("\n");
            if (line != NULL) {
                free(line);
                line = NULL;
            }
            exit(EXIT_SUCCESS);
        }
        else  {
            perror("getline");
            if (line != NULL) {
                free(line);
                line = NULL;
            }
            exit(EXIT_FAILURE);
        }
    }
    remove_new_line(line);
    return line;
}
