#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "to_string.h"

int isPunctuation(char c) {
    char* punctuation = ":<>()[]{}+-*/%%!=&|,";

    return ((strchr(punctuation, c) != NULL) && c != '\0');
}

// Removes the character at the target index from the string
// Returns modified copy of input, does not mutate
char* removeChar(char* original, int index) {
    int length = strlen(original);
    char* returnStr = malloc(length + 1);

    strncpy(returnStr, original, length + 1);

    memmove(&returnStr[index], &returnStr[index + 1], length - index);

    return returnStr;
}

// Changes all standard whitespace characters to space and removes unnecessary whitespace
// Returns modified copy of input, does not mutate
char* normalizeWhitespace(char* original) {
    int length = strlen(original);
    char* returnStr = malloc(length + 1);

    // Copies original to returnStr while changing all whitespace characters to spaces
    for (int i = 0; i < length; i++) {
        if (isspace(original[i])) {
            returnStr[i] = ' ';
        } else {
            returnStr[i] = original[i];
        }
    }

    returnStr[length] = '\0';

    // Chops off any leading spaces
    while (returnStr[0] == ' ') {
        returnStr = removeChar(returnStr, 0);
    }

    // Removes unnecessary spaces from the rest of the string
    char previousChar = returnStr[0];

    for (int j = 1; returnStr[j] != '\0'; j++) {
        if (returnStr[j] == ' ' && previousChar == ' ') {
            returnStr = removeChar(returnStr, j);

            j -= 1;
        }

        previousChar = returnStr[j];
    }

    // Chops off the trailing space if there is one
    length = strlen(returnStr) - 1;

    if (returnStr[length] == ' ') {
        returnStr = removeChar(returnStr, length);
    }

    return returnStr;
}

// Representation functions

char* numberDataRepr(numberData_s* param) {
    char* typeName;
    int paramlength;
    char* paramStr;
    int returnLength;
    char* returnStr;

    if (param->type == INT) {
        typeName = "integer";

        paramlength = snprintf(NULL, 0, "%d", param->integer) + 1;
        paramStr = malloc(paramlength);

        snprintf(paramStr, paramlength, "%d", param->integer);
    } else if (param->type == DOUBLE) {
        typeName = "double";

        paramlength = snprintf(NULL, 0, "%lf", param->decimal) + 1;
        paramStr = malloc(paramlength);
        
        snprintf(paramStr, paramlength, "%lf", param->decimal);
    } else {
        return "Error: invalid or unset tag";
    }

    returnLength = strlen("NumberData(): []") + strlen(typeName) + paramlength;
    returnStr = malloc(returnLength);

    snprintf(returnStr, returnLength, "NumberData(%s): [%s]", typeName, paramStr);
    return returnStr;
}

