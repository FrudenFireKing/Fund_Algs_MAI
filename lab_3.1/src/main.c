#include "../include/functions.h"
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LENGTH 100
#define MAX_OUTPUT_LENGTH 100

int main() {
    char input[MAX_INPUT_LENGTH];
    char result[MAX_OUTPUT_LENGTH];
    int r;
    
    printf("=== Конвертер в системы счисления (битовые операции) ===\n\n");
    
    // Ввод числа
    printf("Введите число: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Ошибка ввода!\n");
        return 1;
    }
    
    // Убираем символ новой строки
    input[strcspn(input, "\n")] = '\0';
    
    // Проверка на пустой ввод
    if (strlen(input) == 0) {
        printf("Ошибка: пустой ввод!\n");
        return 1;
    }
    
    // Проверка что в строке только цифры
    int valid = 1;
    int has_digits = 0;
    for (int i = 0; input[i] != '\0'; i++) {
        if (i == 0 && input[i] == '-') {
            continue; // минус в начале
        }
        if (input[i] >= '0' && input[i] <= '9') {
            has_digits = 1;
        } else {
            valid = 0;
            break;
        }
    }
    
    if (!valid || !has_digits) {
        printf("Ошибка: введите корректное число (только цифры и знак минус в начале)!\n");
        return 1;
    }
    
    // Ввод основания
    printf("Введите r (1-5), где основание системы = 2^r: ");
    if (scanf("%d", &r) != 1) {
        printf("Ошибка: неверный формат числа!\n");
        return 1;
    }
    
    // Очистка буфера ввода
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    
    printf("\nПреобразование числа: %s\n", input);
    printf("Основание системы: 2^%d = %d\n\n", r, 1 << r);
    
    // Выполнение преобразования
    ErrorCode error = completeTaskBigNumber(input, r, result, MAX_OUTPUT_LENGTH);
    
    switch (error) {
        case SUCCESS:
            printf("Результат: %s\n", result);
            break;
        case ERROR_INVALID_POINTER:
            printf("Ошибка: неверный указатель!\n");
            break;
        case ERROR_INVALID_BASE:
            printf("Ошибка: r должно быть в диапазоне 1-5!\n");
            break;
        case ERROR_INVALID_INPUT:
            printf("Ошибка: неверный ввод или переполнение буфера!\n");
            break;
        default:
            printf("Неизвестная ошибка!\n");
    }
    
    // Демонстрация для всех оснований
    printf("\n--- Преобразование для всех оснований ---\n");
    for (int i = 1; i <= 5; i++) {
        if (completeTaskBigNumber(input, i, result, MAX_OUTPUT_LENGTH) == SUCCESS) {
            printf("r = %d (основание %d): %s\n", i, 1 << i, result);
        }
    }
    
    printf("\n--- Тестовые примеры с отрицательными числами ---\n");
    
    completeTaskBigNumber("-255", 4, result, MAX_OUTPUT_LENGTH);
    printf("-255 → 16-ричная: %s (ожидается: -FF)\n", result);
    
    completeTaskBigNumber("-10", 2, result, MAX_OUTPUT_LENGTH);
    printf("-10 → 4-ричная: %s (ожидается: -22)\n", result);
    
    completeTaskBigNumber("-6789", 4, result, MAX_OUTPUT_LENGTH);
    printf("-6789 → 16-ричная: %s (ожидается: -1A85)\n", result);
    

    return 0;
}