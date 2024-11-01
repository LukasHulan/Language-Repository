#ifndef TO_STRING
#define TO_STRING

#include <parse_tree.h>

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

char* numberDataToString(numberData_s* param);
char* programToString(program_s* param);
char* multipleStatementsToString(multipleStatements_s* param);
char* statementToString(statement_s* param);
char* ifToString(if_s* param);
char* elseIfChainToString(elseIfChain_s* param);
char* expressionToString(expression_s* param);
char* numericToString(numeric_s* param);
char* booleanToString(boolean_s* param);
char* listToString(list_s* param);
char* multipleExpressionsToString(multipleExpressions_s* param);
char* functionDeclarationToString(functionDeclaration_s* param);
char* bindingIdentifierToString(bindingIdentifier_s* param);
char* characterToString(character_s* param);
char* identifierToString(identifier_s* param);

char* normalizeWhitespace(char* original, int length);

#endif