char* programRepr(program_s* param) {
    char* multStmntStr = multipleStatementsRepr(param->stmnts);
    int returnLength = strlen("Program: []") + strlen(multStmntStr) + 1;
    char* returnStr = malloc(returnLength);

    snprintf(returnStr, returnLength, "Program: [%s]", multStmntStr);
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

        snprintf(returnStr, returnLength, "MultipleStatements(%s): [%s]", tagName, stmntStr);
    } else if (param->tag == MULTI) {
        tagName = "multi";

        char* firstStr = statementRepr(param->multi_t.first);
        char* restStr = multipleStatementsRepr(param->multi_t.rest);
        
        returnLength = strlen("MultipleStatements(): [, ]") + strlen(tagName) + strlen(firstStr) + strlen(restStr) + 1;
        returnStr = malloc(returnLength);

        snprintf(returnStr, returnLength, "MultipleStatements(%s): [%s, %s]", tagName, firstStr, restStr);
    } else {
        return "Error: invalid or unset tag";
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

        snprintf(returnStr, returnLength, "Statement(%s): [%s]", tagName, multStmntStr);
    } else if (param->tag == DEFINE) {
        tagName = "define";

        char* bidStr = bindingIdentifierRepr(param->define_t.bid);
        char* exprStr = expressionRepr(param->define_t.expr);

        returnLength = strlen("Statement(): [, ]") + strlen(tagName) + strlen(bidStr) + strlen(exprStr) + 1;
        returnStr = malloc(returnLength);

        snprintf(returnStr, returnLength, "Statement(%s): [%s, %s]", tagName, bidStr, exprStr);
    } else if (param->tag == EXECUTE) {
        tagName = "execute";

        char* idStr = identifierRepr(param->execute_t.id);
        char* listStr = listRepr(param->execute_t.list);

        returnLength = strlen("Statement(): [, ]") + strlen(tagName) + strlen(idStr) + strlen(listStr) + 1;
        returnStr = malloc(returnLength);

        snprintf(returnStr, returnLength, "Statement(%s): [%s, %s]", tagName, idStr, listStr);
    } else if (param->tag == RETURN) {
        tagName = "return";

        char* exprStr = expressionRepr(param->return_t.expr);

        returnLength = strlen("Statement(): []") + strlen(tagName) + strlen(exprStr) + 1;
        returnStr = malloc(returnLength);

        snprintf(returnStr, returnLength, "Statement(%s): [%s]", tagName, exprStr);
    } else if (param->tag == IF) {
        tagName = "if";

        char* ifStr = ifRepr(param->if_t.stmnt);

        returnLength = strlen("Statement(): []") + strlen(tagName) + strlen(ifStr) + 1;
        returnStr = malloc(returnLength);

        snprintf(returnStr, returnLength, "Statement(%s): [%s]", tagName, ifStr);
    } else if (param->tag == WHILE) {
        tagName = "while";
        
        char* exprStr = expressionRepr(param->while_t.expr);
        char* stmntStr = statementRepr(param->while_t.stmnt);

        returnLength = strlen("Statement(): [, ]") + strlen(tagName) + strlen(exprStr) + strlen(stmntStr) + 1;
        returnStr = malloc(returnLength);

        snprintf(returnStr, returnLength, "Statement(%s): [%s, %s]", tagName, exprStr, stmntStr);
    } else if (param->tag == PASS) {
        tagName = "pass";
        returnLength = strlen("Statement()") + strlen(tagName) + 1;
        returnStr = malloc(returnLength);

        snprintf(returnStr, returnLength, "Statement(%s)", tagName);
    } else {
        return "Error: invalid or unset tag";
    }

    return returnStr;
}

