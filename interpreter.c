#include <stdio.h> 
#include <stdlib.h>
#include <string.h>

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
        } single_t;

        struct {
            struct statement_s* first;
            struct multipleStatements_s* rest;
        } multi_t;
    };
} multipleStatements_s;

enum stmntTag {BLOCK, DEFINE, EXECUTE, RETURN, IF, WHILE, PASS};

typedef struct statement_s {
    enum stmntTag tag;
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

enum ifTag {IF_ONLY, IF_ELSE, IF_ELSEIF, IF_ELSEIF_ELSE};

typedef struct if_s {
    enum ifTag tag;
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
    enum multiTag tag;
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

enum exprTag {PARENTHESES, IDENTIFIER, NUMERIC, BOOLEAN, CHARACTER, LIST, EVALUATE, FUNCTION};

typedef struct expression_s {
    enum exprTag tag;
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

enum numTag {NUMBER, NUMERIC_OPERATOR};
enum operator {ADD, SUBTRACT, MULTIPLY, DIVIDE};

typedef struct numeric_s {
    enum numTag tag;
    union {
        struct {
            struct numberData_s* num;
        } number_t;

        struct {
            struct numberData_s* num1;
            struct numberData_s* num2;
            enum operator op;
        } numeric_operator_t;
    }; 
} numeric_s;

enum boolTag {TRUE, FALSE, NOT, OR, AND, EQUALS};

typedef struct boolean_s {
    enum boolTag tag;
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

enum listTag {EMPTY, NOT_EMPTY};

typedef struct list_s {
    enum listTag tag;
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
    enum multiTag tag;
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

enum typedTag {TYPED, NOT_TYPED};

typedef struct functionDeclaration_s {
    enum typedTag tag;
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
    enum typedTag tag;
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
    char character;
} character_s;

typedef struct identifier_s {
    char* identifier;
} identifier_s;

// Forward declarations for representation functions
char* numberDataRepr(numberData_s* param);
char* programRepr(program_s* param);
char* multipleStatementsRepr(multipleStatements_s* param);
char* statementRepr(statement_s* param);
char* ifRepr(if_s* param);
char* elseIfChainRepr(elseIfChain_s* param);
char* expressionRepr(expression_s* param);
char* numericRepr(numeric_s* param);
char* booleanRepr(boolean_s* param);
char* listRepr(list_s* param);
char* multipleExpressionsRepr(multipleExpressions_s* param);
char* functionDeclarationRepr(functionDeclaration_s* param);
char* bindingIdentifierRepr(bindingIdentifier_s* param);
char* characterRepr(character_s* param);
char* identifierRepr(identifier_s* param);

// Representation functions
char* numberDataRepr(numberData_s* param) {
    int paramlength;
    char* paramStr;
    int returnLength;
    char* returnStr;

    if (param->type == INT) {
        paramlength = snprintf(NULL, 0, "%d", param->integer) + 1;
        paramStr = malloc(paramlength);
        snprintf(paramStr, paramlength, "%d", param->integer);
    } else if (param->type == DOUBLE) {
        paramlength = snprintf(NULL, 0, "%lf", param->decimal) + 1;
        paramStr = malloc(paramlength);
        snprintf(paramStr, paramlength, "%lf", param->decimal);
    } else {
        return "Error: invalid or unset tag.";
    }

    returnLength = strlen("NumberData: []") + paramlength;
    returnStr = malloc(returnLength);

    snprintf(returnStr, returnLength, "%s%s%s", "NumberData: [", paramStr, "]");
    return returnStr;
}

char* programRepr(program_s* param) {
    char* multStmntStr = multipleStatementsRepr(param->stmnts);
    int returnLength = strlen("Program: []") + strlen(multStmntStr) + 1;
    char* returnStr = malloc(returnLength);

    snprintf(returnStr, returnLength, "%s%s%s", "Program: [", multStmntStr, "]");
    return returnStr;
}

char* multipleStatementsRepr(multipleStatements_s* param) {
    char* tagName;
    int returnLength;
    char* returnStr;

    if (param->tag == SINGLE) {
        tagName = "single";

        char* stmntStr = statementRepr(param->single_t.stmnt);

        returnLength = strlen("MultipleStatements(): []") + strlen(tagName) + strlen(stmntStr) + 1;
        returnStr = malloc(returnLength);

        snprintf(returnStr, returnLength, "%s%s%s%s%s", "MultipleStatements(", tagName, "): [", stmntStr, "]");
    } else if (param->tag == MULTI) {
        tagName = "multi";

        char* firstStr = statementRepr(param->multi_t.first);
        char* restStr = multipleStatementsRepr(param->multi_t.rest);
        
        returnLength = strlen("MultipleStatements(): [, ]") + strlen(tagName) + strlen(firstStr) + strlen(restStr) + 1;
        returnStr = malloc(returnLength);

        snprintf(returnStr, returnLength, "%s%s%s%s%s%s%s", "MultipleStatements(", tagName, "): [", firstStr, ", ", restStr, "]");
    } else {
        return "Error: invalid or unset tag.";
    }

    return returnStr;
}

char* statementRepr(statement_s* param) {
    char* tagName;
    int returnLength;
    char* returnStr;

    if (param->tag == BLOCK) {
        tagName = "block";

        char* multStmntStr = multipleStatementsRepr(param->block_t.stmnts);

        returnLength = strlen("Statement(): []") + strlen(tagName) + strlen(multStmntStr) + 1;
        returnStr = malloc(returnLength);

        snprintf(returnStr, returnLength, "%s%s%s%s%s", "Statement(", tagName, "): [", multStmntStr, "]");
    } else if (param->tag == DEFINE) {
        tagName = "define";

        char* bidStr = bindingIdentifierRepr(param->define_t.bid);
        char* exprStr = expressionRepr(param->define_t.expr);

        returnLength = strlen("Statement(): [, ]") + strlen(tagName) + strlen(bidStr) + strlen(exprStr) + 1;
        returnStr = malloc(returnLength);

        snprintf(returnStr, returnLength, "%s%s%s%s%s%s%s", "Statement(", tagName, "): [", bidStr, ", ", exprStr, "]");
    } else if (param->tag == EXECUTE) {
        tagName = "execute";

        char* idStr = identifierRepr(param->execute_t.id);
        char* listStr = listRepr(param->execute_t.list);

        returnLength = strlen("Statement(): [, ]") + strlen(tagName) + strlen(idStr) + strlen(listStr) + 1;
        returnStr = malloc(returnLength);

        snprintf(returnStr, returnLength, "%s%s%s%s%s%s%s", "Statement(", tagName, "): [", idStr, ", ", listStr, "]");
    } else if (param->tag == RETURN) {
        tagName = "return";

        char* exprStr = expressionRepr(param->return_t.expr);

        returnLength = strlen("Statement(): []") + strlen(tagName) + strlen(exprStr) + 1;
        returnStr = malloc(returnLength);

        snprintf(returnStr, returnLength, "%s%s%s%s%s", "Statement(", tagName, "): [", exprStr, "]");
    } else if (param->tag == IF) {
        tagName = "if";

        char* ifStr = ifRepr(param->if_t.stmnt);

        returnLength = strlen("Statement(): []") + strlen(tagName) + strlen(ifStr) + 1;
        returnStr = malloc(returnLength);

        snprintf(returnStr, returnLength, "%s%s%s%s%s", "Statement(", tagName, "): [", ifStr, "]");
    } else if (param->tag == WHILE) {
        tagName = "while";
        
        char* exprStr = expressionRepr(param->while_t.expr);
        char* stmntStr = statementRepr(param->while_t.stmnt);

        returnLength = strlen("Statement(): [, ]") + strlen(tagName) + strlen(exprStr) + strlen(stmntStr) + 1;
        returnStr = malloc(returnLength);

        snprintf(returnStr, returnLength, "%s%s%s%s%s%s%s", "Statement(", tagName, "): [", exprStr, ", ", stmntStr, "]");
    } else if (param->tag == PASS) {
        tagName = "pass";
        returnLength = strlen("Statement()") + strlen(tagName) + 1;
        returnStr = malloc(returnLength);

        snprintf(returnStr, returnLength, "%s", "Statement(", tagName, ")");
    } else {
        return "Error: invalid or unset tag.";
    }

    return returnStr;
}

char* ifRepr(if_s* param) {
    char* tagName;
    int returnLength;
    char* returnStr;

    if (param->tag == IF_ONLY) {
        tagName = "if_only";

        char* exprStr = expressionRepr(param->if_only_t.ifExpr);
        char* stmntStr = statementRepr(param->if_only_t.ifStmnt);

        returnLength = strlen("If(): [, ]") + strlen(tagName) + strlen(exprStr) + strlen(stmntStr) + 1;
        returnStr = malloc(returnLength);

        snprintf(returnStr, returnLength, "%s%s%s%s%s%s%s", "If(", tagName, "): [", exprStr, ", ", stmntStr, "]");
    } else if (param->tag == IF_ELSE) {
        tagName = "if_else";

        char* ifExprStr = expressionRepr(param->if_else_t.ifExpr);
        char* ifStmntStr = statementRepr(param->if_else_t.ifStmnt);
        char* elseStmntStr = statementRepr(param->if_else_t.elseStmnt);

        returnLength = strlen("If(): [, , ]") + strlen(tagName)
                     + strlen(ifExprStr) + strlen(ifStmntStr) + strlen(elseStmntStr) + 1;
        returnStr = malloc(returnLength);

        snprintf(returnStr, returnLength, "%s%s%s%s%s%s%s%s%s",
                 "If(", tagName, "): [", ifExprStr, ", ", ifStmntStr, ", ", elseStmntStr, "]");
    } else if (param->tag == IF_ELSEIF) {
        tagName = "if_elseif";

        char* ifExprStr = expressionRepr(param->if_elseif_t.ifExpr);
        char* ifStmntStr = statementRepr(param->if_elseif_t.ifStmnt);
        char* chainStr = statementRepr(param->if_elseif_t.chain);

        returnLength = strlen("If(): [, , ]") + strlen(tagName)
                     + strlen(ifExprStr) + strlen(ifStmntStr) + strlen(chainStr) + 1;
        returnStr = malloc(returnLength);

        snprintf(returnStr, returnLength, "%s%s%s%s%s%s%s%s%S",
                 "If(", tagName, "): [", ifExprStr, ", ", ifStmntStr, ", ", chainStr, "]");
    } else if (param->tag == IF_ELSEIF_ELSE) {
        tagName = "if_elseif_else";

        char* ifExprStr = expressionRepr(param->if_elseif_t.ifExpr);
        char* ifStmntStr = statementRepr(param->if_elseif_t.ifStmnt);
        char* chainStr = statementRepr(param->if_elseif_t.chain);
        char* elseStmntStr = statementRepr(param->if_else_t.elseStmnt);

        returnLength = strlen("If(): [, , , ]") + strlen(tagName)
                     + strlen(ifExprStr) + strlen(ifStmntStr) + strlen(chainStr) + strlen(elseStmntStr) + 1;
        returnStr = malloc(returnLength);

        snprintf(returnStr, returnLength, "%s%s%s%s%s%s%s%s%s",
                 "If(", tagName, "): [", ifExprStr, ", ", ifStmntStr, ", ", chainStr, ", ", elseStmntStr, "]");
    } else {
        return "Error: invalid or unset tag.";
    }

    return returnStr;
}

char* elseIfChainRepr(elseIfChain_s* param) {
    char* tagName;
    int returnLength;
    char* returnStr;

    if (param->tag == SINGLE) {
        tagName = "single";

        char* exprStr = expressionRepr(param->single_t.ifExpr);
        char* stmntStr = statementRepr(param->single_t.ifStmnt);

        returnLength = strlen("ElseIfChain(): [, ]") + strlen(tagName) + strlen(exprStr) + strlen(stmntStr) + 1;
        returnStr = malloc(returnLength);

        snprintf(returnStr, returnLength, "%s%s%s%s%s%s%s", "ElseIfChain(", tagName, "): [", exprStr, ", ", stmntStr, "]");
    } else if (param->tag == MULTI) {
        tagName = "multi";

        char* ifExprStr = expressionRepr(param->multi_t.ifExpr);
        char* ifStmntStr = statementRepr(param->multi_t.ifStmnt);
        char* chainStr = statementRepr(param->multi_t.chain);

        returnLength = strlen("ElseIfChain(): [, , ]") + strlen(tagName)
                     + strlen(ifExprStr) + strlen(ifStmntStr) + strlen(chainStr) + 1;
        returnStr = malloc(returnLength);

        snprintf(returnStr, returnLength, "%s%s%s%s%s%s%s%s%s",
                 "ElseIfChain(", tagName, "): [", ifExprStr, ", ", ifStmntStr, ", ", chainStr, "]");
    } else {
        return "Error: invalid or unset tag.";
    }

    return returnStr;
}

char* expressionRepr(expression_s* param) {
    char* tagName;
    int returnLength;
    char* returnStr;

    if (param->tag == PARENTHESES) {
        tagName = "parentheses";

        char* exprStr = expressionRepr(param->parentheses_t.expr);

        returnLength = strlen("Expression(): []") + strlen(tagName) + strlen(exprStr) + 1;
        returnStr = malloc(returnLength);

        snprintf(returnStr, returnLength, "%s%s%s%s%s", "Expression(", tagName, "): [", exprStr, "]");
    } else if (param->tag == IDENTIFIER) {
        tagName = "identifier";

        char* idStr = identifierRepr(param->identifier_t.id);

        returnLength = strlen("Expression(): []") + strlen(tagName) + strlen(idStr) + 1;
        returnStr = malloc(returnLength);

        snprintf(returnStr, returnLength, "%s%s%s%s%s", "Expression(", tagName, "): [", idStr, "]");
    } else if (param->tag == NUMERIC) {
        tagName = "numeric";

        char* numStr = numericRepr(param->numeric_t.num);

        returnLength = strlen("Expression(): []") + strlen(tagName) + strlen(numStr) + 1;
        returnStr = malloc(returnLength);

        snprintf(returnStr, returnLength, "%s%s%s%s%s", "Expression(", tagName, "): [", numStr, "]");
    } else if (param->tag == BOOLEAN) {
        tagName = "boolean";

        char* boolStr = booleanRepr(param->boolean_t.bool);

        returnLength = strlen("Expression(): []") + strlen(tagName) + strlen(boolStr) + 1;
        returnStr = malloc(returnLength);

        snprintf(returnStr, returnLength, "%s%s%s%s%s", "Expression(", tagName, "): [", boolStr, "]");
    } else if (param->tag == CHARACTER) {
        tagName = "character";

        char* chrStr = characterRepr(param->character_t.chr);

        returnLength = strlen("Expression(): []") + strlen(tagName) + strlen(chrStr) + 1;
        returnStr = malloc(returnLength);

        snprintf(returnStr, returnLength, "%s%s%s%s%s", "Expression(", tagName, "): [", chrStr, "]");
    } else if (param->tag == LIST) {
        tagName = "list";

        char* listStr = listRepr(param->list_t.list);

        returnLength = strlen("Expression(): []") + strlen(tagName) + strlen(listStr) + 1;
        returnStr = malloc(returnLength);

        snprintf(returnStr, returnLength, "%s%s%s%s%s", "Expression(", tagName, "): [", listStr, "]");
    } else if (param->tag == EVALUATE) {
        tagName = "evaluate";

        char* idStr = identifierRepr(param->evaluate_t.id);
        char* listStr = listRepr(param->evaluate_t.list);

        returnLength = strlen("Expression(): [, ]") + strlen(tagName) + strlen(idStr) + strlen(listStr) + 1;
        returnStr = malloc(returnLength);

        snprintf(returnStr, returnLength, "%s%s%s%s%s%s%s", "Expression(", tagName, "): [", idStr, ", ", listStr, "]");
    } else if (param->tag == FUNCTION) {
        tagName = "function";

        char* funcStr = functionDeclarationRepr(param->function_t.func);

        returnLength = strlen("Expression(): []") + strlen(tagName) + strlen(funcStr) + 1;
        returnStr = malloc(returnLength);

        snprintf(returnStr, returnLength, "%s%s%s%s%s", "Expression(", tagName, "): [", funcStr, "]");
    } else {
        return "Error: invalid or unset tag.";
    }

    return returnStr;
}

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