/*
Implements functions related to parsing a file into an abstract
syntax tree
*/
#include <stdio.h>
#include "shared.h"
#include "dfsa.h"

ASTnode *parse(FILE* file) {

    while (1) {
        // Read next character
        char next;
        int num_read = fread(&next, sizeof(char), 1, file);
        


        // EOF or error occurred if we read 0 (expected to read 1)
        if (num_read == 0) {
            break;
        }
    }

    return NULL;
}
