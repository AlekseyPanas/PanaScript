#define MAX_LINE_SIZE 100   // Max number of characters in a line of final MIPs code

/*
0 = STMTS, value = NULL
1 = ASSIGN, value = NULL
2 = REASSIGN, value = NULL
3 = FN_CALL, value = NULL
4 = FN_DEF, value = NULL
5 = WHILE, value = NULL
6 = IF, value = NULL
7 = ELIF, value = NULL
8 = ELSE, value = NULL
9 = RETURN, value = NULL
10 = OP, value in {">", "<", "&", "+", "-", ...}
11 = TYPE LITERAL, value in {"b", "w", "h"}
12 = STRING LITERAL, value = [A-Za-z][A-Za-z0-9]*
13 = NUMBER LITERAL, value = 0x[0-9]^(2-8)
14 = VARIABLE LITERAL, value = [A-Za-z][A-Za-z0-9]*
*/
typedef unsigned int node_type;

/*
node_type indicates the type of node we are at. Each node type may or may not have an associated value,
and indicates a specific expected children structure. Refer to associated DFSA Google Drawings diagram
*/
typedef struct syntax_tree_node {
    node_type type;
    char *value;
    struct syntax_tree_node **children;

} ASTnode;

/*
Stores lines of compiled MIPs code in memory. Loop and write them to a file one-by-one
*/
typedef struct string_linked_list {
    char line[MAX_LINE_SIZE];
    struct string_linked_list *next;
} LinkedList;

/*
Given an open file stream in read binary mode, parse the file into an abstract syntax tree and return a 
pointer to the root node. Exit with error code 1 if a syntax error occurred and print user friendly error 
information to stderr
*/
ASTnode *parse(FILE* file);

/*
Given a pointer to the root of an abstract syntax tree, write the final compiled MIPs assembly code to a LinkedList.
Return a pointer to thgis linked list. If an error was caught, print a friendly error message to stderr and exit
with error code 1
*/
LinkedList* compile(ASTnode *root);
