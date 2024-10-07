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

enum numDataTag {INT, DOUBLE};

typedef struct numberData_s {
    enum numDataTag type;
    union {
        int integer;
        double decimal;
    };
} numberData_s;

// Parse tree types
typedef struct program_s {
    struct multipleStatements_s* stmnts;
} program_s;

enum multiTag {SINGLE, MULTI};

typedef struct multipleStatements_s {
    enum multiTag tag;
    union {
        struct {
            struct statement_s* stmnt;
        } SINGLE;

        struct {
            struct statement_s* first;
            struct multipleStatements_s* rest;
        } MULTI;
    };
} multipleStatements_s;

enum stmntTag {BLOCK, DEFINE, EXECUTE, RETURN, IF, WHILE, PASS};

typedef struct statement_s {
    enum stmntTag tag;
    union {
        struct {
            struct multipleStatements_s* stmnts;
        } BLOCK;

        struct {
            struct bindingIdentifier_s* bid;
            struct expression_s* expr;
        } DEFINE;

        struct {
            struct identifier_s* id;
            struct list_s* list;
        } EXECUTE;

        struct {
            struct expression_s* expr;
        } RETURN;

        struct {
            struct if_s* stmnt;
        } IF;

        struct {
            struct expression_s* expr;
            struct statement_s* stmnt;
        } WHILE;

        struct {
            int placeholder;
        } PASS;
    };
} statement_s;

enum ifTag {IF_ONLY, IF_ELSE, IF_ELSEIF, IF_ELSEIF_ELSE};

typedef struct if_s {
    enum ifTag tag;
    union {
        struct {
            struct expression_s* ifExpr;
            struct statement_s* ifStmnt;
        } IF_ONLY;

        struct {
            struct expression_s* ifExpr;
            struct statement_s* ifStmnt;
            struct statement_s* elseStmnt;
        } IF_ELSE;

        struct {
            struct expression_s* ifExpr;
            struct statement_s* ifStmnt;
            struct elseIfChain_s* chain;
        } IF_ELSEIF;

        struct {
            struct expression_s* ifExpr;
            struct statement_s* ifStmnt;
            struct elseIfChain_s* chain;
            struct statement_s* elseStmnt;
        } IF_ELSEIF_ELSE;
    };
} if_s;

typedef struct elseIfChain_s {
    enum multiTag tag;
    union {
        struct {
            struct expression_s* ifExpr;
            struct statement_s* ifStmnt;
        } SINGLE;

        struct {
            struct expression_s* ifExpr;
            struct statement_s* ifStmnt;
            struct elseIfChain_s* chain;
        } MULTI;
    };
} elseIfChain_s;

enum exprTag {PARENTHESES, IDENTIFIER, NUMERIC, BOOLEAN, CHARACTER, LIST, EVALUATE, FUNCTION};

typedef struct expression_s {
    enum exprTag tag;
    union {
        struct {
            struct expression_s* expr;
        } PARENTHESES;

        struct {
            struct identifier_s* id;
        } IDENTIFIER;

        struct {
            struct numeric_s* num;
        } NUMERIC;

        struct {
            struct boolean_s* bool;
        } BOOLEAN;

        struct {
            struct character_s* chr;
        } CHARACTER;

        struct {
            struct list_s* list;
        } LIST;

        struct {
            struct identifier_s* id;
            struct list_s* list;
        } EVALUATE;

        struct {
            struct functionDeclaration_s* func;
        } FUNCTION;
    };
} expression_s;

enum numTag {NUMBER, NUMERIC_OPERATOR};
enum operator {ADD, SUBTRACT, MULTIPLY, DIVIDE};

typedef struct numeric_s {
    enum numTag tag;
    union {
        struct {
            struct numberData_s* num;
        } NUMBER;

        struct {
            struct numberData_s* num1;
            struct numberData_s* num2;
            enum operator op;
        } NUMERIC_OPERATOR;
    }; 
} numeric_s;

enum boolTag {TRUE, FALSE, NOT, OR, AND, EQUALS};

