#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "parse_tree.h"
#include "tokenizer.h"
#include "to_string.h"
#include "parser.h"

parseTree* parse(tokenData* tokens) {
    parseTree* programTree = malloc(sizeof(parseTree));
    programTree->type = PROGRAM;
    programTree->tree.programValue = parseProgram(tokens);

    return programTree;
}

program_s* parseProgram(tokenData* tokens) {
    program_s* program = malloc(sizeof(program_s));
    program->stmnts = parseInitialStatements(tokens);

    return program;
}

multipleStatements_s* parseInitialStatements(tokenData* tokens) {
    multipleStatements_s* mulitStmnts = malloc(sizeof(multipleStatements_s));
    mulitStmnts->tag = SINGLE;
    mulitStmnts->single_t.stmnt = parseStatement(tokens);

    if (tokens->index < tokens->length) {
        mulitStmnts->tag = MULTI;
        mulitStmnts->multi_t.first = mulitStmnts->single_t.stmnt;
        mulitStmnts->multi_t.rest = parseInitialStatements(tokens);
    }

    return mulitStmnts;
};

multipleStatements_s* parseMultiStatements(tokenData* tokens) {
    multipleStatements_s* mulitStmnts = malloc(sizeof(multipleStatements_s));
    mulitStmnts->tag = SINGLE;
    mulitStmnts->single_t.stmnt = parseStatement(tokens);

    if (strcmp(peekToken(tokens), "}")) {
        mulitStmnts->tag = MULTI;
        mulitStmnts->multi_t.first = mulitStmnts->single_t.stmnt;
        mulitStmnts->multi_t.rest = parseMultiStatements(tokens);
    } else {
        popToken(tokens);
    }

    return mulitStmnts;
};

statement_s* parseStatement(tokenData* tokens) {
    statement_s* stmnt = malloc(sizeof(statement_s));

    char* kword = popToken(tokens);

    if (!strcmp(kword, "{")) {
        stmnt->tag = BLOCK;
        stmnt->block_t.stmnts = parseMultiStatements(tokens);
    } else if (!strcmp(kword, "define") || !strcmp(kword, "def")) {
        stmnt->tag = DEFINE;
        stmnt->define_t.bid = parseBindingIdentifier(tokens);

        if (strcmp(popToken(tokens), ":")) {
            syntaxError();
        }

        stmnt->define_t.expr = parseExpression(tokens);
    } else if (!strcmp(kword, "execute") || !strcmp(kword, "exe")) {
        stmnt->tag = EXECUTE;

        char* id = popToken(tokens);

        if (!isIdentifier(id)) {
            syntaxError();
        }

        stmnt->execute_t.id->identifier = id;

        if (strcmp(popToken(tokens), ":")) {
            syntaxError();
        }

        stmnt->execute_t.list = parseList(tokens);
    } else if (!strcmp(kword, "return") || !strcmp(kword, "rtn")) {
        stmnt->tag = RETURN;
        stmnt->return_t.expr = parseExpression(tokens);
    } else if (!strcmp(kword, "if")) {
        stmnt->tag = IF;
        stmnt->if_t.stmnt = parseIf(tokens);
    } else if (!strcmp(kword, "while")) {
        stmnt->tag = WHILE;
        stmnt->while_t.expr = parseExpression(tokens);

        if (strcmp(popToken(tokens), ":")) {
            syntaxError();
        }

        stmnt->while_t.stmnt = parseStatement(tokens);
    } else if (!strcmp(kword, "pass")) {
        stmnt->tag = PASS;
        stmnt->pass_t.placeholder = 1;
    } else {
        syntaxError();
    }

    return stmnt;
}

