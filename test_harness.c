#include <test_harness.h>

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