#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include "additional.h"

void print_error(StatusCode status) {
    switch (status) {
        case SUCCESS:
            break;
        case ERR_INVALID_INPUT:
            fprintf(stderr, "Ошибка: Неверные входные данные\n");
            break;
        case ERR_NUM_OVERFLOW:
            fprintf(stderr, "Ошибка: Переполнение числа\n");
            break;
        case ERR_INVALID_NUMBER:
            fprintf(stderr, "Ошибка: Неверный числовой ввод\n");
            break;
        case ERR_MEMORY_ALLOCATION:
            fprintf(stderr, "Ошибка: Не удалось выделить память\n");
            break;
    }
}

StatusCode parse_integer(const char *str, int *result) {
    if (str == NULL || *str == '\0') {
        return ERR_INVALID_NUMBER;
    }
    
    char *endptr;
    long value = strtol(str, &endptr, 10);
    
    if (*endptr != '\0' || endptr == str) {
        return ERR_INVALID_NUMBER;
    }
    
    if (value > INT_MAX || value < 0) {
        return ERR_NUM_OVERFLOW;
    }
    
    *result = (int)value;
    return SUCCESS;
}

StatusCode read_integer(int *result) {
    char buffer[256];
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return ERR_INVALID_NUMBER;
    }
    
    // Убираем символ новой строки
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
    
    // Проверяем что строка не пустая
    if (buffer[0] == '\0') {
        return ERR_INVALID_NUMBER;
    }
    
    return parse_integer(buffer, result);
}