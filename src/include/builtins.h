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
#include "array.h"

#ifndef _MYSHELLIN_BUILTINS
#define _MYSHELLIN_BUILTINS

/**
 * Check if the command is a builtin or not.
 * @param command String with the command name to check.
 * @return Returns true if it's a builtin or false otherwise.
 */
bool is_builtin(char *command);

/**
 * Run the builtin command.
 * @param args An array of strings containing the arguments to run.
 */
void run_builtin(StringArray *args);

/**
 * Exit the shell.
 * @param args The arguments that were used to call exit. This is used to free the memory before exit.
 */
void exit_shell(StringArray *args);

/**
 * Change the directory to what the user inputs.
 * @param args The arguments the user input.
 */
void change_directory(StringArray *args);

/**
 * Print the current working directory.
 */
void print_current_directory();

/**
 * Print all of the environment variables.
 * @param args The arguments the user input.
 */
void print_environ(StringArray *args);

/**
 * Set an environment variable.
 * @param args The arguments passed to set.
 */
void set_variable(StringArray *args);

/**
 * Print a message or variable.
 * @param args The arguments passed to echo.
 */
void echo(StringArray *args);

#endif
