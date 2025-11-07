#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PATH_MAX 4096
#define MAX_LINE_LENGTH 80

typedef enum {
    OK,
    INVALID_INPUT,
    EQUAL_FILE_PATHS,
    INVALID_FILE_NAME,
    MEMORY_ALLOCATION,
    END_OF_INPUT,
    ERROR_WRITING_FILE
} StatusCode;

void handleError(StatusCode status) {
    switch (status) {
        case OK:
            printf("Everything completed successfully\n");
            break;
        case INVALID_INPUT:
            printf("Invalid input provided\n");
            break;
        case EQUAL_FILE_PATHS:
            printf("Input and output files cannot be the same\n");
            break;
        case INVALID_FILE_NAME:
            printf("Invalid file name or path\n");
            break;
        case MEMORY_ALLOCATION:
            printf("Memory allocation error\n");
            break;
        case END_OF_INPUT:
            printf("End of input reached\n");
            break;
        case ERROR_WRITING_FILE:
            printf("Error writing to output file\n");
            break;
        default:
            printf("Unknown error occurred\n");
    }
}

StatusCode fileValidate(char *path1, char *path2) {
    char pathReal1[PATH_MAX];
    char pathReal2[PATH_MAX];
    
    if (realpath(path1, pathReal1) == NULL) {
        return INVALID_FILE_NAME;
    }

    if (realpath(path2, pathReal2) == NULL) {
        if (path2[0] == '/') {
            strncpy(pathReal2, path2, PATH_MAX - 1);
            pathReal2[PATH_MAX - 1] = '\0';
        } else {
            if (getcwd(pathReal2, PATH_MAX) == NULL) {
                return INVALID_FILE_NAME;
            }
            size_t len = strlen(pathReal2);
            if (len + strlen(path2) + 2 > PATH_MAX) {
                return INVALID_FILE_NAME;
            }
            if (pathReal2[len - 1] != '/') {
                pathReal2[len++] = '/';
            }
            strcpy(pathReal2 + len, path2);
        }
    }

    if (strcmp(pathReal1, pathReal2) == 0) {
        return EQUAL_FILE_PATHS;
    }
    return OK;
}

StatusCode readPath(char **res) {
    int c;
    size_t cap = 64;
    size_t len = 0;
    char *buff = (char*)malloc(cap);
    if (buff == NULL) {
        return MEMORY_ALLOCATION;
    }

    while ((c = getchar()) != '\n' && c != EOF) {
        if (len + 1 >= cap) {
            cap *= 2;
            char *temp = (char*)realloc(buff, cap);
            if (temp == NULL) {
                free(buff);
                return MEMORY_ALLOCATION;
            }
            buff = temp;
        }

        buff[len++] = (char)c;
    }
    
    if (c == EOF && len == 0) {
        free(buff);
        return END_OF_INPUT;
    }
    
    buff[len] = '\0';
    *res = buff;
    return OK;
}

StatusCode readFull(FILE *input, char **res) {
    size_t cap = 4096;
    size_t len = 0;

    char *str = (char*)malloc(cap);
    if (str == NULL) {
        return MEMORY_ALLOCATION;
    }
    
    str[0] = '\0';
    
    while (true) {
        if (fgets(str + len, cap - len, input) == NULL) {
            if (len == 0) {
                free(str);
                return END_OF_INPUT;
            }
            break;
        }
        
        len = strlen(str);
        if (len > 0 && str[len - 1] == '\n') {
            break;
        }

        if (len >= cap - 2) {
            cap *= 2;
            char *temp = (char*)realloc(str, cap);
            if (temp == NULL) {
                free(str);
                return MEMORY_ALLOCATION;
            }
            str = temp;
        }
    }
    
    *res = str;
    return OK;
}

StatusCode writeLongWord(const char *word, FILE *output) {
    size_t word_len = strlen(word);
    size_t pos = 0;
    
    while (pos < word_len) {
        size_t chunk_size = word_len - pos;
        if (chunk_size > MAX_LINE_LENGTH) {
            chunk_size = MAX_LINE_LENGTH;
        }
        
        if (fwrite(word + pos, 1, chunk_size, output) != chunk_size) {
            return ERROR_WRITING_FILE;
        }
        
        fputc('\n', output);
        pos += chunk_size;
    }
    
    return OK;
}

StatusCode formatAndWriteLine(char **words, size_t word_count, size_t total_length, FILE *output) {
    if (word_count == 0) {
        return OK;
    }
    
    if (word_count == 1) {
        if (fprintf(output, "%s\n", words[0]) < 0) {
            return ERROR_WRITING_FILE;
        }
        return OK;
    }
    
    size_t total_spaces = MAX_LINE_LENGTH - total_length;
    size_t space_slots = word_count - 1;
    size_t base_spaces = total_spaces / space_slots;
    size_t extra_spaces = total_spaces % space_slots;
    
    for (size_t i = 0; i < word_count; i++) {
        if (fprintf(output, "%s", words[i]) < 0) {
            return ERROR_WRITING_FILE;
        }
        
        if (i < space_slots) {
            size_t spaces_to_add = base_spaces + (i < extra_spaces ? 1 : 0);
            for (size_t j = 0; j < spaces_to_add; j++) {
                fputc(' ', output);
            }
        }
    }
    
    fputc('\n', output);
    return OK;
}

