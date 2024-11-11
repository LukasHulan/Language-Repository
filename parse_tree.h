#ifndef PARSE_TREE
#define PARSE_TREE

enum tags {
    INT,
    DOUBLE,
    SINGLE,
    MULTI,
    BLOCK,
    DEFINE,
    EXECUTE,
    RETURN,
    IF,
    WHILE,
    PASS,
    IF_ONLY,
    IF_ELSE,
    IF_ELSEIF,
    IF_ELSEIF_ELSE,
    PARENTHESES,
    IDENTIFIER,
    NUMERIC,
    BOOLEAN,
    CHARACTER,
    LIST,
    EVALUATE,
    FUNCTION,
    NUMBER,
    NUMERIC_OPERATOR,
    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    TRUE,
    FALSE,
    NOT,
    OR,
    AND,
    EQUALS,
    EMPTY,
    NOT_EMPTY,
    NUMBER_DATA,
    PROGRAM,
    MULTIPLE_STATEMENTS,
    STATEMENT,
    ELSE_IF_CHAIN,
    EXPRESSION,
    MULTIPLE_EXPRESSIONS,
    FUNCTION_DECLARATION,
    BINDING_IDENTIFIER,
    TYPED,
    NOT_TYPED,
};

// Tags: INT, DOUBLE
typedef struct numberData_s {
    enum tags type;
    union {
        int integer;
        double decimal;
    };
} numberData_s;

typedef struct program_s {
    struct multipleStatements_s* stmnts;
} program_s;

// Tags: SINGLE, MULTI
typedef struct multipleStatements_s {
    enum tags tag;
    union {
        struct {
            struct statement_s* stmnt;
        } single_t;

        struct {
            struct statement_s* first;
            struct multipleStatements_s* rest;
        } multi_t;
    };
} multipleStatements_s;

// Tags: BLOCK, DEFINE, EXECUTE, RETURN, IF, WHILE, PASS
typedef struct statement_s {
    enum tags tag;
    union {
        struct {
            struct multipleStatements_s* stmnts;
        } block_t;

        struct {
            struct bindingIdentifier_s* bid;
            struct expression_s* expr;
        } define_t;

        struct {
            struct identifier_s* id;
            struct list_s* list;
        } execute_t;

        struct {
            struct expression_s* expr;
        } return_t;

        struct {
            struct if_s* stmnt;
        } if_t;

        struct {
            struct expression_s* expr;
            struct statement_s* stmnt;
        } while_t;

        struct {
            int placeholder;
        } pass_t;
    };
} statement_s;

// Tags: IF_ONLY, IF_ELSE, IF_ELSEIF, IF_ELSEIF_ELSE
typedef struct if_s {
    enum tags tag;
    union {
        struct {
            struct expression_s* ifExpr;
            struct statement_s* ifStmnt;
        } if_only_t;

        struct {
            struct expression_s* ifExpr;
            struct statement_s* ifStmnt;
            struct statement_s* elseStmnt;
        } if_else_t;

        struct {
            struct expression_s* ifExpr;
            struct statement_s* ifStmnt;
            struct elseIfChain_s* chain;
        } if_elseif_t;

        struct {
            struct expression_s* ifExpr;
            struct statement_s* ifStmnt;
            struct elseIfChain_s* chain;
            struct statement_s* elseStmnt;
        } if_elseif_else_t;
    };
} if_s;

typedef struct elseIfChain_s {
    enum tags tag;
    union {
        struct {
            struct expression_s* ifExpr;
            struct statement_s* ifStmnt;
        } single_t;

        struct {
            struct expression_s* ifExpr;
            struct statement_s* ifStmnt;
            struct elseIfChain_s* chain;
        } multi_t;
    };
} elseIfChain_s;