if_s* parseIf(tokenData* tokens) {
    if_s* ifStmnt = malloc(sizeof(if_s));
    ifStmnt->tag = IF_ONLY;
    ifStmnt->if_only_t.ifExpr = parseExpression(tokens);

    if (strcmp(popToken(tokens), ":")) {
        syntaxError();
    }

    ifStmnt->if_only_t.ifStmnt = parseStatement(tokens);

    if (!strcmp(peekToken(tokens), "else")) {
        expression_s* ifExpr = ifStmnt->if_only_t.ifExpr;
        statement_s* ifstmnts = ifStmnt->if_only_t.ifStmnt;

        if (!strcmp(getToken(tokens, tokens->index + 1), ":")) {
            ifStmnt->tag = IF_ELSE;
            ifStmnt->if_else_t.ifExpr = ifExpr;
            ifStmnt->if_else_t.ifStmnt = ifstmnts;

            popToken(tokens);
            popToken(tokens);

            ifStmnt->if_else_t.elseStmnt = parseStatement(tokens);
        } else if (!strcmp(getToken(tokens, tokens->index + 1), "if")) {
            popToken(tokens);
            popToken(tokens);

            elseIfChain_s* elseIfChain = parseElseIfChain(tokens);

            if (!strcmp(peekToken(tokens), "else")) {
                if (!strcmp(getToken(tokens, tokens->index + 1), ":")) {
                    popToken(tokens);
                    popToken(tokens);

                    ifStmnt->tag = IF_ELSEIF_ELSE;
                    ifStmnt->if_elseif_else_t.ifExpr = ifExpr;
                    ifStmnt->if_elseif_else_t.ifStmnt = ifstmnts;
                    ifStmnt->if_elseif_else_t.chain = elseIfChain;
                    ifStmnt->if_elseif_else_t.elseStmnt = parseStatement(tokens);
                } else {
                    syntaxError();
                }
            } else {
                ifStmnt->tag = IF_ELSEIF;
                ifStmnt->if_elseif_else_t.ifExpr = ifExpr;
                ifStmnt->if_elseif_else_t.ifStmnt = ifstmnts;
                ifStmnt->if_elseif_else_t.chain = elseIfChain;
            }
        } else {
            syntaxError();
        }
    }

    return ifStmnt;
}

elseIfChain_s* parseElseIfChain(tokenData* tokens) {
    elseIfChain_s* elseIfChain = malloc(sizeof(elseIfChain_s));

    expression_s* elseIfExpr = parseExpression(tokens);

    if (strcmp(popToken(tokens), ":")) {
        syntaxError();
    }

    statement_s* elseIfStmnt = parseStatement(tokens);

    elseIfChain->tag = SINGLE;
    elseIfChain->single_t.ifExpr = elseIfExpr;
    elseIfChain->single_t.ifStmnt = elseIfStmnt;

    if (!strcmp(peekToken(tokens), "else")) {
        if (!strcmp(peekToken(tokens), "if")) {
            elseIfChain->tag = MULTI;
            elseIfChain->multi_t.ifExpr = elseIfExpr;
            elseIfChain->multi_t.ifStmnt = elseIfStmnt;

            popToken(tokens);
            popToken(tokens);

            elseIfChain->multi_t.chain = parseElseIfChain(tokens);
        } else if (!strcmp(peekToken(tokens), ":")) {
            return elseIfChain;
        } else {
            syntaxError();
        }
    }

    return elseIfChain;
}

