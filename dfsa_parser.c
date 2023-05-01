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

            char whitespace[] = " \t\r\n\v\f";  // Define whitespace for strtok delimiter

            // Digit ascii range
            if (buf[0] >= '0' && buf[0] <= '9') {

                char *token = strtok(buf, whitespace); // Get first token, the state ID
                long state_id = strtol(token, NULL, 10); // Convert it
                token = strtok(NULL, ":"); // Grab until colon (:), state name

                printf("Parsed state with ID %ld, NAME '%s'\n", state_id, token);
            } 

            // Tab for state transitions
            else if (buf[0] == '\t') {
                char print_str[500]; // REMOVE DEBUG

                char *str = &buf[1]; // Skips tab
                char *token = strtok(str, ":"); // Gets stripped transition token
                char *stripped = strip(token);
                
                token = strtok(NULL, "\r\n"); // Gets state name of transition
                while (*token == ' ') {token++;} // Skips leading spaces

                // Transition target state specified by its ID
                if (*token >= '0' && *token <= '9') {
                    int target_state_id = strtol(token, NULL, 10);

                    sprintf(print_str, "-> ID %d\n", target_state_id);
                } 
                // Transition target state specified by its name
                else {
                    sprintf(print_str, "-> '%s'\n", token);
                }

                // ASCII Character ranges
                if (stripped[0] == '[') {
                    char c1;
                    char c2;
                    int state = 0;  // 0 not started, 1 read first char, 2 read dash
                    stripped++;
                    while (*stripped != '\0' && *stripped != ']') {
                        
                        // Ignore whitespace
                        if (!(char_in(*stripped, whitespace))) {
                            
                            // Records first character
                            if (state == 0) {
                                c1 = *stripped; 
                                state = 1;
                            } else if (state == 1) {
                                if (*stripped == '-') {
                                    // Skip dash, record state
                                    state = 2;
                                } else {
                                    // Dash expected but not found, error
                                    fprintf(stderr, "Incorrect range syntax, dash expected but not found. Line: %d\n", line_count);
                                    exit(1);
                                }
                            } 
                            // Record 2nd char
                            else if (state == 2) {
                                c2 = *stripped;
                                state = 0;

                                // Add transitions within range (TODO)
                                for (char c = c1; c <= c2; c++) {
                                    // DEBUG: Print state transitions
                                    char temp[500];
                                    sprintf(temp, "Parsed state transition %c ", c);
                                    strcat(temp, print_str);
                                    printf("%s", temp);
                                }
                            }
                        }

                        stripped++;
                    }
                } 
                
                // ASCII Character list
                else if (stripped[0] == '{') {
                    // Removes curly braces
                    stripped++;
                    if (stripped[strlen(stripped) - 1] != '}') {
                        fprintf(stderr, "Closing curly bracket missing on line %d!\n", line_count);
                        exit(1);
                    }
                    stripped[strlen(stripped) - 1] = '\0';

                    // Get next character in list
                    char *list_item = strtok(stripped, ",");
                    
                    while (list_item != NULL) {
                        list_item = strip(list_item);  // Strip spaces

                        // TODO: Add transition
                        char temp[500];
                        sprintf(temp, "Parsed state transition %c ", *list_item);
                        strcat(temp, print_str);
                        printf("%s", temp);

                        list_item = strtok(NULL, ","); // Next char
                    }
                } 
                
                // ASCII Numeric list
                else if (stripped[0] == '(') {
                    // Removes parenthesis
                    stripped++;
                    if (stripped[strlen(stripped) - 1] != ')') {
                        fprintf(stderr, "Closing parenthesis missing on line %d!\n", line_count);
                        exit(1);
                    }
                    stripped[strlen(stripped) - 1] = '\0';

                    // Get next number in list
                    char *list_item = strtok(stripped, ",");

                    while (list_item != NULL) {
                        list_item = strip(list_item);  // Strip spaces

                        int ascii_code = strtol(list_item, NULL, 10);

                        // TODO: Add transition
                        char temp[500];
                        sprintf(temp, "Parsed state transition %d ", ascii_code);
                        strcat(temp, print_str);
                        printf("%s", temp);

                        list_item = strtok(NULL, ","); // Next number
                    }
                } 
                
                // Single character
                else {
                    char trans_chr = *stripped; 

                    // DEBUG: Print state transition
                    char temp[500];
                    sprintf(temp, "Parsed state transition %c ", trans_chr);
                    strcat(temp, print_str);
                    printf("%s", temp);
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
