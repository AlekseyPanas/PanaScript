#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dfsa.h"
#include "print_utils.h"

#define DEBUG 1  // 0 = no debug, 1 = Record notes, 2 = Additional ascii codes of each line


/*
Add a new state to the DSFA linked list given by head with no transitions. Return pointer to this new node
*/
DFSA *add_state(DFSA **head, int id, char *name) {
    DFSA *new_state = malloc(sizeof(DFSA));
    new_state->id = id;
    char *str = malloc(sizeof(char) * (strlen(name) + 1));
    strcpy(str, name);
    new_state->name = str;
    new_state->next = NULL;
    new_state->transitions = NULL;

    while (*head != NULL) {
        head = &((*head)->next);
    }
    *head = new_state;

    return new_state;
}


/*
Append a new transition  src --chr--> target. Return the transition node
*/
Trans *add_transition(DFSA *src, char chr, DFSA *target) {
    Trans *transition = malloc(sizeof(Trans));
    transition->chr = chr;
    transition->next = NULL;
    transition->target = target;

    if (src->transitions == NULL) {
        src->transitions = transition;
    } else {
        // Loop until last item in linked list
        Trans *t = src->transitions;
        while (t->next != NULL) {
            t = t->next;
        }

        // Append new transition
        t->next = transition;
    }

    return transition;
}


/*
Return a pointer to the DFSA state identified by id. Return NULL if state doesn't exist
*/
DFSA *ptr_from_id(DFSA *head, int id) {
    while (head != NULL) {
        if (head->id == id) {
            return head;
        }
        head = head->next;
    }
    return NULL;
}


/*
Return a pointer to the DFSA state the requested name. Return NULL if state doesn't exist
*/
DFSA *ptr_from_name(DFSA *head, char *name) {
    while (head != NULL) {
        if (strcmp(head->name, name) == 0) {
            return head;
        }
        head = head->next;
    }
    return NULL;
}

DFSA *eval_trans(DFSA *cur, char c) {
    Trans *t = cur->transitions;
    while (t != NULL) {
        if (t->chr == c) {
            return t->target;
        }
        t = t->next;
    }
    return NULL;
}

/*
Print the DFSA graph state by state
*/
void print_dfsa(DFSA *head) {
    while (head != NULL) {
        printf("[%d] '%s'\n", head->id, head->name);

        Trans *t = head->transitions;
        while (t != NULL) {
            if (t->chr < 32) {
                printf("\t--%d--> '%s'\n", t->chr, t->target->name);
            } else {
                printf("\t--'%c'--> '%s'\n", t->chr, t->target->name);
            }
            
            t = t->next;
        }

        head = head->next;
    }
}


/*
Cannot evaluate state transitions as we go through the file since a transition may target a state that
hasn't been parsed yet. We will store transition info in this linked list and then add it to the DFSA
at the end.
Either name or ID is set. Name will be NULL if not set but id will not be initialized so it might be garbage
*/
typedef struct temp_transition {
    DFSA *src;
    char *name;
    int id;
    char chr;
    struct temp_transition *next;
} TempTrans;


/*
Add a temporary transition to a linked list. name can be NULL, and id can be -1. Only one of the two is needed
*/
TempTrans *add_temp_trans(TempTrans **head, DFSA *src, char *name, int id, char chr) {
    TempTrans *new_trans = malloc(sizeof(TempTrans));
    new_trans->chr = chr;
    if (name != NULL) {
        char *str = malloc(sizeof(char) * (strlen(name) + 1));
        strcpy(str, name);
        new_trans->name = str;
    } else {
        new_trans->name = NULL;
        new_trans->id = id;
    }
    new_trans->src = src;
    
    while (*head != NULL) {
        head = &((*head)->next);
    }

    *head = new_trans;

    return new_trans;
}