char* ifRepr(if_s* param) {
    char* tagName;
    int returnLength;
    char* returnStr;

    if (param->tag == IF_ONLY) {
        tagName = "if only";

        char* exprStr = expressionRepr(param->if_only_t.ifExpr);
        char* stmntStr = statementRepr(param->if_only_t.ifStmnt);

        returnLength = strlen("If(): [, ]") + strlen(tagName) + strlen(exprStr) + strlen(stmntStr) + 1;
        returnStr = malloc(returnLength);

        snprintf(returnStr, returnLength, "If(%s): [%s, %s]", tagName, exprStr, stmntStr);
    } else if (param->tag == IF_ELSE) {
        tagName = "if else";

        char* ifExprStr = expressionRepr(param->if_else_t.ifExpr);
        char* ifStmntStr = statementRepr(param->if_else_t.ifStmnt);
        char* elseStmntStr = statementRepr(param->if_else_t.elseStmnt);

        returnLength = strlen("If(): [, , ]") + strlen(tagName)
                     + strlen(ifExprStr) + strlen(ifStmntStr) + strlen(elseStmntStr) + 1;
        returnStr = malloc(returnLength);

        snprintf(returnStr, returnLength, "If(%s): [%s, %s, %s]", tagName, ifExprStr, ifStmntStr, elseStmntStr);
    } else if (param->tag == IF_ELSEIF) {
        tagName = "if elseif";

        char* ifExprStr = expressionRepr(param->if_elseif_t.ifExpr);
        char* ifStmntStr = statementRepr(param->if_elseif_t.ifStmnt);
        char* chainStr = elseIfChainRepr(param->if_elseif_t.chain);

        returnLength = strlen("If(): [, , ]") + strlen(tagName)
                     + strlen(ifExprStr) + strlen(ifStmntStr) + strlen(chainStr) + 1;
        returnStr = malloc(returnLength);

        snprintf(returnStr, returnLength, "If(%s): [%s, %s, %s]", tagName, ifExprStr, ifStmntStr, chainStr);
    } else if (param->tag == IF_ELSEIF_ELSE) {
        tagName = "if elseif else";

        char* ifExprStr = expressionRepr(param->if_elseif_t.ifExpr);
        char* ifStmntStr = statementRepr(param->if_elseif_t.ifStmnt);
        char* chainStr = elseIfChainRepr(param->if_elseif_t.chain);
        char* elseStmntStr = statementRepr(param->if_else_t.elseStmnt);

        returnLength = strlen("If(): [, , , ]") + strlen(tagName)
                     + strlen(ifExprStr) + strlen(ifStmntStr) + strlen(chainStr) + strlen(elseStmntStr) + 1;
        returnStr = malloc(returnLength);

        snprintf(returnStr, returnLength, "If(%s): [%s, %s, %s, %s]", tagName, ifExprStr, ifStmntStr, chainStr, elseStmntStr);
    } else {
        return "Error: invalid or unset tag";
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

        snprintf(returnStr, returnLength, "ElseIfChain(%s): [%s, %s]", tagName, exprStr, stmntStr);
    } else if (param->tag == MULTI) {
        tagName = "multi";

        char* ifExprStr = expressionRepr(param->multi_t.ifExpr);
        char* ifStmntStr = statementRepr(param->multi_t.ifStmnt);
        char* chainStr = elseIfChainRepr(param->multi_t.chain);

        returnLength = strlen("ElseIfChain(): [, , ]") + strlen(tagName)
                     + strlen(ifExprStr) + strlen(ifStmntStr) + strlen(chainStr) + 1;
        returnStr = malloc(returnLength);

        snprintf(returnStr, returnLength, "ElseIfChain(%s): [%s, %s, %s]", tagName, ifExprStr, ifStmntStr, chainStr);
    } else {
        return "Error: invalid or unset tag";
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

        snprintf(returnStr, returnLength, "Expression(%s): [%s]", tagName, exprStr);
    } else if (param->tag == IDENTIFIER) {
        tagName = "identifier";

        char* idStr = identifierRepr(param->identifier_t.id);

        returnLength = strlen("Expression(): []") + strlen(tagName) + strlen(idStr) + 1;
        returnStr = malloc(returnLength);

        snprintf(returnStr, returnLength, "Expression(%s): [%s]", tagName, idStr);
    } else if (param->tag == NUMERIC) {
        tagName = "numeric";

        char* numStr = numericRepr(param->numeric_t.num);

        returnLength = strlen("Expression(): []") + strlen(tagName) + strlen(numStr) + 1;
        returnStr = malloc(returnLength);

        snprintf(returnStr, returnLength, "Expression(%s): [%s]", tagName, numStr);
    } else if (param->tag == BOOLEAN) {
        tagName = "boolean";

        char* boolStr = booleanRepr(param->boolean_t.bool);

        returnLength = strlen("Expression(): []") + strlen(tagName) + strlen(boolStr) + 1;
        returnStr = malloc(returnLength);

        snprintf(returnStr, returnLength, "Expression(%s): [%s]", tagName, boolStr);
    } else if (param->tag == CHARACTER) {
        tagName = "character";

        char* chrStr = characterRepr(param->character_t.chr);

        returnLength = strlen("Expression(): []") + strlen(tagName) + strlen(chrStr) + 1;
        returnStr = malloc(returnLength);

        snprintf(returnStr, returnLength, "Expression(%s): [%s]", tagName, chrStr);
    } else if (param->tag == LIST) {
        tagName = "list";

        char* listStr = listRepr(param->list_t.list);

        returnLength = strlen("Expression(): []") + strlen(tagName) + strlen(listStr) + 1;
        returnStr = malloc(returnLength);

        snprintf(returnStr, returnLength, "Expression(%s): [%s]", tagName, listStr);
    } else if (param->tag == EVALUATE) {
        tagName = "evaluate";

        char* idStr = identifierRepr(param->evaluate_t.id);
        char* listStr = listRepr(param->evaluate_t.list);

        returnLength = strlen("Expression(): [, ]") + strlen(tagName) + strlen(idStr) + strlen(listStr) + 1;
        returnStr = malloc(returnLength);

        snprintf(returnStr, returnLength, "Expression(%s): [%s, %s]", tagName, idStr, listStr);
    } else if (param->tag == FUNCTION) {
        tagName = "function";

        char* funcStr = functionDeclarationRepr(param->function_t.func);

        returnLength = strlen("Expression(): []") + strlen(tagName) + strlen(funcStr) + 1;
        returnStr = malloc(returnLength);

        snprintf(returnStr, returnLength, "Expression(%s): [%s]", tagName, funcStr);
    } else {
        return "Error: invalid or unset tag";
    }

    return returnStr;
}

