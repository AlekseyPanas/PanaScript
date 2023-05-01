#define MAX_DFSA_LINE_LENGTH 100  // Max length of string array to store a DFSA file line (99 chars, 1 null term)

/*
Node in a transitions linked list. chr defines a character which triggers
the transition, target is a pointer to the target transition state, and
next is a pointer to the next transition in the list
*/
typedef struct transition {
    char chr;
    struct dfsa *target;
    struct transition *next;
} Trans;

/*
id is an int id for the state, name is a string name for the state, and
transitions is a list of transitions from this state. Next points to the next state in the list
*/
typedef struct dfsa {
    int id;
    char *name;
    struct transition *transitions;
    struct dfsa *next;
} DFSA;


/*
Given an 'r' mode file stream for a file in the .dfsa format, return a pointer to the processed
DFSA struct of that file
*/
DFSA *parse_dfsa(FILE* file);

/*
Return a pointer to the DFSA state identified by id. Return NULL if state doesn't exist
*/
DFSA *ptr_from_id(DFSA *head, int id);

/*
Return a pointer to the DFSA state the requested name. Return NULL if state doesn't exist
*/
DFSA *ptr_from_name(DFSA *head, char *name);

/*
From current state cur, return pointer to new state resulting from transition with character c. Return
NULL if c has no transition (implies garbage state)
*/
DFSA *eval_trans(DFSA *cur, char c);