expression_s* parseExpression(tokenData* tokens) {
    expression_s* expression = malloc(sizeof(expression_s));

    char* token = popToken(tokens);

    if (!strcmp(token, "(")) {
        expression->tag = PARENTHESES;
        expression->parentheses_t.expr = parseExpression(tokens);

        if (strcmp(popToken(tokens), ")")) {
            syntaxError();
        }
    } else if (!strcmp(token, "'")) {
        expression->tag = CHARACTER;

        char* character = popToken(tokens);

        if (strlen(character) != 1) {
            syntaxError();
        }

        expression->character_t.chr->character = character;

        if (strcmp(popToken(tokens), "'")) {
            syntaxError();
        }
    } else if (!strcmp(token, "[")) {
        expression->tag = LIST;
        expression->list_t.list = parseList(tokens);
    } else if (!strcmp(token, "evaluate") || !strcmp(token, "evl")) {
        expression->tag = EVALUATE;

        char* id = popToken(tokens);

        if (!isIdentifier(id)) {
            syntaxError();
        }

        expression->evaluate_t.id->identifier = id;

        if (strcmp(popToken(tokens), ":")) {
            syntaxError();
        }

        if (strcmp(popToken(tokens), "[")) {
            syntaxError();
        }

        expression->evaluate_t.list = parseList(tokens);
    } else if (!strcmp(token, "function") || !strcmp(token, "fnc")) {
        expression->tag = FUNCTION;
        expression->function_t.func = parseFunctionDeclaration(tokens);
    } else if (isIdentifier(token)) {
        expression->tag = IDENTIFIER;
        expression->identifier_t.id = malloc(sizeof(identifier_s));
        expression->identifier_t.id->identifier = token;
    } else if (isNum(token)) {
        expression->tag = NUMERIC;
        expression->numeric_t.num = malloc(sizeof(numeric_s));
        expression->numeric_t.num->tag = NUMBER;
        expression->numeric_t.num->number_t.num = parseNumberData(tokens);
    } else if (!strcmp(token, "true")) {
        expression->tag = BOOLEAN;
        expression->boolean_t.bool = malloc(sizeof(boolean_s));
        expression->boolean_t.bool->tag = TRUE;
        expression->boolean_t.bool->true_t.placeholder = 1;
    } else if (!strcmp(token, "false")) {
        expression->tag = BOOLEAN;
        expression->boolean_t.bool = malloc(sizeof(boolean_s));
        expression->boolean_t.bool->tag = FALSE;
        expression->boolean_t.bool->false_t.placeholder = 1;
    } else if (!strcmp(token, "!")) {
        expression->tag = BOOLEAN;
        expression->boolean_t.bool = malloc(sizeof(boolean_s));
        expression->boolean_t.bool->tag = NOT;
        expression->boolean_t.bool->not_t.expr = parseExpression(tokens);
    } else {
        syntaxError();
    }

    char* next = peekToken(tokens);

    if (!strcmp(next, "+")) {
        popToken(tokens);

        expression_s* expr1 = malloc(sizeof(expression_s));
        expr1->tag = NUMERIC;
        expr1->numeric_t.num = malloc(sizeof(numeric_s));
        expr1->numeric_t.num->tag = NUMERIC_OPERATOR;
        expr1->numeric_t.num->numeric_operator_t.num1 = expression;
        expr1->numeric_t.num->numeric_operator_t.op = ADD;
        expr1->numeric_t.num->numeric_operator_t.num2 = parseExpression(tokens);

        return expr1;
    } else if (!strcmp(next, "-")) {
        popToken(tokens);

        expression_s* expr1 = malloc(sizeof(expression_s));
        expr1->tag = NUMERIC;
        expr1->numeric_t.num = malloc(sizeof(numeric_s));
        expr1->numeric_t.num->tag = NUMERIC_OPERATOR;
        expr1->numeric_t.num->numeric_operator_t.num1 = expression;
        expr1->numeric_t.num->numeric_operator_t.op = SUBTRACT;
        expr1->numeric_t.num->numeric_operator_t.num2 = parseExpression(tokens);

        return expr1;
    } else if (!strcmp(next, "*")) {
        popToken(tokens);

        expression_s* expr1 = malloc(sizeof(expression_s));
        expr1->tag = NUMERIC;
        expr1->numeric_t.num = malloc(sizeof(numeric_s));
        expr1->numeric_t.num->tag = NUMERIC_OPERATOR;
        expr1->numeric_t.num->numeric_operator_t.num1 = expression;
        expr1->numeric_t.num->numeric_operator_t.op = MULTIPLY;
        expr1->numeric_t.num->numeric_operator_t.num2 = parseExpression(tokens);

        return expr1;
    } else if (!strcmp(next, "/")) {
        popToken(tokens);

        expression_s* expr1 = malloc(sizeof(expression_s));
        expr1->tag = NUMERIC;
        expr1->numeric_t.num = malloc(sizeof(numeric_s));
        expr1->numeric_t.num->tag = NUMERIC_OPERATOR;
        expr1->numeric_t.num->numeric_operator_t.num1 = expression;
        expr1->numeric_t.num->numeric_operator_t.op = DIVIDE;
        expr1->numeric_t.num->numeric_operator_t.num2 = parseExpression(tokens);

        return expr1;
    } else if (!strcmp(next, "%%")) {
        popToken(tokens);

        expression_s* expr1 = malloc(sizeof(expression_s));
        expr1->tag = NUMERIC;
        expr1->numeric_t.num = malloc(sizeof(numeric_s));
        expr1->numeric_t.num->tag = NUMERIC_OPERATOR;
        expr1->numeric_t.num->numeric_operator_t.num1 = expression;
        expr1->numeric_t.num->numeric_operator_t.op = MOD;
        expr1->numeric_t.num->numeric_operator_t.num2 = parseExpression(tokens);

        return expr1;
    } else if (!strcmp(next, "=")) {
        popToken(tokens);

        expression_s* expr1 = malloc(sizeof(expression_s));
        expr1->tag = BOOLEAN;
        expr1->boolean_t.bool = malloc(sizeof(boolean_s));
        expression->boolean_t.bool->tag = EQUALS;
        expression->boolean_t.bool->equals_t.expr1 = expression;
        expression->boolean_t.bool->equals_t.expr2 = parseExpression(tokens);

        return expr1;
    } else if (!strcmp(next, "&")) {
        popToken(tokens);

        expression_s* expr1 = malloc(sizeof(expression_s));
        expr1->tag = BOOLEAN;
        expr1->boolean_t.bool = malloc(sizeof(boolean_s));
        expression->boolean_t.bool->tag = AND;
        expression->boolean_t.bool->and_t.expr1 = expression;
        expression->boolean_t.bool->and_t.expr2 = parseExpression(tokens);

        return expr1;
    } else if (!strcmp(next, "|")) {
        popToken(tokens);

        expression_s* expr1 = malloc(sizeof(expression_s));
        expr1->tag = BOOLEAN;
        expr1->boolean_t.bool = malloc(sizeof(boolean_s));
        expression->boolean_t.bool->tag = OR;
        expression->boolean_t.bool->or_t.expr1 = expression;
        expression->boolean_t.bool->or_t.expr2 = parseExpression(tokens);

        return expr1;
    }

    return expression;
}

