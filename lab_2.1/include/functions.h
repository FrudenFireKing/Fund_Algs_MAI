#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdbool.h>


// Проверка на то, что все переданные дробные числа - конечны в СС с основанием base
void checkingFinitenessFunction(int base, size_t count, ...);

// Проверка на то, что дробное число fraction - конечно в СС с основанием base 
bool checkingLimitedOneFraction(double fraction, unsigned int base);

// Функция для вычисления НОД
long long gcd(long long a, long long b);

#endif