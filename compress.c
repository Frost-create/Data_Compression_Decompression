/*
* Program:      compress
*
* Purpose:      Compress a file.
*
* Usage:        compress INFILE -o OUTFILE
*/

#include "nibbler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_usage(void) {
    printf("Usage:  compress INFILE -o OUTFILE\n");
    exit(0);
}

/*
* Purpose: compresses data from input file fin and writes the compressed data to
*          output file nib. It assumes that fin already has been opened by fin = fopen(infilename, "r") and that
*          nib already has been opened by nib = nib_open(outfilename, "w")
*
* Parameters: FILE *fin: pointer to the input file name
*             NIB *nib: pointer to the NIB struct
*
* Returns: nothing
*
*/
void compress(FILE *fin, NIB *nib) {

    while (1){

        int ch = fgetc(fin); // read a character (1 byte) from open input file

        if (ch == EOF){
            break; // if EOF, then exit while loop
        }

        int x = -1; // set x initially to a value not in the conversion table

        // convert common ch byte -> nibble (compress)
        switch (ch) {
            case ' ': x = 0; break; // binary 0000 (0x0)
            case 'e': x = 1; break; // binary 0001 (0x1)
            case 't': x = 2; break; // binary 0010 (0x2)
            case 'n': x = 3; break; // binary 0011 (0x3)
            case 'r': x = 4; break; // binary 0100 (0x4)
            case 'o': x = 5; break; // binary 0101 (0x5)
            case 'a': x = 6; break; // binary 0110 (0x6)
            case 'i': x = 7; break; // binary 0111 (0x7)
            case 's': x = 8; break; // binary 1000 (0x8)
            case 'd': x = 9; break; // binary 1001 (0x9)
            case 'l': x = 10; break; // binary 1010 (0x0A)
            case 'h': x = 11; break; // binary 1011 (0x0B)
            case 'c': x = 12; break; // binary 1100 (0x0C)
            case 'f': x = 13; break; // binary 1101 (0x0D)
            case 'p': x = 14; break; // binary 1110 (0x0E)
            default: x = -1; break; // in case byte was not found in conversion table, exit switch
        }

        // check if byte is in conversion table
        if (x != -1){
            nib_put_nibble(x, nib); // add it as a nibble
        }

        // if byte not in conversion table, write out the full ch as a nibble
        else{
            nib_put_nibble(15, nib); // write a 15 nibble
    
            int high_nibble = (ch >> 4) & 0b00001111; // isolate the high nibble of the ch
            nib_put_nibble(high_nibble, nib); // write the high nibble

            int low_nibble = ch & 0b00001111; // isolate the low nibble of the ch
            nib_put_nibble(low_nibble, nib); // write the low nibble 
        }
    }
}

/*
* Purpose: confirms that the command line has these arguments after the program name:
*          “infile -o outfile”. Then it will open the infile using fopen() and open the 
*          outfile using nib_open() and call compress()
*
* Parameters: int argc: the number of command line arguements
*             char **argv: the value of each command line arguement
*
* Returns: nothing
*
*/
int main(int argc, char **argv) {
    // check valid number of arguements provoided
    if (argc != 4){
        print_usage(); // if not, report it and exit
    }

    // check "-o" was included in command line
    if (strcmp(argv[2], "-o") != 0){
        print_usage(); // if not, report it and exit
    }

    // open input file for reading, save return value f
    FILE *f = fopen(argv[1], "r");

    // if unable to open input file, report error and exit
    if (f == NULL){
        fprintf(stderr, "Can't open file for input: %s\n", argv[1]);
        exit(1);
    }

    // open output file for writing, save NIB pointer as nib
    NIB *nib = nib_open(argv[3], "w"); 

    // if unable to open output file, report error and exit
    if (nib == NULL){
        fprintf(stderr, "Can't open file for output: %s\n", argv[3]);
        exit(1);
    }

    // call compress on the input file, with a pointer to NIB (nib)
    compress(f, nib);

    // close the input and output files
    fclose(f);
    nib_close(nib);

    return 0;
}
