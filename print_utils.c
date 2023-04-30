#include <stdio.h>
#include <string.h>
#include "print_utils.h"

void print_bin(char *str) {
    for (int i = 0; i < strlen(str); i++) {
        printf("%d ", str[i]);
    }
    printf("\n");
}
