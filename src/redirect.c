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
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "redirect.h"

void open_redirect(Redirect *redirect) {
    if (redirect->filename != NULL) {
            redirect->fd_copy = dup(redirect->fd);
            if (redirect->fd_copy == -1) {
                perror("dup");
            }
            else {
                if (redirect->append) {
                    redirect->fd_new = open(redirect->filename, O_WRONLY | O_CREAT | O_APPEND, 0664);
                }
                else {
                    redirect->fd_new = open(redirect->filename, O_WRONLY | O_CREAT | O_TRUNC, 0664);
                }
                if (redirect->fd_new == -1) {
                    fprintf(stderr, "open: Could not open file %s: \"%s\"\n", redirect->filename, strerror(errno));
                    free(redirect->filename);
                    redirect->filename = NULL;
                }
                else {
                    if (close(redirect->fd) == -1) {
                        perror("close");
                    }
                    if (dup(redirect->fd_new) == -1) {
                        perror("dup");
                    }
                }
            }
        }
}

void close_redirect(Redirect *redirect) {
    if (redirect->filename != NULL) {
        if (close(redirect->fd_new) == -1) {
            perror("close");
        }
        if (dup2(redirect->fd_copy, redirect->fd) == -1) {
            perror("dup2");
        }
        if (close(redirect->fd_copy) == -1) {
            perror("close");
        }
        free(redirect->filename);
        redirect->filename = NULL;
    }
}

void set_filename(Redirect *redirect, char *filename) {
    if (filename == NULL) {
        redirect->filename = NULL;
    }
    else {
        if (redirect->filename != NULL) {
            redirect->filename = realloc(redirect->filename, (strlen(filename) + 1) * sizeof(char *));
            if (redirect->filename == NULL) {
                perror("remalloc");
                exit(EXIT_FAILURE);
            }
        }
        else {
            redirect->filename = malloc((strlen(filename) + 1) * sizeof(char *));
            if (redirect->filename == NULL) {
                perror("malloc");
                exit(EXIT_FAILURE);
            }
        }
        memset(redirect->filename, 0, strlen(filename) + 1);
        strcpy(redirect->filename, filename);
    }
}