char* numericRepr(numeric_s* param) {
    char* tagName;
    int returnLength;
    char* returnStr;

    if (param->tag == NUMBER) {
        tagName = "number";

        char* numStr = numberDataRepr(param->number_t.num);

        returnLength = strlen("Numeric(): []") + strlen(tagName) + strlen(numStr) + 1;
        returnStr = malloc(returnLength);

        snprintf(returnStr, returnLength, "Numeric(%s): [%s]", tagName, numStr);
    } else if (param->tag == NUMERIC_OPERATOR) {
        tagName = "numeric operator";

        char* num1Str = expressionRepr(param->numeric_operator_t.num1);
        char* num2Str = expressionRepr(param->numeric_operator_t.num2);

        char* opStr;
        switch (param->numeric_operator_t.op) {
            case ADD:
                opStr = "Add";
                break;
            case SUBTRACT:
                opStr = "Subtract";
                break;
            case MULTIPLY:
                opStr = "Multiply";
                break;
            case DIVIDE:
                opStr = "Divide";
                break;
            case MOD:
                opStr = "Mod";
                break;
            default:
                opStr = "Error: invalid or unset operator";
        };

        returnLength = strlen("NumericOperator(): [, ]") + strlen(tagName) + strlen(num1Str) + strlen(num2Str) + strlen(opStr) + 2;
        returnStr = malloc(returnLength);

        snprintf(returnStr, returnLength, "NumericOperator(%s): [%s, %s, %s]", tagName, num1Str, num2Str, opStr);
    } else {
        return "Error: invalid or unset tag";
    }

    return returnStr;
}

