#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include "../include/functions.h"

// Проверка на то, что все переданные дробные числа - конечны в СС с основанием base
void checkingFinitenessFunction(int base, size_t count, ...)
{
    if (base == 0) {
        printf("Система счисления должна быть больше или равна 2\n");
    }
    else {
        va_list args;
        va_start(args, count);

        for (int i = 0; i < count; i++) {
            double fraction = va_arg(args, double);
            if (checkingLimitedOneFraction(fraction, base)) {
                printf("%f в СС с основанием %d конечно\n", fraction, base);
            } else {
                printf("%f в СС с основанием %d НЕ конечно\n", fraction, base);
            }
        }
        
        va_end(args);
    }
    
}

// Проверка на то, что дробное число fraction - конечно в СС с основанием base 
bool checkingLimitedOneFraction(double fraction, unsigned int base)
{
    // Обработка особых случаев
    if (fraction == 0.0) return true;
    if (base < 2) return false;
    
    // Обработка бесконечности и NaN
    if (isinf(fraction)) {
        return false; // Бесконечность не может быть конечной дробью
    }
    if (isnan(fraction)) {
        return false; // NaN не является числом
    }
    
    // Пытаемся представить дробь как rational number numerator/denominator
    // Используем ограниченную точность для избежания проблем с плавающей точкой
    double epsilon = 1e-10;
    
    // Находим приближенный числитель и знаменатель
    long long denominator = 1;
    while (fabs(fraction * denominator - round(fraction * denominator)) > epsilon && denominator < 1000000000) {
        denominator *= 10;
    }
    
    long long numerator = llround(fraction * denominator);
    
    // Сокращаем дробь
    long long div = gcd(numerator, denominator);
    numerator /= div;
    denominator /= div;
    
    // Проверяем простые делители знаменателя
    long long temp_denom = denominator;
    
    // Факторизуем знаменатель и проверяем, что все простые делители
    // также делят основание системы счисления
    for (long long i = 2; i <= temp_denom; i++) {
        if (temp_denom % i == 0) {
            // i - простой делитель знаменателя
            // Проверяем, делит ли base этот простой делитель
            if (base % i != 0) {
                return false; // Найден простой делитель, не делящий base
            }
            // Убираем все вхождения этого делителя
            while (temp_denom % i == 0) {
                temp_denom /= i;
            }
        }
    }
    
    return true;
}

// Функция для вычисления НОД
long long gcd(long long a, long long b) {
    while (b != 0) {
        long long temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}