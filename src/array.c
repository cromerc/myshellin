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

#include <stdio.h>
#include <string.h>
#include "array.h"

/**
 * Create a String Array by initializing its structure.
 * @param string_array The String Array to create.
 */
void create_string_array(StringArray *string_array) {
    string_array->array = NULL;
    string_array->size = 0;
}

/**
 * Insert a string into the String Array.
 * @param string_array The String Array to insert into.
 * @param string The string to insert into the String Array.
 */
void insert_string_array(StringArray *string_array, char *string) {
    if (string_array->size == 0) {
        string_array->array = malloc(sizeof(char *));
        if (string_array->array == NULL) {
            fprintf(stderr, "malloc failed");
            exit(EXIT_FAILURE);
        }
    }
    else {
        string_array->array = realloc(string_array->array, (string_array->size + 1) * sizeof(char *));
        if (string_array->array == NULL) {
            fprintf(stderr, "realloc failed");
            exit(EXIT_FAILURE);
        }
    }
    string_array->array[string_array->size] = malloc(sizeof(string));
    if (string_array->array == NULL) {
        fprintf(stderr, "malloc failed");
        exit(EXIT_FAILURE);
    }
    strcpy(string_array->array[string_array->size], string);
    string_array->size++;
}

/**
 * Delete a string from the String Array.
 * @param string_array The String Array to delete from.
 * @param index The index in the String Array to delete.
 */
void delete_string_array(StringArray *string_array, int index) {
    if (string_array->size > 0 && string_array->size > index) {
        for (int i = index; i < string_array->size - 1; i++) {
            free(string_array->array[i]);
            string_array->array[i] = NULL;
            string_array->array[i] = malloc(sizeof(string_array->array[i + 1]));
            if (string_array->array[i] == NULL) {
                fprintf(stderr, "malloc failed");
                exit(EXIT_FAILURE);
            }
            strcpy(string_array->array[i], string_array->array[i + 1]);
        }
        free(string_array->array[string_array->size - 1]);
        string_array->array[string_array->size - 1] = NULL;
        string_array->array = realloc(string_array->array, (string_array->size - 1) * sizeof(char *));
        if (string_array->array == NULL) {
            fprintf(stderr, "realloc failed");
            exit(EXIT_FAILURE);
        }
        string_array->size--;
    }
}

/**
 * Free the String Array and all of its strings.
 * @param string_array The String Array to free.
 */
void free_string_array(StringArray *string_array) {
    for (int i = 0; i < string_array->size; i++) {
        if (string_array->array[i] != NULL) {
            free(string_array->array[i]);
            string_array->array[i] = NULL;
        }
    }
    if (string_array->array != NULL) {
        free(string_array->array);
        string_array->array = NULL;
    }
    string_array->size = 0;
}
