#include <stdbool.h>
#include <limits.h>
#include <math.h>
#include "additional.h"

// -h: найти кратные числа
ReturnCode findMultiples(int number, int *result, int *count) {
    if (number <= 0 || number > 100) {
        return RC_INVALID_NUMBER;
    }
    
    *count = 0;
    for (int i = number; i <= 100; i += number) {
        result[(*count)++] = i;
    }
    
    return RC_OK;
}

// -p: проверка простого числа (упрощенна€ и более надежна€)
ReturnCode checkPrime(int number, PrimeStatus *status) {
    if (number < 0) {
        *status = PRIME_STATUS_INVALID;
        return RC_OK;
    }
    if (number == 0) {
        *status = PRIME_STATUS_ZERO;
        return RC_OK;
    }
    if (number == 1) {
        *status = PRIME_STATUS_ONE;
        return RC_OK;
    }
    if (number == 2) {
        *status = PRIME_STATUS_PRIME;
        return RC_OK;
    }
    // ѕровер€ем четные
    if (number % 2 == 0) {
        *status = PRIME_STATUS_COMPOSITE;
        return RC_OK;
    }
    
    // ѕровер€ем только нечетные делители
    for (int i = 3; i * i <= number; i += 2) {
        if (number % i == 0) {
            *status = PRIME_STATUS_COMPOSITE;
            return RC_OK;
        }
    }
    
    *status = PRIME_STATUS_PRIME;
    return RC_OK;
}

// -s: конвертаци€ в HEX (упрощенна€ и более надежна€)
ReturnCode convertToHex(const char *decimalStr, char *hexResult) {
    if (decimalStr == NULL || hexResult == NULL) {
        return RC_INVALID_NUMBER;
    }
    
    // ѕропускаем ведущие нули
    while (*decimalStr == '0') {
        decimalStr++;
    }
    
    // ≈сли все нули
    if (*decimalStr == '\0') {
        hexResult[0] = '0';
        hexResult[1] = '\0';
        return RC_OK;
    }
    
    //  онвертируем строку в число и обратно в 16-ричную
    unsigned long long number = 0;
    const char *p = decimalStr;
    
    while (*p) {
        if (*p < '0' || *p > '9') {
            return RC_INVALID_NUMBER;
        }
        
        // проверка переполнени€
        if (number > ULLONG_MAX / 10) {
            return RC_OVERFLOW;
        }
        number = number * 10 + (*p - '0');
        p++;
    }
    
    // конвертируем в 16-ричную если ноль
    if (number == 0) {
        hexResult[0] = '0';
        hexResult[1] = '\0';
        return RC_OK;
    }
    
    const char hexDigits[] = "0123456789ABCDEF";
    char tempBuffer[32];  // макс 64-бит число в 16-ричной из 16 чисел, 32 дл€ будущих изменений
    int index = 0;
    
    while (number > 0) {
        tempBuffer[index++] = hexDigits[number % 16];
        number /= 16;
    }
    

    for (int i = 0; i < index; i++) {
        hexResult[i] = tempBuffer[index - 1 - i];
    }
    hexResult[index] = '\0';
    
    return RC_OK;
}

// -e: таблица степеней
ReturnCode buildPowerTable(int maxPower, long long (*table)[11], int *count) {
    *count = 0;
    
    for (int base = 1; base <= 10; base++) {
        long long value = 1;
        for (int power = 1; power <= maxPower; power++) {
            value *= base;
            if (value > LLONG_MAX) {
                return RC_OVERFLOW;
            }
            table[base][power] = value;
            (*count)++;
        }
    }
    return RC_OK;
}



// -a: сумма арифметической прогрессии
ReturnCode calculateSum(int number, int *result) {
    if (number < 1) {
        return RC_INVALID_NUMBER;
    }
    
    // проверка на переполнение
    if (number > (int)sqrt(2.0 * INT_MAX)) {
        return RC_OVERFLOW;
    }
    
    *result = (number * (number + 1)) / 2;
    return RC_OK;
}

// -f: факториал
ReturnCode calculateFactorial(int number, long long *result) {
    if (number < 0) {
        return RC_INVALID_NUMBER;
    }
    
    *result = 1;
    for (int i = 2; i <= number; i++) {
        if (*result > LLONG_MAX / i) {
            return RC_OVERFLOW;
        }
        *result *= i;
    }
    return RC_OK;
}

