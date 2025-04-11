#include "executor.h"
#include "parse_tree.h"

void freeDatum(datum_s* value) {
    switch (value->tag) {
        case NUMBER:
            free(value->number);
            break;
        case CHARACTER:
            free(value->character);
            break;
        case FUNCTION:
            free(value->function);
            break;
        default:
            println("Error freeing datum: invalid or unset tag.");
    }

    free(value);
}

void freeBindings(bindingList* bindings) {
    if (bindings->next != NULL) {
        freeBindings(bindings->next);
    }

    freeDatum(bindings->value);
    free(bindings->identifier);
    free(bindings);
}

void popScope() {
    scopeFrame* temp = topScope;
    topScope = topScope->previous;
    free(temp);
}

scopeFrame* initialScope() {
    scopeFrame* rtrn = malloc(sizeof(scopeFrame));
    rtrn->tag = INITIAL;
    rtrn->bindings = malloc(sizeof(bindingList));
    rtrn->previous = NULL;

    return rtrn;
}

datum execute(parseTree* pt) {

}