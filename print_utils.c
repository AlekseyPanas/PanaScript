#include <stdio.h>
#include <string.h>
#include "print_utils.h"

void print_bin(char *str) {
    for (int i = 0; i < strlen(str); i++) {
        printf("%d ", str[i]);
    }
    printf("\n");
}

char *strip(char *s) {
    while (*s == ' ') {s++;}
    while (s[strlen(s) - 1] == ' ') {s[strlen(s) - 1] = '\0';}
    return s;
}

int char_in(char c, char *s) {
    for (int i = 0; i < strlen(s); i++) {
        if (s[i] == c) {
            return 1;
        }
    }
    return 0;
}
