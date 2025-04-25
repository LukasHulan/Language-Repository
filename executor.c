#include <stdio.h> 
#include <stdlib.h>
#include <string.h>

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
            printf("Error freeing datum: invalid or unset tag.");
    }

    free(value);
}

void freeBindings(bindingList* bindings) {
    if (bindings->next != NULL) {
        freeBindings(bindings->next);
    }

    freeDatum(bindings->value);
    free(bindings->identifier);
    free(bindings->type);
    free(bindings);
}

void popScope() {
    scopeFrame* temp = topScope;
    topScope = topScope->previous;
    freeBindings(temp->bindings);
    free(temp);
}

scopeFrame* initialScope() {
    scopeFrame* rtrn = malloc(sizeof(scopeFrame));
    rtrn->tag = INITIAL;
    rtrn->bindings = malloc(sizeof(bindingList));
    rtrn->previous = NULL;

    return rtrn;
}

// 1 if function; 0 if not a function
void addScope(int isFunction) {
    scopeFrame* newFrame = malloc(sizeof(scopeFrame));
    newFrame->tag = (isFunction) ? FUNCTION : NOT_FUNCTION;
    newFrame->bindings = malloc(sizeof(bindingList));
    newFrame->previous = topScope;

    topScope = newFrame;
}

int compareNums(datum_s* value1, datum_s* value2) {
    if (value1->tag == NUMBER && value2->tag == NUMBER) {
        if (value1->number->type == INT) {
            if (value2->number->type == INT) {
                if (value1->number->integer == value2->number->integer) {
                    return 0;
                } else if (value1->number->integer > value2->number->integer) {
                    return 1;
                } else {
                    return -1;
                }
            } else {
                if (value1->number->integer == value2->number->decimal) {
                    return 0;
                } else if (value1->number->integer > value2->number->decimal) {
                    return 1;
                } else {
                    return -1;
                }
            }
        } else {
            if (value2->number->type == INT) {
                if (value1->number->decimal == value2->number->integer) {
                    return 0;
                } else if (value1->number->decimal > value2->number->integer) {
                    return 1;
                } else {
                    return -1;
                }
            } else {
                if (value1->number->decimal == value2->number->decimal) {
                    return 0;
                } else if (value1->number->decimal > value2->number->decimal) {
                    return 1;
                } else {
                    return -1;
                }
            }
        }
    } else {
        printf("Error: invalid type; must be numbers to compare.");
        exit(0);
    }

    return 0;
}

int isTruthy(datum_s* value) {
    if (value->tag == NUMBER) {
        if (value->number->type == INT) {
            return value->number->integer == 0;
        } else {
            return value->number->decimal == 0.0;
        }
    }

    return 1;
}

void addBoundId(char* id, char* type, datum_s* value) {
    bindingList* newBinding = malloc(sizeof(bindingList));

    newBinding->tag = BOUND;
    newBinding->identifier = id;
    newBinding->isTyped = (type == NULL) ? 0 : 1;
    newBinding->type = type;
    newBinding->value = value;
    newBinding->next = topScope->bindings;

    topScope->bindings = newBinding;
}

void addUnboundId(char* id, char* type) {
    bindingList* newBinding = malloc(sizeof(bindingList));

    newBinding->tag = NOT_BOUND;
    newBinding->identifier = id;
    newBinding->isTyped = (type == NULL) ? 0 : 1;
    newBinding->type = type;
    newBinding->next = topScope->bindings;

    topScope->bindings = newBinding;
}

datum_s* findBinding(char* id) {
    scopeFrame* currentScope = topScope;
    bindingList* currentBinding = topScope->bindings;

    while (currentScope != NULL) {
        while (currentBinding != NULL) {
            if (!strcmp(currentBinding->identifier, id)) {
                return currentBinding->value;
            } else {
                currentBinding = currentBinding->next;
            }
        }

        currentScope = topScope->previous;
    }

    return NULL;
}

