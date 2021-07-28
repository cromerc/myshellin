/*
 * Copyright 2021 Christopher Cromer
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

#ifndef _MYSHELLIN_ARRAY
#define _MYSHELLIN_ARRAY

/**
 * This struct houses a dynamically sized string array.
 */
typedef struct {
    /**
     * The array.
     */
    char **array;

    /**
     * The amount of elements in the array.
     */
    size_t size;
} StringArray;

/**
 * This struct is used to clean up memory on exit.
 */
typedef struct {
    /**
     * The array of memory to clean.
     */
    void **array;

    /**
     * The amount of elements in the array.
     */
    size_t size;
} CleanArray;

/**
 * This struct has an array list with key value pairs based on strings.
 */
typedef struct {
    /**
     * The keys array.
     */
    StringArray *keys;

    /**
     * The values array.
     */
    StringArray *values;

    /**
     * The amount of elements in the array list.
     */
    size_t size;
} ArrayList;

/**
 * A global clean array to use on exit.
 */
CleanArray clean;

/**
 * A global variables array used for environment variables and values.
 */
ArrayList *variables;

/**
 * Create a String Array by initializing its structure.
 * @return Returns a new String Array.
 */
StringArray *create_string_array();

/**
 * Insert a string into the String Array.
 * @param string_array The String Array to insert into.
 * @param string The string to insert into the String Array.
 */
void insert_string_array(StringArray *string_array, char *string);

/**
 * Delete a string from the String Array.
 * @param string_array The String Array to delete from.
 * @param index The index in the String Array to delete.
 */
void delete_string_array(StringArray *string_array, int index);

/**
 * Free the String Array and all of its strings.
 * @param string_array The String Array to free.
 */
void free_string_array(StringArray *string_array);

/**
 * Create a new Array List.
 * @return Returns the newly created Array List.
 */
ArrayList *create_array_list();

/**
 * Set a key inside the Array List.
 * @param array_list The Array List to work on.
 * @param key The key to insert/update.
 * @param value The value to insert/update.
 */
void set_array_list(ArrayList *array_list, char *key, char *value);

/**
 * Get a value based on a key from an Array List.
 * @param array_list The Array List to work on.
 * @param key The key to search for.
 * @return Returns the value if the key is found in the Array List otherwise it returns NULL.
 */
char *get_array_list(ArrayList *array_list, char *key);

/**
 * Remove a key from the Array List.
 * @param array_list The Array List to work on.
 * @param key The key to remove.
 */
void unset_array_list(ArrayList *array_list, char *key);

/**
 * Free all the memory used in the Array List.
 * @param array_list The Array List to free.
 */
void free_array_list(ArrayList *array_list);

#endif
