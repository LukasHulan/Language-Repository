#include <stdio.h> 
#include <stdlib.h>
#include <string.h>

#include <to_string.h>

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

    returnLength = strlen("NumberData(): []") + paramlength;
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
        tagName = "if_only";

        char* exprStr = expressionRepr(param->if_only_t.ifExpr);
        char* stmntStr = statementRepr(param->if_only_t.ifStmnt);

        returnLength = strlen("If(): [, ]") + strlen(tagName) + strlen(exprStr) + strlen(stmntStr) + 1;
        returnStr = malloc(returnLength);

        snprintf(returnStr, returnLength, "If(%s): [%s, %s]", tagName, exprStr, stmntStr);
    } else if (param->tag == IF_ELSE) {
        tagName = "if_else";

        char* ifExprStr = expressionRepr(param->if_else_t.ifExpr);
        char* ifStmntStr = statementRepr(param->if_else_t.ifStmnt);
        char* elseStmntStr = statementRepr(param->if_else_t.elseStmnt);

        returnLength = strlen("If(): [, , ]") + strlen(tagName)
                     + strlen(ifExprStr) + strlen(ifStmntStr) + strlen(elseStmntStr) + 1;
        returnStr = malloc(returnLength);

        snprintf(returnStr, returnLength, "If(%s): [%s, %s, %s]", tagName, ifExprStr, ifStmntStr, elseStmntStr);
    } else if (param->tag == IF_ELSEIF) {
        tagName = "if_elseif";

        char* ifExprStr = expressionRepr(param->if_elseif_t.ifExpr);
        char* ifStmntStr = statementRepr(param->if_elseif_t.ifStmnt);
        char* chainStr = statementRepr(param->if_elseif_t.chain);

        returnLength = strlen("If(): [, , ]") + strlen(tagName)
                     + strlen(ifExprStr) + strlen(ifStmntStr) + strlen(chainStr) + 1;
        returnStr = malloc(returnLength);

        snprintf(returnStr, returnLength, "If(%s): [%s, %s, %s]", tagName, ifExprStr, ifStmntStr, chainStr);
    } else if (param->tag == IF_ELSEIF_ELSE) {
        tagName = "if_elseif_else";

        char* ifExprStr = expressionRepr(param->if_elseif_t.ifExpr);
        char* ifStmntStr = statementRepr(param->if_elseif_t.ifStmnt);
        char* chainStr = statementRepr(param->if_elseif_t.chain);
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
        char* chainStr = statementRepr(param->multi_t.chain);

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

        char* num1Str = numberDataRepr(param->numeric_operator_t.num1);
        char* num2Str = numberDataRepr(param->numeric_operator_t.num2);

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
            default:
                opStr = "Error: invalid or unset operator";
        };

        returnLength = strlen("NumericOperator(): [, ]") + strlen(tagName) + strlen(num1Str) + strlen(num2Str) + strlen(opStr) + 1;
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