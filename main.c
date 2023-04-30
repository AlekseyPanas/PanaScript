#include <stdio.h>

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
    ASTnode **children;

} ASTnode;

int main(int argc, char**args) {

    printf("%d", '\n');

    return 0;
}
