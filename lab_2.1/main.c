#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "./include/functions.h"


void printTestHeader(const char* testName) {
    printf("\n");
    printf("====================================================================\n");
    printf("Тест: %s\n", testName);
    printf("====================================================================\n");
}


int main(int argc, char * argv[])
{
    printf("ТЕСТИРОВАНИЕ АЛГОРИТМА ПРОВЕРКИ КОНЕЧНОСТИ ДРОБЕЙ\n");

    // 1. Базовые тесты в различных системах счисления
    printTestHeader("Базовые тесты в различных системах счисления");
    
    // Проверка в двоичной системе счисления
    printf("Двоичная СС (2):\n");
    checkingFinitenessFunction(2, 4, 0.1, 0.2, 0.3, 0.125);
    printf("\n");

    // Проверка в троичной системе счисления
    printf("Троичная СС (3):\n");
    checkingFinitenessFunction(3, 6, 0.4, 0.7, 0.1357, 0.1284975, 0.1212, 1.0/3.0);
    printf("\n");

    // Проверка в четверичной системе счисления
    printf("Четверичная СС (4):\n");
    checkingFinitenessFunction(4, 3, 0.15, 0.25, 0.38);
    printf("\n");

    // 2. Тесты на отказоустойчивость
    printTestHeader("Тесты на отказоустойчивость");
    
    // Некорректная система счисления
    printf("Некорректная СС (0):\n");
    checkingFinitenessFunction(0, 4, 0.1, 0.2, 0.3, 0.125);
    printf("\n");
    
    printf("Некорректная СС (1):\n");
    checkingFinitenessFunction(1, 2, 0.5, 0.25);
    printf("\n");

    // 3. Тесты с граничными значениями
    printTestHeader("Тесты с граничными значениями");
    
    // Ноль и очень маленькие числа
    printf("Ноль и малые значения:\n");
    checkingFinitenessFunction(2, 5, 0.0, -0.0, 1e-10, 1e-15, 1e-20);
    printf("\n");
    
    // Отрицательные числа
    printf("Отрицательные числа:\n");
    checkingFinitenessFunction(2, 3, -0.125, -0.5, -0.75);
    printf("\n");

    // 4. Тесты с бесконечными и специальными значениями
    printTestHeader("Тесты с бесконечными и специальными значениями");
    
    // Бесконечность
    printf("Бесконечность:\n");
    double inf = INFINITY;
    checkingFinitenessFunction(2, 1, inf);
    printf("\n");
    
    // NaN (Not a Number)
    printf("NaN (Not a Number):\n");
    double nan_value = NAN;
    checkingFinitenessFunction(2, 1, nan_value);
    printf("\n");

    // 5. Тесты с очень большими и очень маленькими числами
    printTestHeader("Тесты с экстремальными значениями");
    
    // Очень маленькие числа
    printf("Очень маленькие числа:\n");
    checkingFinitenessFunction(2, 1, 1e-308); // близко к минимальному double
    checkingFinitenessFunction(10, 1, 1e-308);
    printf("\n");
    
    // Очень большие числа (дробная часть)
    printf("Большие числа с дробной частью:\n");
    checkingFinitenessFunction(2, 2, 1.0 + 1e-10, 1000.0009765625); // 1000 + 1/1024
    printf("\n");

    // 6. Тесты с периодическими дробями
    printTestHeader("Тесты с периодическими дробями");
    
    // Дроби с периодом в различных СС
    printf("Периодические дроби:\n");
    checkingFinitenessFunction(3, 1, 1.0/3.0); // 0.333... - конечно в троичной
    checkingFinitenessFunction(2, 1, 1.0/3.0); // 0.333... - бесконечно в двоичной
    checkingFinitenessFunction(6, 1, 1.0/3.0); // 0.333... - конечно в шестеричной
    printf("\n");

    // 7. Тесты с простыми числами в знаменателе
    printTestHeader("Тесты с простыми числами в знаменателе");
    
    // Дроби с простыми числами в знаменателе
    checkingFinitenessFunction(2, 3, 1.0/2.0, 1.0/3.0, 1.0/5.0);
    checkingFinitenessFunction(3, 3, 1.0/2.0, 1.0/3.0, 1.0/5.0);
    checkingFinitenessFunction(5, 3, 1.0/2.0, 1.0/3.0, 1.0/5.0);
    checkingFinitenessFunction(10, 3, 1.0/2.0, 1.0/3.0, 1.0/5.0);
    printf("\n");

    // 8. Тесты с составными основаниями
    printTestHeader("Тесты с составными основаниями");
    
    // Составные системы счисления
    checkingFinitenessFunction(6, 2, 1.0/2.0, 1.0/3.0);   // 6 = 2*3
    checkingFinitenessFunction(12, 3, 1.0/3.0, 1.0/4.0, 1.0/6.0); // 12 = 2*2*3
    checkingFinitenessFunction(15, 2, 1.0/3.0, 1.0/5.0);  // 15 = 3*5
    printf("\n");

    // 9. Тесты на точность представления
    printTestHeader("Тесты на точность представления");
    
    // Числа, которые могут иметь проблемы с точностью
    checkingFinitenessFunction(2, 3, 0.1, 0.2, 0.3); // известные проблемы в двоичной СС
    checkingFinitenessFunction(10, 3, 0.1, 0.2, 0.3); // конечно в десятичной
    printf("\n");

    // 10. Тесты с большим количеством аргументов
    printTestHeader("Тесты с большим количеством аргументов");
    
    // Множественные аргументы
    printf("Множественные аргументы (10 чисел):\n");
    checkingFinitenessFunction(2, 10, 
        0.5, 0.25, 0.125, 0.0625, 0.03125, 
        0.015625, 0.0078125, 0.00390625, 0.001953125, 0.0009765625);
    printf("\n");

    printf("\nВСЕ ТЕСТЫ ЗАВЕРШЕНЫ!\n");
    
    return 0;
}