bindingIdentifier_s* parseBindingIdentifier(tokenData* tokens) {
    bindingIdentifier_s* bindingIdentifier = malloc(sizeof(bindingIdentifier_s));

    char* id1 = popToken(tokens);

    if (!isIdentifier(id1)) {
        syntaxError();
    }

    if (!isIdentifier(peekToken(tokens))) {
        bindingIdentifier->tag = NOT_TYPED;
        bindingIdentifier->not_typed_t.id->identifier = id1;
    } else {
        bindingIdentifier->tag = TYPED;
        bindingIdentifier->typed_t.type->identifier = id1;

        char* id2 = popToken(tokens);

        if (!isIdentifier(id2)) {
            syntaxError();
        }

        bindingIdentifier->typed_t.id->identifier = id2;
    }

    return bindingIdentifier;
}

list_s* parseList(tokenData* tokens) {
    list_s* list = malloc(sizeof(list_s));

    if (!strcmp(peekToken(tokens), "]")) {
        list->tag = EMPTY;
        list->empty_t.placeholder = 1;
    } else {
        list->tag = NOT_EMPTY;
        list->not_empty_t.exprs = parseMultiExpressions(tokens);
    }

    popToken(tokens);

    return list;
}

multipleExpressions_s* parseMultiExpressions(tokenData* tokens) {
    multipleExpressions_s* multiExprs = malloc(sizeof(multipleExpressions_s));

    expression_s* expr = parseExpression(tokens);

    if (!strcmp(peekToken(tokens), "]")) {
        multiExprs->tag = SINGLE;
        multiExprs->single_t.expr = expr;
    } else if (!strcmp(peekToken(tokens), ",")) {
        multiExprs->tag = MULTI;
        multiExprs->multi_t.first = expr;
        multiExprs->multi_t.rest = parseMultiExpressions(tokens);
    } else {
        syntaxError();
    }

    return multiExprs;
}

