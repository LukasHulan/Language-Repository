#include <stdio.h> 
#include <stdlib.h>
#include <string.h>

#include <parse_tree.h>
#include <to_string.h>

int ptcmp(parseTree* first, parseTree* second) {
    if (first->type != second->type) {
        return 0;
    }

    char* firstStr;
    char* secondStr;

    switch (first->type) {
        case NUMBER_DATA:
            firstStr = numberDataRepr(&(first->tree->numberDataValue));
            secondStr = numberDataRepr(&(second->tree->numberDataValue));
            break;
        case PROGRAM:
            firstStr = programRepr(&(first->tree->programValue));
            secondStr = programRepr(&(second->tree->programValue));
            break;
        case MULTIPLE_STATEMENTS:
            firstStr = multipleStatementsRepr(&(first->tree->multipleStatementsValue));
            secondStr = multipleStatementsRepr(&(second->tree->multipleStatementsValue));
            break;
        case STATEMENT:
            firstStr = statementRepr(&(first->tree->statementValue));
            secondStr = statementRepr(&(second->tree->statementValue));
            break;
        case IF:
            firstStr = ifRepr(&(first->tree->ifValue));
            secondStr = ifRepr(&(second->tree->ifValue));
            break;
        case ELSE_IF_CHAIN:
            firstStr = elseIfChainRepr(&(first->tree->elseIfChainValue));
            secondStr = elseIfChainRepr(&(second->tree->elseIfChainValue));
            break;
        case EXPRESSION:
            firstStr = expressionRepr(&(first->tree->expressionValue));
            secondStr = expressionRepr(&(second->tree->expressionValue));
            break;
        case NUMERIC:
            firstStr = numericRepr(&(first->tree->numericValue));
            secondStr = numericRepr(&(second->tree->numericValue));
            break;
        case BOOLEAN:
            firstStr = booleanRepr(&(first->tree->booleanValue));
            secondStr = booleanRepr(&(second->tree->booleanValue));
            break;
        case LIST:
            firstStr = listRepr(&(first->tree->listValue));
            secondStr = listRepr(&(second->tree->listValue));
            break;
        case MULTIPLE_EXPRESSIONS:
            firstStr = multipleExpressionsRepr(&(first->tree->multipleExpressionsValue));
            secondStr = multipleExpressionsRepr(&(second->tree->multipleExpressionsValue));
            break;
        case FUNCTION_DECLARATION:
            firstStr = functionDeclarationRepr(&(first->tree->functionDeclarationValue));
            secondStr = functionDeclarationRepr(&(second->tree->functionDeclarationValue));
            break;
        case BINDING_IDENTIFIER:
            firstStr = bindingIdentifierRepr(&(first->tree->bindingIdentifierValue));
            secondStr = bindingIdentifierRepr(&(second->tree->bindingIdentifierValue));
            break;
        case CHARACTER:
            firstStr = characterRepr(&(first->tree->characterValue));
            secondStr = characterRepr(&(second->tree->characterValue));
            break;
        case IDENTIFIER:
            firstStr = identifierRepr(&(first->tree->identifierValue));
            secondStr = identifierRepr(&(second->tree->identifierValue));
            break;
        default:
            printf("Error: invalid or unset type\n");
            return 0;
    }

    return strcmp(firstStr, secondStr);
}