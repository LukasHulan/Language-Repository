#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "tokenizer.h"
#include "to_string.h"

int currentIndex = 0;

char* nextToken(char* input) {
    if (input[currentIndex] == '\0') {
        return NULL;
    }

    char* rtrnToken;

    if (isPunctuation(input[currentIndex])) {
        rtrnToken = malloc(2);
        rtrnToken[0] = input[currentIndex];
        rtrnToken[1] = '\0';

        currentIndex++;

        if (input[currentIndex] == ' ') {
            currentIndex++;
        }

        return rtrnToken;
    }

    int counter = 0;
    int startPoint = currentIndex;
    while (input[currentIndex] != '\0' && input[currentIndex] != ' ' && !isPunctuation(input[currentIndex])) {
        counter++;
        currentIndex++;
    }
    currentIndex = startPoint;

    rtrnToken = malloc(counter + 1);
    for (int i = 0; i < counter; i++) {
        rtrnToken[i] = input[currentIndex];
        currentIndex++;
    }
    rtrnToken[counter] = '\0';

    if (input[currentIndex] == ' ') {
        currentIndex++;
    }

    return rtrnToken;
}

tokenData* tokenize(char* input) {
    tokenData* result = malloc(sizeof(tokenData));
    result->index = 0;
    result->length = 0;

    char* str = normalizeWhitespace(input);

    if (!strcmp(str, "")) {
        result->tokens = malloc(sizeof(char*));
        return result;
    }

    while (nextToken(str) != NULL) {
        result->length++;
    }
    currentIndex = 0;

    char** tokenList = malloc(sizeof(char*) * result->length);
    tokenList[result->length] = '\0';

    char* token;
    for (int i = 0; (token = nextToken(str)) != NULL; i++) {
        tokenList[i] = token;
    }

    result->tokens = tokenList;
    currentIndex = 0;
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

// Returns a copy of the token at the target index
char* getToken(tokenData* tknData, int target) {
    int length = strlen(tknData->tokens[target]);
    char* token = malloc(length + 1);

    strncpy(token, tknData->tokens[target], length);
    token[length] = '\0';

    return token;
}

// Returns a copy of the current token
char* peekToken(tokenData* tknData) {
    char* token = getToken(tknData, tknData->index);

    return token;
}

// Returns a copy of the current token and moves the token index
char* popToken(tokenData* tknData) {
    if (tknData->index == tknData->length) {
        return "\0";
    }

    char* token = getToken(tknData, tknData->index);

    tknData->index++;

    return token;
}