functionDeclaration_s* parseFunctionDeclaration(tokenData* tokens) {
    functionDeclaration_s* func = malloc(sizeof(functionDeclaration_s));

    char* token = popToken(tokens);

    if (!strcmp(token, "[")) {
        func->tag = NOT_TYPED;
        func->not_typed_t.list = parseList(tokens);

        if (strcmp(peekToken(tokens), ":")) {
            syntaxError();
        }

        popToken(tokens);

        func->not_typed_t.stmnt = parseStatement(tokens);
    } else if (isIdentifier(token)) {
        func->tag = TYPED;
        func->typed_t.id = malloc(sizeof(identifier_s));
        func->typed_t.id->identifier = token;

        if (strcmp(peekToken(tokens), "[")) {
            syntaxError();
        }

        popToken(tokens);

        func->typed_t.list = parseList(tokens);

        if (strcmp(peekToken(tokens), ":")) {
            syntaxError();
        }

        popToken(tokens);

        func->typed_t.stmnt = parseStatement(tokens);
    } else {
        syntaxError();
    }

    return func;
}

numberData_s* parseNumberData(tokenData* tokens) {  
    numberData_s* num = malloc(sizeof(numberData_s));

    char* token = peekToken(tokens);
    int type = isNum(token);
    
    if (type == 1) {
        num->type = INT;
        num->integer = atoi(token);
    } else if (type == -1) {
        num->type = DOUBLE;
        num->decimal = atof(token);
    } else {
        syntaxError();
    }

    return num;
}

void syntaxError() {
    printf("Parsing failure: invalid syntax.\n");

    exit(EXIT_FAILURE);
}

int isKeyword(char* token) {
    if (!strcmp(token, "if")) {
        return 1;
    } else if (!strcmp(token, "else")) {
        return 1;
    } else if (!strcmp(token, "while")) {
        return 1;
    } else if (!strcmp(token, "pass")) {
        return 1;
    } else if (!strcmp(token, "true")) {
        return 1;
    } else if (!strcmp(token, "false")) {
        return 1;
    } else if (!strcmp(token, "def")) {
        return 1;
    } else if (!strcmp(token, "define")) {
        return 1;
    } else if (!strcmp(token, "exe")) {
        return 1;
    } else if (!strcmp(token, "execute")) {
        return 1;
    } else if (!strcmp(token, "evl")) {
        return 1;
    } else if (!strcmp(token, "evaluate")) {
        return 1;
    } else if (!strcmp(token, "fnc")) {
        return 1;
    } else if (!strcmp(token, "function")) {
        return 1;
    } else if (!strcmp(token, "rtn")) {
        return 1;
    } else if (!strcmp(token, "return")) {
        return 1;
    }

    return 0;
}

// Returns 1 if int, -1 if double, or 0 if invalid
int isNum(char* token) {
    int hasSign = 0;
    int hasDecimal = 0;

    if (token[0] == '-' || token[0] == '+') {
        hasSign = 1;
    }

    for (int i = hasSign; i < strlen(token); i++) {
        if (token[i] == '.') {
            if (hasDecimal) {
                return 0;
            } else {
                hasDecimal = 1;
            }
        } else {
            if (!isdigit(token[i])) {
                return 0;
            }
        }
    }

    if (hasDecimal) {
        return -1;
    } else {
        return 1;
    }
}

int isIdentifier(char* token) {
    if (!isNum(token) && !isKeyword(token)) {
        for (int i = 0; i < strlen(token); i++) {
            if (isPunctuation(token[i])) {
                return 0;
            }
        }

        return 1;
    }
    
    return 0;
}