char* booleanRepr(boolean_s* param) {
    char* tagName;
    int returnLength;
    char* returnStr;

    if (param->tag == TRUE) {
        tagName = "true";

        returnLength = strlen("Boolean()") + strlen(tagName) + 1;
        returnStr = malloc(returnLength);

        snprintf(returnStr, returnLength, "Boolean(%s)", tagName);
    } else if (param->tag == FALSE) {
        tagName = "false";

        returnLength = strlen("Boolean()") + strlen(tagName) + 1;
        returnStr = malloc(returnLength);

        snprintf(returnStr, returnLength, "Boolean(%s)", tagName);
    } else if (param->tag == NOT) {
        tagName = "not";

        char* exprStr = expressionRepr(param->not_t.expr);

        returnLength = strlen("Boolean(): []") + strlen(tagName) + strlen(exprStr) + 1;
        returnStr = malloc(returnLength);

        snprintf(returnStr, returnLength, "Boolean(%s): [%s]", tagName, exprStr);
    } else if (param->tag == OR) {
        tagName = "or";

        char* expr1Str = expressionRepr(param->or_t.expr1);
        char* expr2Str = expressionRepr(param->or_t.expr2);

        returnLength = strlen("Boolean(): [, ]") + strlen(tagName) + strlen(expr1Str) + strlen(expr2Str) + 1;
        returnStr = malloc(returnLength);

        snprintf(returnStr, returnLength, "Boolean(%s): [%s, %s]", tagName, expr1Str, expr2Str);
    } else if (param->tag == AND) {
        tagName = "and";

        char* expr1Str = expressionRepr(param->and_t.expr1);
        char* expr2Str = expressionRepr(param->and_t.expr2);

        returnLength = strlen("Boolean(): [, ]") + strlen(tagName) + strlen(expr1Str) + strlen(expr2Str) + 1;
        returnStr = malloc(returnLength);

        snprintf(returnStr, returnLength, "Boolean(%s): [%s, %s]", tagName, expr1Str, expr2Str);
    } else if (param->tag == EQUALS) {
        tagName = "equals";

        char* expr1Str = expressionRepr(param->equals_t.expr1);
        char* expr2Str = expressionRepr(param->equals_t.expr2);

        returnLength = strlen("Boolean(): [, ]") + strlen(tagName) + strlen(expr1Str) + strlen(expr2Str) + 1;
        returnStr = malloc(returnLength);

        snprintf(returnStr, returnLength, "Boolean(%s): [%s, %s]", tagName, expr1Str, expr2Str);
    } else {
        return "Error: invalid or unset tag";
    }

    return returnStr;
}

char* listRepr(list_s* param) {
    char* tagName;
    int returnLength;
    char* returnStr;

    switch (param->tag) {
        case EMPTY:
            tagName = "empty";
            returnLength = strlen("List()") + strlen(tagName) + 1;
            returnStr = malloc(returnLength);

            snprintf(returnStr, returnLength, "List(%s)", tagName);
            break;
        case NOT_EMPTY:
            tagName = "not empty";

            char* exprs = multipleExpressionsRepr(param->not_empty_t.exprs);

            returnLength = strlen("List() []") + strlen(tagName) + strlen(exprs) + 1;
            returnStr = malloc(returnLength);

            snprintf(returnStr, returnLength, "List(%s) [%s]", tagName, exprs);
            break;
        default:
            return "Error: invalid or unset tag";
    }

    return returnStr;
}

char* multipleExpressionsRepr(multipleExpressions_s* param) {
    char* tagName;
    int returnLength;
    char* returnStr;

    switch (param->tag) {
        case SINGLE:
            tagName = "single";

            char* expr = expressionRepr(param->single_t.expr);

            returnLength = strlen("MultipleExpressions() []") + strlen(tagName) + strlen(expr) + 1;
            returnStr = malloc(returnLength);

            snprintf(returnStr, returnLength, "MultipleExpressions(%s) [%s]", tagName, expr);
            break;
        case MULTI:
            tagName = "multi";

            char* first = expressionRepr(param->multi_t.first);
            char* rest = multipleExpressionsRepr(param->multi_t.rest);

            returnLength = strlen("MultipleExpressions() [, ]") + strlen(tagName) + strlen(first) + strlen(rest) + 1;
            returnStr = malloc(returnLength);

            snprintf(returnStr, returnLength, "MultipleExpressions(%s) [%s, %s]", tagName, first, rest);
            break;
        default:
            return "Error: invalid or unset tag";
    }

    return returnStr;
}

