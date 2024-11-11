#include <stdio.h> 
#include <stdlib.h>
#include <string.h>

#include "parse_tree.h"
#include "to_string.h"
#include "test_harness.h"

enum testType {COMPARE_STRINGS, COMPARE_TREES, TEST_PARSE, TEST_PARSE_AGINST_TREE, 
               TEST_REPRESNTATION, TEST_TO_STRING};

typedef struct testData {
    enum testType type;
    union {
        struct {
            char* first;
            char* second;
        } cmprStrs;

        struct {
            parseTree* first;
            parseTree* second;
        } cmprPTs;

        struct {
            char* code;
        } testParse;

        struct {
            char* code;
            parseTree* result;
        } testPT;

        struct {
            parseTree* tree;
            char* repr;
        } testRepr;

        struct {
            parseTree* tree;
            char* code;
        } testToStr;
    };
} testData;

int runTests(testData* tests[], int numTests) {
    int testsFailed = 0;
    int errors = 0;

    printf("Running tests: %d\n", numTests);
    
    for (int testCounter = 1; testCounter <= numTests; testCounter++) {
        testData* current = tests[testCounter];

        switch (current->type) {
            case COMPARE_STRINGS:
                if (strcmp(current->cmprStrs.first, current->cmprStrs.second)) {
                    testsFailed++;
                    printf("Failed test: %d\n", testCounter);
                }

                break;
            case COMPARE_TREES:
                if (ptcmp(current->cmprPTs.first, current->cmprPTs.second)) {
                    testsFailed++;
                    printf("Failed test: %d\n", testCounter);
                }

                break;
            case TEST_PARSE:
                if (1 /*To do*/) {
                    testsFailed++;
                    printf("Failed test: %d\n", testCounter);
                }

                break;
            case TEST_PARSE_AGINST_TREE:
                if (1 /*To do*/) {
                    testsFailed++;
                    printf("Failed test: %d\n", testCounter);
                }

                break;
            case TEST_REPRESNTATION:
                if (strcmp(parseTreeRepr(current->testRepr.tree), current->testRepr.repr)) {
                    testsFailed++;
                    printf("Failed test: %d\n", testCounter);
                }

                break;
            case TEST_TO_STRING:
                if (1 /*To do*/) {
                    testsFailed++;
                    printf("Failed test: %d\n", testCounter);
                }

                break;
            default:
                errors++;
                printf("Error at test %d: invalid or unset type\n", testCounter);
        }
    }

    printf("\n");
    printf("Tests passed: %d\n", numTests - testsFailed);
    printf("Tests failed: %d\n", testsFailed);

    if (errors > 0) {
        printf("Errors: %d\n", errors);
    }

    return testsFailed > 0;
}

void unitTesting() {
    // Declare data for tests
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

    // pass
    statement_s* passStmnt = malloc(sizeof(statement_s));
    passStmnt->tag = PASS;
    passStmnt->pass_t.placeholder = 1;

    parseTree* ptIfStmnt = malloc(sizeof(parseTree));
    ptIfStmnt->type = PROGRAM;
    ptIfStmnt->tree.programValue = ifStmntRoot;

    // printf("%s", parseTreeRepr(ptIfStmnt));

    testData* reprTest1 = malloc(sizeof(testData));
    reprTest1->type = TEST_REPRESNTATION;
    reprTest1->testRepr.tree = ptIfStmnt;
    reprTest1->testRepr.repr = "Program: [MultipleStatements(single): [Statement(if): [If(if_only): [Expression(boolean): [Boolean(true)], Statement(define): [FunctionDeclaration(not typed) [Identifier: [x]], Expression(numeric): [Numeric(number): [NumberData(integer): [5]]]]]]]]";

    // Compile array of data to test
    testData* tests[1] = {reprTest1};

    // Run the tests
    runTests(tests, 1);
}