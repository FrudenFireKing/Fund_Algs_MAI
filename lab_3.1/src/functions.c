#include "../include/functions.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Добавление
unsigned int addOne(unsigned int a) {
    unsigned int one = 1;
    while (one) {
        unsigned int carry = a & one;
        a = a ^ one;
        one = carry << 1;
    }
    return a;
}

// Вычитание
unsigned int subtractOne(unsigned int a) {
    return ~addOne(~a);
}

// Битовое сложение двух чисел
unsigned long long bitwiseAdd(unsigned long long a, unsigned long long b) {
    unsigned long long carry = 0;
    unsigned long long one = 1;
    
    while (b != 0) {
        // Вычисляем переносы
        carry = a & b;
        
        // Сложение без учета переноса
        a = a ^ b;
        
        // Перенос для следующего разряда
        b = carry << 1;
    }
    
    return a;
}

// Преобразование цифры в символ
char digit_to_char(unsigned int digit) {
    if (digit > 31) return '?';
    const char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    return digits[digit];
}

// Преобразование строки в число
unsigned long long stringToNumberBitwise(const char* str, int* is_negative) {
    if (str == NULL || *str == '\0') {
        *is_negative = 0;
        return 0;
    }
    
    // Пропускаем минус
    *is_negative = (str[0] == '-');
    const char* number_str = *is_negative ? &str[1] : str;

    unsigned long long result = 0;
    unsigned int index = 0;
    
    while (number_str[index] != '\0') {
        char currentChar = number_str[index];
        if (!(currentChar >= '0' && currentChar <= '9')) break;
        
        unsigned int digit = currentChar & 0x0F;  // Битовая версия (currentChar - '0')
        
        // Умножение на 10: (result << 3) + (result << 1)
        unsigned long long result_times_8 = result << 3;
        unsigned long long result_times_2 = result << 1;
        unsigned long long result_times_10 = bitwiseAdd(result_times_8, result_times_2);
        
        result = bitwiseAdd(result_times_10, digit);
        index = addOne(index);
    }
    
    return result;
}

// Основная функция
ErrorCode completeTaskBigNumber(const char* inputNumber, const int r, char *result, unsigned int result_size) {
    // Проверки
    if (r < 1 || r > 5) {
        return ERROR_INVALID_BASE;
    }
    if (result == NULL) {
        return ERROR_INVALID_POINTER;
    }
    if (result_size == 0) {
        return ERROR_INVALID_INPUT;
    }

    // Проверка на отрицательно число
    int is_negative;

    // Преобразуем строку в число
    unsigned long long decimal = stringToNumberBitwise(inputNumber, &is_negative);

    // Особый случай - ноль
    if (decimal == 0) {
        if (result_size < 2) {
            return ERROR_INVALID_INPUT;
        }
        result[0] = '0';
        result[1] = '\0';
        return SUCCESS;
    }

    // Преобразование
    unsigned long long base_mask = subtractOne(1ULL << r);
    unsigned long long temp = decimal;
    char buffer[64];
    unsigned int pos = 0;

    while (temp > 0 && pos < 63) {
        unsigned long long digit = temp & base_mask;
        buffer[pos] = digit_to_char((unsigned int)digit);
        pos = addOne(pos);
        temp = temp >> r;
    }

    // Добавляем минус в буффер, если число отрицательно
    if (is_negative && pos < result_size - 1) {
        buffer[pos] = '-';
        pos = addOne(pos);
    }

    // Проверка переполнения буфера результата
    if (pos >= result_size) {
        return ERROR_INVALID_INPUT;
    }

    // Разворачиваем результат
    unsigned int left = 0;
    unsigned int right = subtractOne(pos);
    while (left < right) {
        char tmp = buffer[left];
        buffer[left] = buffer[right];
        buffer[right] = tmp;
        left = addOne(left);
        right = subtractOne(right);
    }

    // Копируем результат
    for (unsigned int i = 0; i < pos; i = addOne(i)) {
        result[i] = buffer[i];
    }
    result[pos] = '\0';

    return SUCCESS;
}