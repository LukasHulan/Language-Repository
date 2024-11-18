#include <stdio.h> 
#include <stdlib.h>
#include <string.h>

#include "parse_tree.h"
#include "to_string.h"
#include "tokenizer.h"
#include "test_harness.h"

enum testType {COMPARE_STRINGS, COMPARE_TREES, TEST_PARSE, TEST_PARSE_AGINST_TREE, 
               TEST_REPRESNTATION, TEST_TO_STRING, COMPARE_INTS, COMPARE_TOKENS};

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

        struct {
            int first;
            int second;
        } cmprInts;

        struct {
            tokenData* first;
            tokenData* second;
        } cmprTkns;
    };
} testData;

int runTests(testData* tests[], int numTests) {
    int testsFailed = 0;
    int errors = 0;

    printf("Running tests: %d\n", numTests);
    
    for (int testCounter = 0; testCounter < numTests; testCounter++) {
        testData* current = tests[testCounter];

        switch (current->type) {
            case COMPARE_STRINGS:
                if (strcmp(current->cmprStrs.first, current->cmprStrs.second)) {
                    testsFailed++;
                    printf("Failed test: %d\n", (testCounter + 1));
                }

                break;
            case COMPARE_TREES:
                if (ptcmp(current->cmprPTs.first, current->cmprPTs.second)) {
                    testsFailed++;
                    printf("Failed test: %d\n", (testCounter + 1));
                }

                break;
            case TEST_PARSE:
                if (1 /*To do*/) {
                    testsFailed++;
                    printf("Failed test: %d\n", (testCounter + 1));
                }

                break;
            case TEST_PARSE_AGINST_TREE:
                if (1 /*To do*/) {
                    testsFailed++;
                    printf("Failed test: %d\n", (testCounter + 1));
                }

                break;
            case TEST_REPRESNTATION:
                if (strcmp(parseTreeRepr(current->testRepr.tree), current->testRepr.repr)) {
                    testsFailed++;
                    printf("Failed test: %d\n", (testCounter + 1));
                }

                break;
            case TEST_TO_STRING:
                if (1 /*To do*/) {
                    testsFailed++;
                    printf("Failed test: %d\n", (testCounter + 1));
                }

                break;
            case COMPARE_INTS:
                if (current->cmprInts.first != current->cmprInts.second) {
                    testsFailed++;
                    printf("Failed test: %d\n", (testCounter + 1));
                }

                break;
            case COMPARE_TOKENS:
                if (!tkncmp(current->cmprTkns.first, current->cmprTkns.second)) {
                    testsFailed++;
                    printf("Failed test: %d\n", (testCounter + 1));
                }

                break;
            default:
                errors++;
                printf("Error at test %d: invalid or unset type\n", (testCounter + 1));
        }
    }

    printf("\n");
    printf("Tests passed: %d\n", (numTests - testsFailed - errors));
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

    multipleStatements_s* multiPassDef = malloc(sizeof(multipleStatements_s));
    multiPassDef->tag = MULTI;
    multiPassDef->multi_t.first = passStmnt;
    multiPassDef->multi_t.rest = defMultStmnt;

    parseTree* ptPassStmnt = malloc(sizeof(parseTree));
    ptPassStmnt->type = STATEMENT;
    ptPassStmnt->tree.statementValue = passStmnt;

    parseTree* ptIfStmnt = malloc(sizeof(parseTree));
    ptIfStmnt->type = PROGRAM;
    ptIfStmnt->tree.programValue = ifStmntRoot;

    parseTree* ptMultiPassDef = malloc(sizeof(parseTree));
    ptMultiPassDef->type = MULTIPLE_STATEMENTS;
    ptMultiPassDef->tree.multipleStatementsValue = multiPassDef;

    testData* reprTest1 = malloc(sizeof(testData));
    reprTest1->type = TEST_REPRESNTATION;
    reprTest1->testRepr.tree = ptIfStmnt;
    reprTest1->testRepr.repr = "Program: [MultipleStatements(single): [Statement(if): [If(if_only): [Expression(boolean): [Boolean(true)], Statement(define): [BindingIdentifier(not typed) [Identifier: [x]], Expression(numeric): [Numeric(number): [NumberData(integer): [5]]]]]]]]";

    testData* reprTest2 = malloc(sizeof(testData));
    reprTest2->type = TEST_REPRESNTATION;
    reprTest2->testRepr.tree = ptPassStmnt;
    reprTest2->testRepr.repr = "Statement(pass)";

    testData* reprTest3 = malloc(sizeof(testData));
    reprTest3->type = TEST_REPRESNTATION;
    reprTest3->testRepr.tree = ptMultiPassDef;
    reprTest3->testRepr.repr = "MultipleStatements(multi): [Statement(pass), MultipleStatements(single): [Statement(define): [BindingIdentifier(not typed) [Identifier: [x]], Expression(numeric): [Numeric(number): [NumberData(integer): [5]]]]]]";

    testData* normTest1 = malloc(sizeof(testData));
    normTest1->type = COMPARE_STRINGS;
    normTest1->cmprStrs.first = normalizeWhitespace("");
    normTest1->cmprStrs.second = "";

    testData* normTest2 = malloc(sizeof(testData));
    normTest2->type = COMPARE_STRINGS;
    normTest2->cmprStrs.first = normalizeWhitespace("Hello\nworld");
    normTest2->cmprStrs.second = "Hello world";

    testData* normTest3 = malloc(sizeof(testData));
    normTest3->type = COMPARE_STRINGS;
    normTest3->cmprStrs.first = normalizeWhitespace("Hello\tworld");
    normTest3->cmprStrs.second = "Hello world";

    testData* normTest4 = malloc(sizeof(testData));
    normTest4->type = COMPARE_STRINGS;
    normTest4->cmprStrs.first = normalizeWhitespace(" Hello world");
    normTest4->cmprStrs.second = "Hello world";

    testData* normTest5 = malloc(sizeof(testData));
    normTest5->type = COMPARE_STRINGS;
    normTest5->cmprStrs.first = normalizeWhitespace("Hello world ");
    normTest5->cmprStrs.second = "Hello world";

    testData* normTest6 = malloc(sizeof(testData));
    normTest6->type = COMPARE_STRINGS;
    normTest6->cmprStrs.first = normalizeWhitespace(" \n\t  Hello\n\t\n \n world  \t");
    normTest6->cmprStrs.second = "Hello world";

    testData* pnctTest1 = malloc(sizeof(testData));
    pnctTest1->type = COMPARE_INTS;
    pnctTest1->cmprInts.first = isPunctuation(' ');
    pnctTest1->cmprInts.second = 0;

    testData* pnctTest2 = malloc(sizeof(testData));
    pnctTest2->type = COMPARE_INTS;
    pnctTest2->cmprInts.first = isPunctuation(':');
    pnctTest2->cmprInts.second = 1;

    tokenData* emptyToken = malloc(sizeof(tokenData));
    emptyToken->index = 0;
    emptyToken->length = 0;
    emptyToken->tokens = malloc(sizeof(char*));

    testData* tknsTest1 = malloc(sizeof(testData));
    tknsTest1->type = COMPARE_TOKENS;
    tknsTest1->cmprTkns.first = tokenize("");
    tknsTest1->cmprTkns.second = emptyToken;

    tokenData* helloWorldToken = malloc(sizeof(tokenData));
    helloWorldToken->index = 0;
    helloWorldToken->length = 3;
    char* strings[] = {"Hello", ":", "world"};
    helloWorldToken->tokens = strings;

    testData* tknsTest2 = malloc(sizeof(testData));
    tknsTest2->type = COMPARE_TOKENS;
    tknsTest2->cmprTkns.first = tokenize("Hello: world");
    tknsTest2->cmprTkns.second = helloWorldToken;

    // Compile array of data to test

    testData* tests[] = {
        reprTest1, reprTest2, reprTest3, 
        normTest1, normTest2, normTest3, normTest4, normTest5, normTest6,
        pnctTest1, pnctTest2,
        tknsTest1, tknsTest2
    };

    // Run the tests
    
    runTests(tests, 11);
}