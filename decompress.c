/*
* Program:      decompress
*
* Purpose:      Decompress a file that was compressed with "compress".
*
* Usage:        decompress INFILE -o OUTFILE
*/

#include "nibbler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_usage(void) {
    printf("Usage:  decompress INFILE -o OUTFILE\n");
    exit(0);
}

/*
* Purpose: 1. Use the nibbler ADT in Section 3 to read a nibble from input the file.
*          2. If the nibble is EOF, then reading the input file has ended. 
*             If the nibble is between 0 and 14, then the nibble represents a byte. 
*             Convert the nibble into that byte. Otherwise, the nibble is 15. 
*             Read the next two nibbles, and combine them to make a byte. 
*             If a 15 nibble is read and either of the two nibbles that follow is EOF then 
*             the input file has ended, and the program should ignore the 15 nibble and any nibble that followed.
*          3. Use fputc() to write the byte of data to the output file.
*          4. Repeat steps 1–3 for all remaining data of the input file
*
* Parameters: NIB *nib: pointer to the NIB struct
*             FILE *fout: pointer to the output file name
*
* Returns: nothing
*
*/
void decompress(NIB *nib, FILE *fout) {
    
    while (1){
        // get a nibble from the compressed file
        int nibble = nib_get_nibble(nib);

        // check if current nibble represents end of file
        if (nibble == EOF){
            break; // if so exit loop
        }

        // check if current nibble is 15
        // if so, then the next 2 nibbles will represent 1 ch (1 byte)
        // this ch never got compressed, so need to read 2 nibbles
        // as 1 byte
        if (nibble == 15){
            // get first nibble (first half of byte)
            int nibble1 = nib_get_nibble(nib);

            // check if this nibble is end of file
            if (nibble1 == EOF){
                break; // if so exit loop
            }

            // get second nibble (second half of byte)
            int nibble2 = nib_get_nibble(nib);

            // check if this nibble is end of file
            if (nibble2 == EOF){
                break; // if so exit loop
            }

            int byte = (nibble1 << 4) | nibble2; // combine nibble1 and nibble2 into a byte
            fputc(byte, fout); // put the byte in the outfile
        }

        // the current nibble was compressed
        else{

            char x = '\0'; // hold decompressed byte in temporary variable

            // convert nibble -> 1 byte character (decompress)
            switch (nibble){
                case 0: x = ' '; break; // nibble 0 -> ' ' 
                case 1: x = 'e'; break; // nibble 1 -> 'e'
                case 2: x = 't'; break; // nibble 2 -> 't'
                case 3: x = 'n'; break; // nibble 3 -> 'n'
                case 4: x = 'r'; break; // nibble 4 -> 'r'
                case 5: x = 'o'; break; // nibble 5 -> 'o'
                case 6: x = 'a'; break; // nibble 6 -> 'a'
                case 7: x = 'i'; break; // nibble 7 -> 'i'
                case 8: x = 's'; break; // nibble 8 -> 's'
                case 9: x = 'd'; break; // nibble 9 -> 'd'
                case 10: x = 'l'; break; // nibble 10 -> 'l'
                case 11: x = 'h'; break; // nibble 11 -> 'h'
                case 12: x = 'c'; break; // nibble 12 -> 'c'
                case 13: x = 'f'; break; // nibble 13 -> 'f'
                case 14: x = 'p'; break; // nibble 14 -> 'p'
                default: break; // in case nibble was not found, exit switch
            }

            fputc(x, fout); // write 1 byte character to outfile
        }
    }
}

/*
* Purpose: confirms that the command line has these arguments after the program name:
*          “infile -o outfile”. Then it will open the infile using nib_open() and open the 
*          outfile using fopen() and call decompress()
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

    // open input file for reading, save NIB pointer as nib
    NIB *nib = nib_open(argv[1], "r");

    // if unable to open input file, report error and exit
    if (nib == NULL){
        fprintf(stderr, "Can't open file for input: %s\n", argv[1]);
        exit(1);
    }

    // open output file for writing, save return value as f
    FILE *f = fopen(argv[3], "w"); 

    // if unable to open output file, report error and exit
    if (f == NULL){
        fprintf(stderr, "Can't open file for output: %s\n", argv[3]);
        exit(1);
    }

    // call decompress on the input file, with a pointer to NIB (nib)
    decompress(nib, f);

    // close the input and output files
    fclose(f);
    nib_close(nib);

    return 0;
}
