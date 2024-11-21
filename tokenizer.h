#ifndef TOKENIZER
#define TOKENIZER

typedef struct tokenData {
    int index;
    int length;
    char** tokens;
} tokenData;

tokenData* tokenize(char* input);

int tkncmp(tokenData* first, tokenData* second);

char* getToken(tokenData* tknData, int target);
char* peekToken(tokenData* tknData);
char* popToken(tokenData* tknData);

#endif