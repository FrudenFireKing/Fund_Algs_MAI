#include "additional.h"
#include <ctype.h>
#include <limits.h>
#include <string.h>

void handleError(ReturnCode status) {
    const char *messages[] = {
        [RC_OK] = "Operation completed successfully",
        [RC_INVALID_ARGS] = "Error: Invalid number of arguments",
        [RC_INVALID_FLAG] = "Error: Invalid flag",
        [RC_INVALID_NUMBER] = "Error: Invalid number",
        [RC_OVERFLOW] = "Error: Number overflow",
        [RC_ALLOCATION_ERROR] = "Error: Memory allocation failed"
    };
    printf("%s\n", messages[status]);
}

ReturnCode parseInteger(char *str, int *number) {
    if (str == NULL || *str == '\0') {
        return RC_INVALID_NUMBER;
    }
    
    char *end;
    long result = strtol(str, &end, 10);
    
    if (*end != '\0' || end == str) {
        return RC_INVALID_NUMBER;
    }
    if (result > INT_MAX || result < INT_MIN) {
        return RC_OVERFLOW;
    }
    
    *number = (int)result;
    return RC_OK;
}

ReturnCode validateDigits(const char *str) {
    if (str == NULL || *str == '\0') {
        return RC_INVALID_NUMBER;
    }
    
    for (; *str; str++) {
        if (!isdigit(*str)) {
            return RC_INVALID_NUMBER;
        }
    }
    return RC_OK;
}

ReturnCode validateArguments(int argc, char *numberInput, char *flagInput, int *number, char *flag) {
    if (argc != 3) {
        return RC_INVALID_ARGS;
    }

    // Проверяем флаг
    if ((flagInput[0] != '-' && flagInput[0] != '/') || strlen(flagInput) != 2) {
        return RC_INVALID_FLAG;
    }
    
    *flag = flagInput[1];
    const char *validFlags = "hpsea f";
    if (strchr(validFlags, *flag) == NULL) {
        return RC_INVALID_FLAG;
    }

    // Для флага -s проверяем цифры, для остальных - парсим число
    if (*flag == 's') {
        return validateDigits(numberInput);
    } else {
        return parseInteger(numberInput, number);
    }
}