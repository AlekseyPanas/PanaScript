#include <stdio.h>
#include "shared.h"

int main(int argc, char**argv) {

    if (argc == 2 || argc == 3) {
    
        char *out_filename;
        if (argc == 3) {
            out_filename = argv[2];
        } else {
            out_filename = "output.asm";
        }

        // Open file and error check
        FILE *file = fopen(argv[1], "rb");
        if (file == NULL) {
            fprintf(stderr, "File does not exist!\n");
            return 1;
        }

        // Parse and compile
        ASTnode *ast_root = parse(file);
        LinkedList *line = compile(ast_root);
        fclose(file);

        // Write to file
        FILE *out = fopen(out_filename, "w");
        if (out == NULL) {
            fprintf(stderr, "Failed to generate output file!\n");
            return 1;
        }

        while (line != NULL) {
            fprintf(out, "%s", line->line);

            line = line->next;
        }

        fclose(out);

        return 0;
    } else {
        fprintf(stderr, "Too many or too little arguments!\n\nCommand Syntax:\npanascript <filename> [output filename]\n");
        return 1;
    }
}
