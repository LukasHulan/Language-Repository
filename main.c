#include <stdio.h> 
#include <stdlib.h>
#include <string.h>

#include <parse_tree.h>
#include <to_string.h>
#include <test_harness.h>

int main() {
    // "define x: 5"
    identifier_s* defStmntId = malloc(sizeof(identifier_s));
    defStmntId->identifier = "x";
    bindingIdentifier_s* defStmntBid = malloc(sizeof(bindingIdentifier_s));
    defStmntBid->tag = NOT_TYPED;
    defStmntBid->not_typed_t.id = defStmntId;
    numberData_s* fiveInt = malloc(sizeof(numberData_s));
    fiveInt->type = INT;
    fiveInt->integer = 5;
    numeric_s* numFive = malloc(sizeof(numeric_s));
    numFive->tag = NUMBER;
    numFive->number_t.num = fiveInt;
    expression_s* numExpr = malloc(sizeof(expression_s));
    numExpr->tag = NUMERIC;
    numExpr->numeric_t.num = numFive;
    statement_s* defStmnt = malloc(sizeof(statement_s));
    defStmnt->tag = DEFINE;
    defStmnt->define_t.bid = defStmntBid;
    defStmnt->define_t.expr = numExpr;
    multipleStatements_s* defMultStmnt = malloc(sizeof(multipleStatements_s));
    defMultStmnt->tag = SINGLE;
    defMultStmnt->single_t.stmnt = defStmnt;
    program_s* defStmntRoot = malloc(sizeof(program_s));
    defStmntRoot->stmnts = defMultStmnt;

    // "if true: def x: 5"
    boolean_s* boolTrue = malloc(sizeof(boolean_s));
    boolTrue->tag = TRUE;
    boolTrue->true_t.placeholder = 1;
    expression_s* boolExpr = malloc(sizeof(expression_s));
    boolExpr->tag = BOOLEAN;
    boolExpr->boolean_t.bool = boolTrue;
    if_s* ifCond = malloc(sizeof(if_s));
    ifCond->tag = IF_ONLY;
    ifCond->if_only_t.ifExpr = boolExpr;
    ifCond->if_only_t.ifStmnt = defStmnt;
    statement_s* ifStmnt = malloc(sizeof(statement_s));
    ifStmnt->tag = IF;
    ifStmnt->if_t.stmnt = ifCond;
    multipleStatements_s* ifMultStmnt = malloc(sizeof(multipleStatements_s));
    ifMultStmnt->tag = SINGLE;
    ifMultStmnt->single_t.stmnt = ifStmnt;
    program_s* ifStmntRoot = malloc(sizeof(program_s));
    ifStmntRoot->stmnts = ifMultStmnt;

    printf("%s", numberDataRepr(fiveInt));

    statement_s* passStmnt = malloc(sizeof(statement_s));
    passStmnt->tag = PASS;
    passStmnt->pass_t.placeholder = 1;

    // printf("%s", statementRepr(passStmnt));

    return 0;
}