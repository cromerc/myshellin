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
 * @return Returns a new String Array.
 */
StringArray *create_string_array() {
    StringArray *string_array = malloc(sizeof(StringArray));
    if (string_array == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    string_array->array = NULL;
    string_array->size = 0;
    return string_array;
}

/**
 * Insert a string into the String Array.
 * @param string_array The String Array to insert into.
 * @param string The string to insert into the String Array.
 */
void insert_string_array(StringArray *string_array, char *string) {
    if (string_array->array == NULL) {
        string_array->array = malloc(2 * sizeof(char *));
        if (string_array->array == NULL) {
            perror("malloc");
            free_string_array(string_array);
            exit(EXIT_FAILURE);
        }
    }
    else {
        string_array->array = realloc(string_array->array, (string_array->size + 2) * sizeof(char *));
        if (string_array->array == NULL) {
            perror("realloc");
            free_string_array(string_array);
            exit(EXIT_FAILURE);
        }
    }
    string_array->array[string_array->size] = malloc(strlen(string) * sizeof(char *));
    if (string_array->array == NULL) {
        perror("malloc");
        free_string_array(string_array);
        exit(EXIT_FAILURE);
    }
    strcpy(string_array->array[string_array->size], string);
    // A NULL terminated array
    string_array->array[string_array->size + 1] = NULL;
    string_array->size++;
}

/**
 * Delete a string from the String Array.
 * @param string_array The String Array to delete from.
 * @param index The index in the String Array to delete.
 */
void delete_string_array(StringArray *string_array, int index) {
    if (string_array->array != NULL && string_array->size > 0 && string_array->size > index) {
        for (size_t i = index; i < string_array->size - 1; i++) {
            free(string_array->array[i]);
            string_array->array[i] = NULL;
            string_array->array[i] = malloc(strlen(string_array->array[i + 1]) * sizeof(char *));
            if (string_array->array[i] == NULL) {
                perror("malloc");
                exit(EXIT_FAILURE);
            }
            strcpy(string_array->array[i], string_array->array[i + 1]);
        }
        free(string_array->array[string_array->size - 1]);
        string_array->array[string_array->size - 1] = NULL;
        string_array->array = realloc(string_array->array, (string_array->size) * sizeof(char *));
        if (string_array->array == NULL) {
            perror("realloc");
            free_string_array(string_array);
            exit(EXIT_FAILURE);
        }
        string_array->size--;
    }
    else {
        fprintf(stderr, "StringArray index doesn't exist!\n");
        free_string_array(string_array);
        exit(EXIT_FAILURE);
    }
}

/**
 * Free the String Array and all of its strings.
 * @param string_array The String Array to free.
 */
void free_string_array(StringArray *string_array) {
    for (size_t i = 0; i < string_array->size; i++) {
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
    if (string_array != NULL) {
        free(string_array);
        string_array = NULL;
    }
}

/**
 * Create a new Array List.
 * @return Returns the newly created Array List.
 */
ArrayList *create_array_list() {
    ArrayList *array_list = malloc(sizeof(ArrayList));
    if (array_list == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    array_list->keys = NULL;
    array_list->values = NULL;
    array_list->size = 0;
    return array_list;
}

/**
 * Set a key inside the Array List.
 * @param array_list The Array List to work on.
 * @param key The key to insert/update.
 * @param value The value to insert/update.
 */
void set_array_list(ArrayList *array_list, char *key, char *value) {
    if (array_list->keys == NULL) {
        array_list->keys = create_string_array();
        array_list->values = create_string_array();
    }
    for (size_t i = 0; i < array_list->size; i++) {
        if (strcmp(array_list->keys->array[i], key) == 0) {
            array_list->values->array[i] = realloc(array_list->values->array[i], strlen(value) * sizeof(char *));
            strcpy(array_list->values->array[i], value);
            if (array_list->values->array[i] == NULL) {
                perror("realloc");
                exit(EXIT_FAILURE);
            }
            return;
        }
    }
    insert_string_array(array_list->keys, key);
    insert_string_array(array_list->values, value);
    array_list->size++;
}

/**
 * Get a value based on a key from an Array List.
 * @param array_list The Array List to work on.
 * @param key The key to search for.
 * @return Returns the value if the key is found in the Array List otherwise it returns NULL.
 */
char *get_array_list(ArrayList *array_list, char *key) {
    if (array_list->keys != NULL) {
        for (size_t i = 0; i < array_list->size; i++) {
            if (strcmp(array_list->keys->array[i], key) == 0) {
                return array_list->values->array[i];
            }
        }
    }
    return NULL;
}

/**
 * Remove a key from the Array List.
 * @param array_list The Array List to work on.
 * @param key The key to remove.
 */
void unset_array_list(ArrayList *array_list, char *key) {
    if (array_list->keys != NULL) {
        for (size_t i = 0; i < array_list->size; i++) {
            if (strcmp(array_list->keys->array[i], key) == 0) {
                delete_string_array(array_list->keys, i);
                delete_string_array(array_list->values, i);
                array_list->size--;
                return;
            }
        }
    }
}

/**
 * Free all the memory used in the Array List.
 * @param array_list The Array List to free.
 */
void free_array_list(ArrayList *array_list) {
    if (array_list != NULL) {
        free_string_array(array_list->keys);
        free_string_array(array_list->values);
        free(array_list);
        array_list = NULL;
    }
}