int bindingExists(char* id) {
    scopeFrame* currentScope = topScope;
    bindingList* currentBinding = topScope->bindings;

    while (currentScope != NULL) {
        while (currentBinding != NULL) {
            if (!strcmp(currentBinding->identifier, id)) {
                return 1;
            } else {
                currentBinding = currentBinding->next;
            }
        }

        currentScope = topScope->previous;
    }

    return 0;
}

int bindId(char* id, datum_s* value) {
    if (bindingExists(id)) {
        bindingList* binding = findBinding(id);

        if (binding->isTyped) {
            datumList_s* param = malloc(sizeof(datumList_s));
            param->tag = NOT_EMPTY;
            param->first = value;
            param->rest = NULL;
            
            datum_s* typeCheckValue = evalFunc(binding->type, param);

            if (typeCheckValue->tag == NUMBER && isTruthy(typeCheckValue)) {
                binding->value = value;
                return 1;
            } else {
                return 0;
            }
        } else {
            binding->value = value;
            return 1;
        }
    }

    return 0;
}

datumList_s* compileList(list_s* pt) {
    datumList_s* rtrn = malloc(sizeof(datumList_s));
    rtrn->tag = pt->tag;
    
    if (rtrn->tag == EMPTY) {
        rtrn->first = NULL;
        rtrn->rest = NULL;

        return rtrn;
    }

    datumList_s* currentNode = rtrn;
    multipleExpressions_s* currentExpr = pt->not_empty_t.exprs;

    while (currentExpr->tag != SINGLE) {
        currentNode->first = executeExpr(currentExpr->multi_t.first);

        currentNode->rest = malloc(sizeof(datumList_s));
        currentNode = currentNode->rest;
        currentNode->tag = NOT_EMPTY;

        currentExpr = currentExpr->multi_t.rest;
    }

    currentNode->first = executeExpr(currentExpr->single_t.expr);
    currentNode->rest = NULL;

    return rtrn;
}

datum_s* rtrnValue = NULL;

datum_s* execute(parseTree* pt) {
    if (pt->type == PROGRAM) {
        return executeMultiSmnt(pt->tree.programValue->stmnts);
    } else {
        printf("This should never hapen; something has gone horribly wrong.");
        exit(0);
    }
}

datum_s* executeMultiSmnt(multipleStatements_s* pt) {
    addScope(0);

    multipleStatements_s* current = pt;

    while (current->tag != SINGLE) {
        executeStmnt(current->multi_t.first);

        if (rtrnValue != NULL) {
            popScope();
            return NULL;
        }

        current = current->multi_t.rest;
    }

    executeStmnt(current->single_t.stmnt);

    popScope();
    return NULL;
}