typedef struct boolean_s {
    enum boolTag tag;
    union {
        struct {
            int placeholder;
        } TRUE;

        struct {
            int placeholder;
        } FALSE;

        struct {
            struct expression_s* expr;
        } NOT;

        struct {
            struct expression_s* expr1;
            struct expression_s* expr2;
        } OR;

        struct {
            struct expression_s* expr1;
            struct expression_s* expr2;
        } AND;

        struct {
            struct expression_s* expr1;
            struct expression_s* expr2;
        } EQUALS;
    };
} boolean_s;

enum listTag {EMPTY, NOT_EMPTY};

typedef struct list_s {
    enum listTag tag;
    union {
        struct {
            int placeholder;
        } EMPTY;

        struct {
            struct multipleExpressions_s* exprs;
        } NOT_EMPTY;
    };
} list_s;

typedef struct multipleExpressions_s {
    enum multiTag tag;
    union {
        struct {
            struct expression_s* expr;
        } SINGLE;

        struct {
            struct expression_s* first;
            struct multipleExpressions_s* rest;
        } MULTI;
    };
} multipleExpressions_s;

enum typedTag {TYPED, NOT_TYPED};

typedef struct functionDeclaration_s {
    enum typedTag tag;
    union {
        struct {
            struct list_s* list;
            struct statement_s* stmnt;
        } NOT_TYPED;

        struct {
            struct identifier_s* id;
            struct list_s* list;
            struct statement_s* stmnt;
        } TYPED;
    };
} functionDeclaration_s;

typedef struct bindingIdentifier_s {
    enum typedTag tag;
    union {
        struct {
            struct identifier_s* id;
        } NOT_TYPED;

        struct {
            struct identifier_s* type;
            struct identifier_s* id;
        } TYPED;
    };
} bindingIdentifier_s;

typedef struct character_s {
    char character;
} character_s;

typedef struct identifier_s {
    char* identifier;
} identifier_s;

int main() {
    // "define x: 5"
    identifier_s defStmntId;
    defStmntId.identifier = "x";
    bindingIdentifier_s defStmntBid;
    defStmntBid.tag = NOT_TYPED;
    defStmntBid.NOT_TYPED.id = &defStmntId;
    numberData_s fiveInt;
    fiveInt.type = INT;
    fiveInt.integer = 5;
    numeric_s numFive;
    numFive.tag = NUMBER;
    numFive.NUMBER.num = &fiveInt;
    expression_s numExpr;
    numExpr.tag = NUMERIC;
    numExpr.NUMERIC.num = &numFive;
    statement_s defStmnt;
    defStmnt.tag = DEFINE;
    defStmnt.DEFINE.bid = &defStmntBid;
    defStmnt.DEFINE.expr = &numExpr;
    multipleStatements_s defMultStmnt;
    defMultStmnt.tag = SINGLE;
    defMultStmnt.SINGLE.stmnt = &defStmnt;
    program_s defStmntRoot;
    defStmntRoot.stmnts = &defMultStmnt;

    // "if true: def x: 5"
    boolean_s boolTrue;
    boolTrue.tag = TRUE;
    boolTrue.TRUE.placeholder = 1;
    expression_s boolExpr;
    boolExpr.tag = BOOLEAN;
    boolExpr.BOOLEAN.bool = &boolTrue;
    if_s ifCond;
    ifCond.tag = IF_ONLY;
    ifCond.IF_ONLY.ifExpr = &boolExpr;
    ifCond.IF_ONLY.ifStmnt = &defStmnt;
    statement_s ifStmnt;
    ifStmnt.tag = IF;
    ifStmnt.IF.stmnt = &ifCond;
    multipleStatements_s ifMultStmnt;
    ifMultStmnt.tag = SINGLE;
    ifMultStmnt.SINGLE.stmnt = &ifStmnt;
    program_s ifStmntRoot;
    ifStmntRoot.stmnts = &ifMultStmnt;

    return 0;
}