// Tags: PARENTHESES, IDENTIFIER, NUMERIC, BOOLEAN, CHARACTER, LIST, EVALUATE, FUNCTION
typedef struct expression_s {
    enum tags tag;
    union {
        struct {
            struct expression_s* expr;
        } parentheses_t;

        struct {
            struct identifier_s* id;
        } identifier_t;

        struct {
            struct numeric_s* num;
        } numeric_t;

        struct {
            struct boolean_s* bool;
        } boolean_t;

        struct {
            struct character_s* chr;
        } character_t;

        struct {
            struct list_s* list;
        } list_t;

        struct {
            struct identifier_s* id;
            struct list_s* list;
        } evaluate_t;

        struct {
            struct functionDeclaration_s* func;
        } function_t;
    };
} expression_s;

// Tags: NUMBER, NUMERIC_OPERATOR
// Operators: ADD, SUBTRACT, MULTIPLY, DIVIDE
typedef struct numeric_s {
    enum tags tag;
    union {
        struct {
            struct numberData_s* num;
        } number_t;

        struct {
            struct numberData_s* num1;
            struct numberData_s* num2;
            enum tags op;
        } numeric_operator_t;
    }; 
} numeric_s;

// Tags: TRUE, FALSE, NOT, OR, AND, EQUALS
typedef struct boolean_s {
    enum tags tag;
    union {
        struct {
            int placeholder;
        } true_t;

        struct {
            int placeholder;
        } false_t;

        struct {
            struct expression_s* expr;
        } not_t;

        struct {
            struct expression_s* expr1;
            struct expression_s* expr2;
        } or_t;

        struct {
            struct expression_s* expr1;
            struct expression_s* expr2;
        } and_t;

        struct {
            struct expression_s* expr1;
            struct expression_s* expr2;
        } equals_t;
    };
} boolean_s;

// Tags: EMPTY, NOT_EMPTY
typedef struct list_s {
    enum tags tag;
    union {
        struct {
            int placeholder;
        } empty_t;

        struct {
            struct multipleExpressions_s* exprs;
        } not_empty_t;
    };
} list_s;

typedef struct multipleExpressions_s {
    enum tags tag;
    union {
        struct {
            struct expression_s* expr;
        } single_t;

        struct {
            struct expression_s* first;
            struct multipleExpressions_s* rest;
        } multi_t;
    };
} multipleExpressions_s;

// Tags: TYPED, NOT_TYPED
typedef struct functionDeclaration_s {
    enum tags tag;
    union {
        struct {
            struct list_s* list;
            struct statement_s* stmnt;
        } not_typed_t;

        struct {
            struct identifier_s* id;
            struct list_s* list;
            struct statement_s* stmnt;
        } typed_t;
    };
} functionDeclaration_s;

typedef struct bindingIdentifier_s {
    enum tags tag;
    union {
        struct {
            struct identifier_s* id;
        } not_typed_t;

        struct {
            struct identifier_s* type;
            struct identifier_s* id;
        } typed_t;
    };
} bindingIdentifier_s;

typedef struct character_s {
    char* character;
} character_s;

typedef struct identifier_s {
    char* identifier;
} identifier_s;

typedef union parseTreeType_s {
    numberData_s* numberDataValue;
    program_s* programValue;
    multipleStatements_s* multipleStatementsValue;
    statement_s* statementValue;
    if_s* ifValue;
    elseIfChain_s* elseIfChainValue;
    expression_s* expressionValue;
    numeric_s* numericValue;
    boolean_s* booleanValue;
    list_s* listValue;
    multipleExpressions_s* multipleExpressionsValue;
    functionDeclaration_s* functionDeclarationValue;
    bindingIdentifier_s* bindingIdentifierValue;
    character_s* characterValue;
    identifier_s* identifierValue;
} parseTreeType_s;

// Tags: NUMBER_DATA, PROGRAM, MULTIPLE_STATEMENTS, STATEMENT, IF, ELSE_IF_CHAIN, 
//       EXPRESSION, NUMERIC, BOOLEAN, LIST,  MULTIPLE_EXPRESSIONS, 
//       FUNCTION_DECLARATION, BINDING_IDENTIFIER, CHARACTER, IDENTIFIER
typedef struct parseTree {
    enum tags type;
    parseTreeType_s tree;
} parseTree;

int ptcmp(parseTree* first, parseTree* second);

#endif