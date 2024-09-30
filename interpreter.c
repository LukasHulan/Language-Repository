/*
* Number data type. Uses a union to handle both integers and doubles.
* 
* Examples
* myNumber1: type: INT, data: 0
* myNumber3: type: INT, data: 5
* myNumber4: type: INT, data: 53
* myNumber5: type: DOUBLE, data: 0.0
* myNumber6: type: DOUBLE, data: 2.5
* myNumber7: type: DOUBLE, data: 10.7
*/

typedef union numberUnion {
    int integer;
    double decimal;
} numberUnion;

enum numberUnionFlag {INT, DOUBLE};

typedef struct numberData {
    enum numberUnionFlag type;
    numberUnion data;
} numberData;

// Parse tree types

typedef struct rootNode {
    multipleStatementsNode *value;
} programNode;

enum multStmntFlag {MULTI, SINGLE};

typedef struct multipleStatementsNode {
    enum multStmntFlag flag;
    statement *value;
} multipleStatementsNode;
