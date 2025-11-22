#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>

// Коды ошибок
typedef enum {
    SUCCESS = 0,
    ERROR_INVALID_POINTER,
    ERROR_INVALID_BASE,
    ERROR_INVALID_INPUT
} ErrorCode;

// Базовые битовые операции
unsigned int addOne(unsigned int a);
unsigned int subtractOne(unsigned int a);
unsigned long long bitwiseAdd(unsigned long long a, unsigned long long b);

// Вспомогательные функции
char digit_to_char(unsigned int digit);
unsigned long long stringToNumberBitwise(const char* str, int* is_negative);

// Основная функция преобразования
ErrorCode completeTaskBigNumber(const char* inputNumber, const int r, char *result, unsigned int result_size);

#endif