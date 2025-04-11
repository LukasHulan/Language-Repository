#ifndef EXECUTOR
#define EXECUTOR

#include <stdlib.h>

#include "parse_tree.h"

enum scopeTags {
    INITIAL,
    FUNCTION,
    NOT_FUNCTION
};

enum bindingTags {
    BOUND,
    NOT_BOUND
};

typedef datum;

// Tags: BOUND, NOT_BOUND
typedef struct bindingList {
    enum bindingTags tag;
    char* identifier;
    datum_s* value;
    bindingList* next;
} bindingList;

// Tags: INITIAL, FUNCTION, NOT_FUNCTION
typedef struct scopeFrame {
    enum scopeTags tag;
    bindingList* bindings;
    scopeFrame* previous;
} scopeFrame;

typedef struct function_s {
    bindingList* parameters;
    parseTree* pt;
} function_s;

// Tags: NUMBER, CHARACTER, FUNCTION
typedef struct datum_s {
    enum tags tag;
    union {
        numberData_s* number;
        character_s* character;
        function_s* function;
    };
} datum_s;

extern scopeFrame* topScope;

void freeDatum(datum_s* value);
void freeBindings(bindingList* bindings);
void popScope();

scopeFrame* initialScope();

datum_s* execute(parseTree* pt);

#endif