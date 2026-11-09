/*
* File:     nibbler.c
*
* Purpose:  Normally files are read and written one byte at a time, but
*           these routines let us read and write files one nibble (4 bits)
*           at a time.
*/

#include "nibbler.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
* Purpose: opens a file so that it can be used by nib_get_nibble() or nib_put_nibble()
*
* Parameters: const char *filename: a pointer to the name of the file to open
*             const char *mode: a pointer to the mode, either 'w' (writing) or 'r' (reading)
*
* Returns: a pointer to the nib struct (NIB) called nib
*
*/
NIB *nib_open(const char *filename, const char *mode) {

    FILE *f = fopen(filename, mode); // open file for either reading or writing (declared in mode)

    if (f == NULL){ // check to see if valid input for filename
        return NULL; // if filename doesn't exist, return nothing
    }

    NIB *nib = calloc(1, sizeof(NIB)); // allocate space to store NIB

    nib->underlying_f = f; // set the file pointer to NIB underlying_f

    if (mode[0] == 'r'){ 
        nib->opened_for_read = 1; // set NIB opened_for_read to 1 if mode is set to read
    }

    return nib; // return pointer for nib struct
}

/*
* Purpose: read a nibble from an open input file
*
* Parameters: NIB *nib: a pointer to the struct NIB
*
* Returns: if there is already a value stored in stored_nibble: return that value
*          otherwise: return the high nibble of a byte read from the open file
*
*/
int nib_get_nibble(NIB *nib) {
    // check if there is a value already stored in stored_nibble
    // if so, return that value, and set num_nibbles = 0
    // (0 = no stored value, 1 = stored value)
    if (nib->num_nibbles > 0){
        nib->num_nibbles = 0;
        return nib->stored_nibble;
    }

    else{
        int byte = fgetc(nib->underlying_f); // get a character (one byte) from the file stored in underlying_f

        // check if it's EOF
        if (byte == EOF){
            return EOF;
        }

        int low_nibble = byte & 0b00001111; // isolate the low nibble of the byte
        int high_nibble = (byte >> 4) & 0b00001111; // isolate the high nibble of the byte
        nib->stored_nibble = low_nibble; // store the low nibble in stored_nibble
        nib->num_nibbles = 1; // set num_nibbles = 1 
        return high_nibble; // return the high nibble
    }
}

/*
* Purpose: writes a nibble to an open output file
*
* Parameters: int nibble: an input nibble value
*             NIB *nib: a pointer to the struct NIB
*
* Returns: nothing
*
*/
void nib_put_nibble(int nibble, NIB *nib) {
    // if there is a value stored in stored_nibble
    // make a byte out of the stored_nibble and the input nibble
    if (nib->num_nibbles > 0){
        int high_nibble = nib->stored_nibble; // take the stored nibble and make it the high part of the byte
        int low_nibble = nibble; // take the input nibble and make it the low part of the byte
        int my_byte = (high_nibble << 4) | low_nibble; // combine the high nibble and low nibble into a byte
        fputc(my_byte, nib->underlying_f); // write the byte to the file stored in underlying_f
        nib->num_nibbles = 0; // set num_nibbles to 0 (no stored value in stored_nibble)
    }

    else{
        nib->stored_nibble = nibble; // store the input value nibble into stored_nibble
        nib->num_nibbles = 1; // set num_nibbles to 1 (exists a stored value in stored_nibble)
    }
}

/*
* Purpose: closes a file opened by nib_open()
*
* Parameters: NIB *nib: a pointer (called nib) to the NIB struct
*
* Returns: nothing
*
*/
void nib_close(NIB *nib) {
    //if the file is opened for writing, and there is a stored nibble
    if (nib->opened_for_read == 0 && nib->num_nibbles > 0){
        int high_nibble = nib->stored_nibble; //set the high nibble to the stored nibble
        int low_nibble = 0b1111; // set the low nibble to binary 1111
        int my_byte = (high_nibble << 4) | low_nibble; // combine the high nibble and low nibble into a byte
        fputc(my_byte, nib->underlying_f); // write the byte to the file stored in underlying_f
    }

    fclose(nib->underlying_f); // close the file stored in underlying_f
    free(nib); // free the nib pointer
}
