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
transitions is a list of transitions from this state
*/
typedef struct dfsa {
    int id;
    char *name;
    struct transition *transitions;
} DFSA;
