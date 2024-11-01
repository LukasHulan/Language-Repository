#include <stdio.h> 
#include <stdlib.h>
#include <string.h>

#include <parse_tree.h>
#include <to_string.h>
#include <test_harness.h>

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
    
    for (int testCounter = 0; testCounter < numTests; testCounter++) {
        testData* current = tests[testCounter];

        switch (current->type) {
            case COMPARE_STRINGS:
                if (!strcmp(current->cmprStrs.first, current->cmprStrs.second)) {
                    testsFailed++;
                    printf("Test failed: %d\n", testCounter);
                }

                break;
            case COMPARE_TREES:
                if (!ptcmp(current->cmprPTs.first, current->cmprPTs.second)) {
                    testsFailed++;
                    printf("Test failed: %d\n", testCounter);
                }

                break;
            case TEST_PARSE:
                if (1 /*To do*/) {
                    testsFailed++;
                    printf("Test failed: %d\n", testCounter);
                }

                break;
            case TEST_PARSE_AGINST_TREE:
                if (1 /*To do*/) {
                    testsFailed++;
                    printf("Test failed: %d\n", testCounter);
                }

                break;
            case TEST_REPRESNTATION:
                if (1 /*To do*/) {
                    testsFailed++;
                    printf("Test failed: %d\n", testCounter);
                }

                break;
            case TEST_TO_STRING:
                if (1 /*To do*/) {
                    testsFailed++;
                    printf("Test failed: %d\n", testCounter);
                }

                break;
            default:
                errors++;
                printf("Error at test %d: invalid or unset type\n", testCounter);
        }
    }

    printf("Tests passed: %d\n", numTests - testsFailed);
    printf("Tests failed: %d\n", testsFailed);

    if (errors > 0) {
        printf("Errors: %d\n", errors);
    }

    return testsFailed > 0;
}

void unitTesting() {
    // Declare data for tests

    // Compile array of data to test
    testData tests[1];

    // Run the tests
    runTests(tests, 0);
}