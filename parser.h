#ifndef PARSER
#define PARSER

#include "parse_tree.h"
#include "tokenizer.h"

parseTree* parse(tokenData* tokens);
program_s* parseProgram(tokenData* tokens);
multipleStatements_s* parseInitialStatements(tokenData* tokens);
multipleStatements_s* parseMultiStatements(tokenData* tokens);
statement_s* parseStatement(tokenData* tokens);
if_s* parseIf(tokenData* tokens);
elseIfChain_s* parseElseIfChain(tokenData* tokens);
expression_s* parseExpression(tokenData* tokens);
bindingIdentifier_s* parseBindingIdentifier(tokenData* tokens);
list_s* parseList(tokenData* tokens);
multipleExpressions_s* parseMultiExpressions(tokenData* tokens);
functionDeclaration_s* parseFunctionDeclaration(tokenData* tokens);
numberData_s* parseNumberData(tokenData* tokens);

void syntaxError();

int isKeyword(char* token);
int isNum(char* token);
int isIdentifier(char* token);

#endif