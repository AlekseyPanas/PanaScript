#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dfsa.h"
#include "print_utils.h"

int main(int argc, char **argv) {
    if (argc == 2) {

        // Open file and error check
        FILE *file = fopen(argv[1], "r");
        if (file == NULL) {
            fprintf(stderr, "File does not exist!\n");
            return 1;
        }

        int line_count = 1;

        while (1) {
            // Read next line, exit if end of file
            char buf[MAX_DFSA_LINE_LENGTH];
            char *out = fgets(buf, MAX_DFSA_LINE_LENGTH, file);
            if (out == NULL) {
                break;
            }

            // print_bin(buf);

            char whitespace[]= " \t\r\n\v\f";  // Define whitespace for strtok delimiter

            // Digit ascii range
            if (buf[0] >= '0' && buf[0] <= '9') {

                char *token = strtok(buf, whitespace); // Get first token, the state ID
                long state_id = strtol(token, NULL, 10); // Convert it
                token = strtok(NULL, ":"); // Grab until colon (:), state name

                printf("Parsed state with ID %ld, NAME '%s'\n", state_id, token);
            } 

            // Tab for state transitions
            else if (buf[0] == '\t') {

                char *str = &buf[1]; // Skips tab
                char *token = strtok(str, ":"); // Gets transition character
                char trans_chr = *token; 
                token = strtok(NULL, "\r\n"); // Gets state name of transition
                while (*token == ' ') {token++;} // Skips leading spaces

                // Transition target state specified by its ID
                if (*token >= '0' && *token <= '9') {
                    int target_state_id = strtol(token, NULL, 10);

                    printf("Parsed state transition %c -> ID %d\n", trans_chr, target_state_id);
                } 
                // Transition target state specified by its name
                else {
                    printf("Parsed state transition %c -> '%s'\n", trans_chr, token);
                }
                
            }

            // Else: If not # or whitespace, we are dealing with an invalid line
            else if (strchr(" #\t\r\n\v\f", buf[0]) == NULL) {
                fprintf(stderr, "Invalid syntax on line %d\n", line_count);
                exit(1);
            }

            line_count++;
        }

    } else {
        fprintf(stderr, "Too many or too little arguments!\n\nCommand Syntax:\ndfsa_parser <filename>\n");
        return 1;
    }
    
    return 0;
}
