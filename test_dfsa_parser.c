#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dfsa.h"

int main(int argc, char **argv) {
    if (argc == 2) {

        // Open file and error check
        FILE *file = fopen(argv[1], "r");
        if (file == NULL) {
            fprintf(stderr, "File does not exist!\n");
            return 1;
        }

        // Call DFSA Parser
        parse_dfsa(file);

    } else {
        fprintf(stderr, "Too many or too little arguments!\n\nCommand Syntax:\ndfsa_parser <filename>\n");
        return 1;
    }

    return 0;
}
