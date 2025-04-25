#ifndef EXECUTOR
#define EXECUTOR

#include <stdlib.h>

#include "parse_tree.h"

enum scopeTags {
    INITIAL,
    IS_FUNCTION,
    NOT_FUNCTION
};

enum bindingTags {
    BOUND,
    NOT_BOUND
};

struct datum_s;

// Tags: BOUND, NOT_BOUND
typedef struct bindingList {
    enum bindingTags tag;
    int isTyped;
    char* identifier;
    char* type;
    struct datum_s* value;
    struct bindingList* next;
} bindingList;

// Tags: INITIAL, IS_FUNCTION, NOT_FUNCTION
typedef struct scopeFrame {
    enum scopeTags tag;
    bindingList* bindings;
    struct scopeFrame* previous;
} scopeFrame;

typedef struct function_s {
    bindingList* parameters;
    parseTree* pt;
} function_s;

// Tags: EMPTY, NOT_EMPTY
typedef struct datumList_s {
    enum tags tag;
    datum_s* first;
    struct datumList_s* rest;
} datumList_s;

// Tags: NUMBER, CHARACTER, FUNCTION, NONE
typedef struct datum_s {
    enum tags tag;
    union {
        numberData_s* number;
        character_s* character;
        function_s* function;
        datumList_s* list;
    };
} datum_s;

extern scopeFrame* topScope;

void freeDatum(datum_s* value);
void freeBindings(bindingList* bindings);
void popScope();

scopeFrame* initialScope();

datum_s* evalFunc(char* id, datumList_s* args);
datum_s* exeFunc(char* id, datumList_s* args);

datum_s* execute(parseTree* pt);
datum_s* executeMultiSmnt(multipleStatements_s* pt);
datum_s* executeStmnt(statement_s* pt);
datum_s* executeExpr(expression_s* pt);
datum_s* executeList(list_s* pt);
datum_s* executeMultiExpr(multipleExpressions_s* pt);
datum_s* executeFuncDclr(functionDeclaration_s* pt);
datum_s* executeNumData(numberData_s* pt);

datum_s* evalNumericOp(numeric_s*);

#endif