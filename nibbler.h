/*
* File:         nibbler.h
*
* Purpose:      Header for a library that lets one read and write files one
*               "nibble" at a time.
*
*               A nibble is 4 bits of data, or one half of an 8-bit byte.
*/

#include <stdbool.h>
#include <stdio.h>

typedef struct nib NIB;

struct nib {
    FILE *underlying_f;
    bool opened_for_read;
    int num_nibbles; // 0 or 1
    int stored_nibble; // when num_nibbles == 1
};

NIB *nib_open(const char *filename, const char *mode);
int nib_get_nibble(NIB *nib);
void nib_put_nibble(int nibble, NIB *nib);
void nib_close(NIB *nib);
