#include "../include/status.h"

char *getStatusMsg(Status status) {
    switch (status) {
        case NO_OPEN_BRACKET_AFTER_PRINT:
            return "Expected '(' after print";
        case NO_CLOSE_BRACKET_AFTER_VAR_IN_PRINT:
            return "Expected ')' after variable in print";
        case UNEXPECTED_CHARACTERS_AFTER_PRINT:
            return "Unexpected characters at the end of print";
        case INVALID_VAR_IN_PRINT:
            return "Expected variable A-Z in print";
        case UNKNOWN_VAR_IN_PRINT:
            return "Unknown variable in print";
        case EXPECTED_VAR_IN_ASSIGNMENT:
            return "Expected variable A-Z in assignment";
        case NO_EQUAL_SIGN_IN_ASSIGNMENT:
            return "Expected '=' in assignment";
        case UNEXPECTED_CHARACTERS_AFTER_ASSIGNMENT:
            return "Unexpected characters at the end of assignment";
        case UNDEFINED_POWER_OPERATION:
            return "Undefined power operation";
        case DIVISION_BY_ZERO:
            return "Division by zero";
        case NO_CLOSE_BRACKET_AFTER_EXPRESSION:
            return "Expected ')'";
        case UNKNOWN_VAR_IN_EXPRESSION:
            return "Unknown variable in expression";
        case UNEXPECTED_CHARACTER_IN_EXPRESSION:
            return "Unexpected character in expression";
        default:
            return "";
    }
}