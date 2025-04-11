#include <stdio.h> 
#include <stdlib.h>
#include <string.h>

#include "parse_tree.h"
#include "to_string.h"
#include "test_harness.h"
#include "tokenizer.h"
#include "parser.h"
#include "executor.h"

scopeFrame* topScope;

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

    if (givenFile) {
        FILE* file;
        if ((file = fopen(fileName, "rb")) != NULL) {
            fseek(file, 0L, SEEK_END);
            long length = ftell(file);
            rewind(file);

            char* buffer = malloc(length + 1);

            if (buffer) {
                fread(buffer, 1, length, file);
            }
            buffer[length] = '\0';

            fclose(file);

            if (buffer) {
                if (toDebug) printf("Tokenizing...\n");
                tokenData* tkns = tokenize(buffer);
                
                free(buffer);

                if (toDebug) {
                    printf("Tokens: ");
                    for (int i = 0; i < tkns->length; i++) {
                        printf("\"%s\" ", tkns->tokens[i]);
                    }
                    printf("\n");
                }

                if (toDebug) printf("Parsing...\n");
                parseTree* pt = parse(tkns);

                for (int i = 0; i < tkns->length; i++) {
                    free(tkns->tokens[i]);
                }
                free(tkns);

                if (toDebug) {
                    printf("Representation:\n");
                    printf("%s\n", parseTreeRepr(pt));
                }

                topScope = initialScope();

                execute(pt);
            } else {
                printf("Could not read file.");
            }
        } else {
            printf("Could not find a file named \"%s\".\n", fileName);
        }
    } else {
        printf("No file provided.\n");
    }
}