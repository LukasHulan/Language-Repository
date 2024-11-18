#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "tokenizer.h"
#include "to_string.h"

int isPunctuation(char c) {
    char* punctuation = ":<>()[]{}+-*/!=&|";

    return (strchr(punctuation, c) != NULL);
}

tokenData* tokenize(char* input) {
    tokenData* result;

    // Guard statement to handle empty input
    if (!strcmp(input, "")) {
        result->index = 0;
        result->length = 0;
        result->tokens = malloc(sizeof(char*));

        return result;
    }

    char* str = normalizeWhitespace(input);
    int length = strlen(str);
    int tokensFound = 0;

    // Count tokens
    for (int i = 0; i < length; i++) {
        if (str[i] == ' ' || str[i] == '\0') {
            tokensFound ++;
        } else if (isPunctuation(str[i])) {
            if (i > 0 && str[i - 1] != ' ') {
                tokensFound ++;
            }

            if (str[i + 1] != '\0' && str[i + 1] != ' ') {
                tokensFound ++;
            }
        }
    }

    tokensFound++;

    // Initialize list of tokens
    char** tokenList = malloc(sizeof(char*) * tokensFound);

    // Populate list of tokens
    int tokenStart = 0;
    int currentChar = 0;
    int tokenCount = 0;
    while (tokenCount < tokensFound) {
        int tokenLength = 0;
        char* token = "";

        while (str[currentChar] != ' ' || str[currentChar] != '\0' || !isPunctuation(str[currentChar])) {
            tokenLength++;
            currentChar++;
        }

        if (isPunctuation(str[currentChar])) {
            if (currentChar > 0 && str[currentChar - 1] != ' ') {
                token = malloc(tokenLength + 1);

                currentChar = tokenStart;
                while (str[currentChar] != ' ' || str[currentChar] != '\0' || !isPunctuation(str[currentChar])) {
                    token[currentChar - tokenStart] = str[currentChar];
                    currentChar++;
                }

                token[tokenLength] = '\0';
                tokenList[tokenCount] = token;

                tokenCount++;
            }

            if (str[currentChar + 1] != '\0' && str[currentChar + 1] != ' ') {
                tokenLength = 1;
                token = malloc(tokenLength + 1);

                token[0] = str[currentChar];
                token[1] = '\0';

                tokenList[tokenCount] = token;

                tokenCount++;
            }

            currentChar++;
            tokenStart = currentChar;
        } else {
            token = malloc(tokenLength + 1);

            currentChar = tokenStart;
            while (str[currentChar] != ' ' || str[currentChar] != '\0' || !isPunctuation(str[currentChar])) {
                token[currentChar - tokenStart] = str[currentChar];
                currentChar++;
            }

            token[tokenLength] = '\0';
            tokenList[tokenCount] = token;

            tokenCount++;
            currentChar++;
            tokenStart = currentChar;
        }
    }

    result->index = 0;
    result->length = tokenCount;
    result->tokens = tokenList;

    return result;
}

int tkncmp(tokenData* first, tokenData* second) {
    if (first->length != second->length) {
        return 1;
    }

    for (int i = 0; i < first->length; i++) {
        if (strcmp(first->tokens[i], second->tokens[i])) {
            return 1;
        }
    }

    return 0;
}