StatusCode handleString(char *str, FILE *output) {
    char *ptr = strdup(str);
    if (ptr == NULL) {
        return MEMORY_ALLOCATION;
    }

    char **words = NULL;
    size_t word_count = 0;
    size_t current_line_length = 0;
    size_t capacity = 10;
    
    words = (char**)malloc(capacity * sizeof(char*));
    if (words == NULL) {
        free(ptr);
        return MEMORY_ALLOCATION;
    }

    char *word = strtok(ptr, " \t");
    StatusCode status = OK;
    
    while (word != NULL && status == OK) {
        size_t word_len = strlen(word);
        
        // Пропускаем пустые слова
        if (word_len == 0) {
            word = strtok(NULL, " \t");
            continue;
        }
        
        // Обработка слов длиннее 80 символов
        if (word_len > MAX_LINE_LENGTH) {
            // Сначала выводим текущую накопленную строку, если есть
            if (word_count > 0) {
                status = formatAndWriteLine(words, word_count, current_line_length, output);
                word_count = 0;
                current_line_length = 0;
            }
            
            // Затем выводим длинное слово
            if (status == OK) {
                status = writeLongWord(word, output);
            }
            
            word = strtok(NULL, " \t");
            continue;
        }
        
        // Проверяем, помещается ли слово в текущую строку
        size_t new_length = current_line_length;
        if (word_count > 0) {
            new_length += word_len + 1; // +1 для пробела
        } else {
            new_length += word_len;
        }
        
        if (new_length > MAX_LINE_LENGTH && word_count > 0) {
            // Выводим текущую строку
            status = formatAndWriteLine(words, word_count, current_line_length, output);
            word_count = 0;
            current_line_length = 0;
        }
        
        // Добавляем слово в массив
        if (word_count >= capacity) {
            capacity *= 2;
            char **temp = (char**)realloc(words, capacity * sizeof(char*));
            if (temp == NULL) {
                status = MEMORY_ALLOCATION;
                break;
            }
            words = temp;
        }
        
        words[word_count] = word;
        word_count++;
        
        if (word_count == 1) {
            current_line_length = word_len;
        } else {
            current_line_length += word_len;
        }
        
        word = strtok(NULL, " \t");
    }
    
    // Выводим оставшиеся слова
    if (status == OK && word_count > 0) {
        status = formatAndWriteLine(words, word_count, current_line_length, output);
    }
    
    free(words);
    free(ptr);
    return status;
}

StatusCode processFile(char *inputPath, char *outputPath) {
    FILE *input = fopen(inputPath, "r");
    if (input == NULL) {
        return INVALID_FILE_NAME;
    }

    FILE *output = fopen(outputPath, "w");
    if (output == NULL) {
        fclose(input);
        return INVALID_FILE_NAME;
    }
    
    char *buf = NULL;
    StatusCode status = OK;
    
    while ((status = readFull(input, &buf)) == OK) {
        status = handleString(buf, output);
        free(buf);
        buf = NULL;
        
        if (status != OK) {
            break;
        }
    }
    
    if (status == END_OF_INPUT) {
        status = OK;
    }
    
    fclose(input);
    fclose(output);
    
    return status;
}

int main() {
    bool flag = true;
    
    while (flag) {
        printf("Enter first file name: ");
        char *path1 = NULL;
        StatusCode status = readPath(&path1);
        
        if (status != OK) {
            handleError(status);
            free(path1);
            if (status == END_OF_INPUT) {
                printf("End of input reached\n");
                flag = false;
            }
            continue;
        }
        
        if (strcmp(path1, "exit") == 0 || strcmp(path1, "") == 0) {
            free(path1);
            printf("Program terminated\n");
            break;
        }
        
        printf("Enter second file name: ");
        char *path2 = NULL;
        status = readPath(&path2);
        
        if (status != OK) {
            handleError(status);
            free(path1);
            free(path2);
            if (status == END_OF_INPUT) {
                printf("End of input reached\n");
                flag = false;
            }
            continue;
        }

        status = fileValidate(path1, path2);
        if (status != OK) {
            handleError(status);
            free(path1);
            free(path2);
            continue;
        }

        status = processFile(path1, path2);
        if (status != OK) {
            handleError(status);
        } else {
            printf("File successfully processed: %s -> %s\n", path1, path2);
        }
        
        free(path1);
        free(path2);
    }
    
    printf("Program ended\n");
    return 0;
}