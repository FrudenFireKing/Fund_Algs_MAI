#include "../include/parser.h"
#include <stdio.h>
#include <stdbool.h>

void printUsage() {
    printf("Использование: ./main.out <входной файл> <выходной файл>.\n");
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printUsage();
        return 1;
    }
    char *inPath = argv[1];
    FILE *fin = fopen(inPath, "r");
    if (!fin) {
        printf("Ошибка: не удалось открыть входной файл.");
        return 1;
    }

    char *outPath = argv[2];
    FILE *fout = fopen(outPath, "w");
    if (!fout) {
        printf("Ошибка: не удалось открыть выходной файл.");
        return 1;
    }

    parse_file(fin, fout);
    fclose(fin);
    fclose(fout);
    return 0;
}