#ifndef PARSER_H
#define PARSER_H

#include "status.h"
#include <stdio.h>
#include <stdbool.h>

#define MAX_STR_LEN 500
#define VAR_NUM 26

typedef struct {
    char *buffer; //исходная строка
    char *p;      // текущая позиция в строке
    int cur_line;
    bool has_error;
    bool op_used;
    char operation_msg[MAX_STR_LEN];
    Status status;
} ParseData;

typedef struct {
    int vars[VAR_NUM];
    bool initialized[VAR_NUM];
} Vars;

bool print_inited_vars(FILE *fout, Vars *vars);

void parse_file(FILE *fin, FILE *fout);

bool is_valid_var(unsigned char c);

bool parse_char(char c, ParseData *parse_data);

int parse_var(ParseData *parse_data);

void skip_spaces(ParseData *data);

#endif // PARSER_H