datum_s* executeStmnt(statement_s* pt) {
    switch (pt->tag) {
        case BLOCK:
            executeMultiSmnt(pt->block_t.stmnts);

            break;
        case DEFINE:
            char* varId = (pt->define_t.bid->tag == TYPED) ? pt->define_t.bid->typed_t.id : pt->define_t.bid->not_typed_t.id;
            char* typeSpecifier = (pt->define_t.bid->tag == TYPED) ? pt->define_t.bid->typed_t.type : NULL;

            addBoundId(varId, typeSpecifier, executeExpr(pt->define_t.expr));

            break;
        case EXECUTE:
            exeFunc(pt->execute_t.id, compileList(pt->execute_t.list));
            break;
        case RETURN:
            rtrnValue = executeExpr(pt->return_t.expr);

            break;
        case IF:
            switch (pt->if_t.stmnt->tag) {
                case IF_ONLY:
                    if (isTruthy(pt->if_t.stmnt->if_only_t.ifExpr)) {
                        executeStmnt(pt->if_t.stmnt->if_only_t.ifStmnt);
                    }

                    break;
                case IF_ELSE:
                    if (isTruthy(pt->if_t.stmnt->if_else_t.ifExpr)) {
                        executeStmnt(pt->if_t.stmnt->if_else_t.ifStmnt);
                    } else {
                        executeStmnt(pt->if_t.stmnt->if_else_t.elseStmnt);
                    }
                    
                    break;
                case IF_ELSEIF:
                    if (isTruthy(pt->if_t.stmnt->if_elseif_t.ifExpr)) {
                        executeStmnt(pt->if_t.stmnt->if_elseif_t.ifStmnt);
                    } else {
                        elseIfChain_s* chain = pt->if_t.stmnt->if_elseif_t.chain;
                        int exprEvaluated = 0;

                        while (!exprEvaluated && chain->tag != SINGLE) {
                            if (isTruthy(chain->multi_t.ifExpr)) {
                                executeStmnt(chain->multi_t.ifStmnt);

                                if (rtrnValue != NULL) {
                                    return NULL;
                                }

                                exprEvaluated = 1;
                            }

                            chain = chain->multi_t.chain;
                        }

                        if (!exprEvaluated && chain->tag == SINGLE) {
                            if (isTruthy(chain->single_t.ifExpr)) {
                                executeStmnt(chain->single_t.ifStmnt);

                                if (rtrnValue != NULL) {
                                    return NULL;
                                }
                            }
                        }
                    }

                    break;
                case IF_ELSEIF_ELSE:
                    if (isTruthy(pt->if_t.stmnt->if_elseif_else_t.ifExpr)) {
                        executeStmnt(pt->if_t.stmnt->if_elseif_else_t.ifStmnt);
                    } else {
                        elseIfChain_s* chain = pt->if_t.stmnt->if_elseif_else_t.chain;
                        int exprEvaluated = 0;

                        while (!exprEvaluated && chain->tag != SINGLE) {
                            if (isTruthy(chain->multi_t.ifExpr)) {
                                executeStmnt(chain->multi_t.ifStmnt);

                                if (rtrnValue != NULL) {
                                    return NULL;
                                }

                                exprEvaluated = 1;
                            }

                            chain = chain->multi_t.chain;
                        }

                        if (!exprEvaluated && chain->tag == SINGLE) {
                            if (isTruthy(chain->single_t.ifExpr)) {
                                executeStmnt(chain->single_t.ifStmnt);

                                if (rtrnValue != NULL) {
                                    return NULL;
                                }

                                exprEvaluated = 1;
                            }
                        }

                        if (!exprEvaluated) {
                            executeStmnt(pt->if_t.stmnt->if_elseif_else_t.elseStmnt);
                        }
                    }

                    break;
                default:
                    break;
            }
            break;
        case WHILE:
            while (isTruthy(executeExpr(pt->while_t.expr))) {
                executeStmnt(pt->while_t.stmnt);

                if (rtrnValue != NULL) {
                    return NULL;
                }
            }

            break;
        case PASS:
            break;
        default:
            break;
    }

    return NULL;
}

datum_s* executeExpr(expression_s* pt) {
    switch (pt->tag) {
        case PARENTHESES:
            return executeExpr(pt->parentheses_t.expr);
        case IDENTIFIER:
            return findBinding(pt->identifier_t.id->identifier);
        case NUMERIC:
            numeric_s* numeric = pt->numeric_t.num;

            if (numeric->tag == NUMBER) {
                datum_s* num = malloc(sizeof(datum_s));
                num->tag = NUMBER;
                num->number = numeric->number_t.num;

                return num;
            } else {
                return evalNumericOp(pt->numeric_t.num);
            }
        default:
            break;
    }
}

datum_s* evalNumericOp(numeric_s* numOp) {
    if (numOp->tag != NUMERIC_OPERATOR) {
        return NULL;
    }

    datum_s* rtrnValue = malloc(sizeof(datum_s));
    rtrnValue->tag = NUMBER;

    datum_s* num1 = executeExpr(numOp->numeric_operator_t.num1);
    datum_s* num2 = executeExpr(numOp->numeric_operator_t.num2);

    
}