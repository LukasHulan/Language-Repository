#ifndef TEST_HARNESS
#define TEST_HARNESS

#include <parse_tree.h>

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

int runTests(testData* tests[], int numTests);

#endif