DFSA *parse_dfsa(FILE* file) {
    int line_count = 1;

    // Initialize DFSA
    DFSA *dfsa = NULL;

    // Store source state for transitions
    DFSA *src = NULL;

    // Temporary transition storage
    TempTrans *temp_trans_list = NULL;

    while (1) {
        // Read next line, exit if end of file
        char buf[MAX_DFSA_LINE_LENGTH];
        char *out = fgets(buf, MAX_DFSA_LINE_LENGTH, file);
        if (out == NULL) {
            break;
        }

        if (DEBUG == 2) {print_bin(buf);}

        char whitespace[] = " \t\r\n\v\f";  // Define whitespace for strtok delimiter

        // Digit ascii range: State definition
        if (buf[0] >= '0' && buf[0] <= '9') {

            char *token = strtok(buf, whitespace); // Get first token, the state ID
            long state_id = strtol(token, NULL, 10); // Convert it
            token = strtok(NULL, ":"); // Grab until colon (:), state name

            // Add new state and save it as src for any upcoming transitions
            src = add_state(&dfsa, state_id, token);

            if (DEBUG == 1) {printf("Parsed state with ID %ld, NAME '%s'\n", state_id, token);}
        } 

        // Tab for state transitions
        else if (buf[0] == '\t') {
            // If src isnt set, means we have a transition with no parent state. Error!
            if (src == NULL) {
                fprintf(stderr, "Transition with no parent state! (Line %d)\n", line_count);
                exit(1);
            }
            
            char print_str[500]; // Used for DEBUG printing only

            // name and id to be passed directly to an add_temp_trans call
            char *temp_trans_name;
            int temp_trans_id;

            char *str = &buf[1]; // Skips tab
            char *token = strtok(str, ":"); // Gets stripped transition token
            char *stripped = strip(token);
            
            token = strtok(NULL, "\r\n"); // Gets state name of transition
            while (*token == ' ') {token++;} // Skips leading spaces

            // Transition target state specified by its ID
            if (*token >= '0' && *token <= '9') {
                int target_state_id = strtol(token, NULL, 10);

                temp_trans_id = target_state_id;
                temp_trans_name = NULL;

                if (DEBUG == 1) {sprintf(print_str, "-> ID %d\n", target_state_id);}
            } 
            
            // Transition target state specified by its name
            else {
                temp_trans_name = malloc(sizeof(char) * (strlen(token) + 1));
                strcpy(temp_trans_name, token);
                temp_trans_id = -1;

                if (DEBUG == 1) {sprintf(print_str, "-> '%s'\n", token);}
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

                            // Add transitions within range
                            for (char c = c1; c <= c2; c++) {
                                
                                // Add Transition
                                add_temp_trans(&temp_trans_list, src, temp_trans_name, temp_trans_id, c);

                                if (DEBUG == 1) {
                                    // DEBUG: Print state transitions
                                    char temp[500];
                                    sprintf(temp, "Parsed state transition %c ", c);
                                    strcat(temp, print_str);
                                    printf("%s", temp);
                                }
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

                    // Add Transition
                    add_temp_trans(&temp_trans_list, src, temp_trans_name, temp_trans_id, *list_item);

                    if (DEBUG == 1) {
                        // DEBUG: Print transition
                        char temp[500];
                        sprintf(temp, "Parsed state transition %c ", *list_item);
                        strcat(temp, print_str);
                        printf("%s", temp);
                    }

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

                    // Add Transition
                    add_temp_trans(&temp_trans_list, src, temp_trans_name, temp_trans_id, (char) ascii_code);

                    if (DEBUG == 1) {
                        // DEBUG: Print Transition
                        char temp[500];
                        sprintf(temp, "Parsed state transition %d ", ascii_code);
                        strcat(temp, print_str);
                        printf("%s", temp);
                    }

                    list_item = strtok(NULL, ","); // Next number
                }
            } 
            
            // Single character
            else {
                char trans_chr = *stripped; 

                // Add Transition
                add_temp_trans(&temp_trans_list, src, temp_trans_name, temp_trans_id, trans_chr);

                if (DEBUG == 1) {
                    // DEBUG: Print state transition
                    char temp[500];
                    sprintf(temp, "Parsed state transition %c ", trans_chr);
                    strcat(temp, print_str);
                    printf("%s", temp);
                }

            }
            
        }

        // Else: If not # or whitespace, we are dealing with an invalid line
        else if (strchr(" #\t\r\n\v\f", buf[0]) == NULL) {
            fprintf(stderr, "Invalid syntax on line %d\n", line_count);
            exit(1);
        }

        line_count++;
    }

    // Loop and add all transitions
    while (temp_trans_list != NULL) {
        DFSA *target;
        if (temp_trans_list->name != NULL) {
            target = ptr_from_name(dfsa, temp_trans_list->name);
            if (target == NULL) {
                fprintf(stderr, "Transition targets nonexistent state: '%s'\n", temp_trans_list->name);
                exit(1);
            }
        } else {
            target = ptr_from_id(dfsa, temp_trans_list->id);
            if (target == NULL) {
                fprintf(stderr, "Transition targets nonexistent state: ID %d\n", temp_trans_list->id);
                exit(1);
            }
        }

        add_transition(temp_trans_list->src, temp_trans_list->chr, target);

        temp_trans_list = temp_trans_list->next;
    }

    // Print final DFSA
    if (DEBUG == 1) {
        printf("\n\n\n");
        print_dfsa(dfsa);
    }

    return dfsa;
}
