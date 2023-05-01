/*
Implements functions related to parsing a file into an abstract
syntax tree
*/
#include <stdio.h>
#include <stdlib.h>
#include "shared.h"
#include "dfsa.h"

ASTnode *parse(FILE* file) {

    // Parse DFSA file
    FILE *dfsa_file = fopen("./panascript.dfsa", "r");
    if (dfsa_file == NULL) {
        fprintf(stderr, "Missing language file panascript.dfsa!!\n");
        exit(1);
    }    
    DFSA *dfsa = parse_dfsa(dfsa_file);

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