char* functionDeclarationRepr(functionDeclaration_s* param) {
    char* tagName;
    int returnLength;
    char* returnStr;
    
    switch (param->tag) {
        case NOT_TYPED:
            tagName = "not typed";

            char* list = listRepr(param->not_typed_t.list);
            char* stmnt = statementRepr(param->not_typed_t.stmnt);

            returnLength = strlen("FunctionDeclaration() [, ]") + strlen(tagName) + strlen(list) + strlen(stmnt) + 1;
            returnStr = malloc(returnLength);

            snprintf(returnStr, returnLength, "FunctionDeclaration(%s) [%s, %s]", tagName, list, stmnt);
            break;
        case TYPED:
            tagName = "typed";
            
            char* id = identifierRepr(param->typed_t.id);
            char* list_ = listRepr(param->typed_t.list);
            char* stmnt_ = statementRepr(param->typed_t.stmnt);

            returnLength = strlen("FunctionDeclaration() [, , ]") + strlen(tagName) + strlen(id) + strlen(list_) + strlen(stmnt_) + 1;
            returnStr = malloc(returnLength);

            snprintf(returnStr, returnLength, "FunctionDeclaration(%s) [%s, %s, %s]", tagName, id, list_, stmnt_);
            break;
        default:
            return "Error: invalid or unset tag";
    }

    return returnStr;
}


char* bindingIdentifierRepr(bindingIdentifier_s* param) {
    char* tagName;
    int returnLength;
    char* returnStr;

    switch (param->tag) {
        case NOT_TYPED:
            tagName = "not typed";

            char* id = identifierRepr(param->not_typed_t.id);

            returnLength = strlen("BindingIdentifier() []") + strlen(tagName) + strlen(id) + 1;
            returnStr = malloc(returnLength);

            snprintf(returnStr, returnLength, "BindingIdentifier(%s) [%s]", tagName, id);
            break;
        case TYPED:
            tagName = "typed";
            
            char* type = identifierRepr(param->typed_t.type);
            char* id_ = identifierRepr(param->typed_t.id);

            returnLength = strlen("BindingIdentifier() [, , ]") + strlen(tagName) + strlen(type) + strlen(id_) + 1;
            returnStr = malloc(returnLength);

            snprintf(returnStr, returnLength, "BindingIdentifier(%s) [%s, %s]", tagName, type, id_);
            break;
        default:
            return "Error: invalid or unset tag";
    }

    return returnStr;
}

char* characterRepr(character_s* param) {
    int returnLength = strlen("Character: []") + 2;
    char* returnStr = malloc(returnLength);

    snprintf(returnStr, returnLength, "Character: [%c]", *(param->character));
    return returnStr;
}

char* identifierRepr(identifier_s* param) {
    int returnLength = strlen("Identifier: []") + strlen(param->identifier) + 1;
    char* returnStr = malloc(returnLength);

    snprintf(returnStr, returnLength, "Identifier: [%s]", param->identifier);
    return returnStr;
}

char* parseTreeRepr(parseTree* param) {
    switch (param->type) {
        case NUMBER_DATA:
            return numberDataRepr(param->tree.numberDataValue);
            break;
        case PROGRAM:
            return programRepr(param->tree.programValue);
            break;
        case MULTIPLE_STATEMENTS:
            return multipleStatementsRepr(param->tree.multipleStatementsValue);
            break;
        case STATEMENT:
            return statementRepr(param->tree.statementValue);
            break;
        case IF:
            return ifRepr(param->tree.ifValue);
            break;
        case ELSE_IF_CHAIN:
            return elseIfChainRepr(param->tree.elseIfChainValue);
            break;
        case EXPRESSION:
            return expressionRepr(param->tree.expressionValue);
            break;
        case NUMERIC:
            return numericRepr(param->tree.numericValue);
            break;
        case BOOLEAN:
            return booleanRepr(param->tree.booleanValue);
            break;
        case LIST:
            return listRepr(param->tree.listValue);
            break;
        case MULTIPLE_EXPRESSIONS:
            return multipleExpressionsRepr(param->tree.multipleExpressionsValue);
            break;
        case FUNCTION_DECLARATION:
            return functionDeclarationRepr(param->tree.functionDeclarationValue);
            break;
        case BINDING_IDENTIFIER:
            return bindingIdentifierRepr(param->tree.bindingIdentifierValue);
            break;
        case CHARACTER:
            return characterRepr(param->tree.characterValue);
            break;
        case IDENTIFIER:
            return identifierRepr(param->tree.identifierValue);
            break;
        default:
            return "Error: invalid or unset tag";
    }
}