/*
* Number data type. Uses a union to handle both integers and doubles.
* 
* Examples
* myNumber1: type: INT, integer: 1
* myNumber3: type: INT, integer: 5
* myNumber4: type: INT, integer: 53
* myNumber5: type: DOUBLE, decimal: 0.0
* myNumber6: type: DOUBLE, decimal: 2.5
* myNumber7: type: DOUBLE, decimal: 10.7
*/

enum numberUnionFlag {INT, DOUBLE};

typedef struct {
    enum numberUnionFlag type;
    union {
        int integer;
        double decimal;
    };
    
} numberData;

// Parse tree types

typedef struct{
    multipleStatementsNode* value;
} programNode;

enum multStmntFlag {MULTI, SINGLE};

typedef struct {
    enum multStmntFlag flag;
    union {
        statementNode* single;
        struct {
            statementNode* first;
            multipleStatementsNode* rest;
        };
    };
} multipleStatementsNode;

enum stmntFlag {BLOCK, DEFINE, EXECUTE, RETURN, IF, WHILE, PASS};

typedef struct {
    enum stmntFlag flag;
    union {
        multipleStatementsNode* block;
        struct {
            defineNode* def;
            bindingIdentifierNode* bid;
            expressionNode* expr;
        };
        struct {
            executeNode* exe;
            identifierNode* id;
            listNode* list;
        };
        struct {
            returnNode* rtn;
            expressionNode* expr;
        };
        ifNode* ifStmnt;
        struct {
            expressionNode* expr;
            statementNode* stmnt;
        };
    };
} statementNode;