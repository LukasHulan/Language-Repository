#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parse_tree.h"
#include "tokenizer.h"

parseTree* parse(tokenData* tokens) {
    parseTree* programTree = malloc(sizeof(parseTree));
    programTree->type = PROGRAM;
    programTree->tree.programValue = parseProgram(tokens);
}

program_s* parseProgram(tokenData* tokens) {
    program_s* program = malloc(sizeof(program_s));
    program->stmnts = parseInitialStatements(tokens);
}

multipleStatements_s* parseInitialStatements(tokenData* tokens) {
    multipleStatements_s* mulitStmnts = malloc(sizeof(multipleStatements_s));
    mulitStmnts->tag = SINGLE;
    mulitStmnts->single_t.stmnt = parseStatement(tokens);

    if (tokens->index < tokens->length) {
        mulitStmnts->tag = MULTI;
        mulitStmnts->multi_t.first = mulitStmnts->single_t.stmnt;
        mulitStmnts->multi_t.rest = parseMultiStatements(tokens);
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
        stmnt->execute_t.id = parseIdentifier(tokens);

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

// To do
expression_s* parseExpression(tokenData* tokens) {
    return malloc(sizeof(expression_s));
}

// To do
bindingIdentifier_s* parseBindingIdentifier(tokenData* tokens) {
    return malloc(sizeof(bindingIdentifier_s));
}

//To do
identifier_s* parseIdentifier(tokenData* tokens) {
    return malloc(sizeof(identifier_s));
}

// To do
list_s* parseList(tokenData* tokens) {
    return malloc(sizeof(list_s));
}

void syntaxError() {
    printf("Parsing failure: invalid syntax.\n");

    exit(EXIT_FAILURE);
}