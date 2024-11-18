#ifndef TOKENIZER
#define TOKENIZER

typedef struct tokenData {
    int index;
    int length;
    char** tokens;
} tokenData;

tokenData* tokenize(char* input);
int isPunctuation(char c);
int tkncmp(tokenData* first, tokenData* second);

#endif