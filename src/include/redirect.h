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

#ifndef _MYSHELLIN_REDIRECT
#define _MYSHELLIN_REDIRECT

/**
 * This structure helps for handling redirection to new fds.
 */
typedef struct {
    /**
     * The fd to work with. Should be either 1 for stdout or 2 for stderr.
     * Once set it should not be changed.
     */
    int fd;

    /**
     * The new fd for the file.
     */
    int fd_new;

    /**
     * The copy of the original fd.
     */
    int fd_copy;

    /**
     * The name of the file to write to.
     */
    char *filename;

    /**
     * Whether or not to truncate or append the file.
     */
    bool append;
} Redirect;

/**
 * Open an fd to handle redirect.
 * @param redirect The redirect struct that houses the new fd and the old fd.
 */
void open_redirect(Redirect *redirect);

/**
 * Close an fd and restore the original.
 * @param redirect The redirect struct that houses the new fd and the old fd.
 */
void close_redirect(Redirect *redirect);

/**
 * Set the filename in a redirect object.
 * @param redirect The redirect struct to set the filename to.
 * @param filename The new filename to set.
 */
void set_filename(Redirect *redirect, char *filename);

#endif
