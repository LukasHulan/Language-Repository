#include <stdio.h> 
#include <stdlib.h>
#include <string.h>

#include "parse_tree.h"
#include "to_string.h"
#include "test_harness.h"
#include "tokenizer.h"
#include "parser.h"

int main(int argc, char* argv[]) {
    int toTest = 0;
    int toDebug = 0;
    int givenFile = 0;
    char* fileName;

    if (argc >= 2) {
        for (int i = 1; i < argc; i++) {
            if (strlen(argv[i]) > 4 && !strcmp(((char*) argv[i] + strlen(argv[i]) - 4), ".txt")) {
                givenFile = 1;
                fileName = argv[i];
            } else if (!strcmp(argv[i], "-d") || !strcmp(argv[i], "--debug")) {
                toDebug = 1;
            } else if (!strcmp(argv[i], "-t") || !strcmp(argv[i], "--test")) {
                toTest = 1;
            } else {
                printf("%s is not a valid parameter.\n", argv[i]);
            }
        }
    } else {
        printf("No parameters.\n");
    }

    if (toTest) {
        unitTesting();
    }

    if (toDebug) {
        if (givenFile) {
            FILE* file;
            if ((file = fopen(fileName, "r")) != NULL) {
                fseek(file, 0L, SEEK_END);
                long length = ftell(file);
                rewind(file);

                char* buffer = malloc(length + 1);
                buffer[length] = '\0';

                if (buffer) {
                    fread(buffer, 1, length, file);
                }

                fclose(file);

                if (buffer) {
                    printf("Tokenizing...\n");
                    tokenData* tkns = tokenize(buffer);

                    printf("Tokens: ");
                    for (int i = 0; i < tkns->length; i++) {
                        printf("\"%s\" ", tkns->tokens[i]);
                    }
                    printf("\n");

                    printf("Parsing...\n");
                    parseTree* pt = parse(tkns);

                    printf("Representation:\n");
                    printf("%s\n", parseTreeRepr(pt));
                }
            } else {
                printf("Could not find a file named \"%s\".\n", fileName);
            }
        } else {
            printf("No file provided.\n");
        }
    }
}