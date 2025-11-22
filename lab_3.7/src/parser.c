#include "../include/parser.h"
#include "../include/expression.h"
#include "../include/status.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

void skip_spaces(ParseData *data){
    while (*(data->p) == ' ') (data->p)++;
}

bool is_valid_var(unsigned char c) {
    return 'A' <= c && c <= 'Z';
}

bool parse_char(char c, ParseData *parse_data) {
    skip_spaces(parse_data);
    if (*parse_data->p == c) {
        parse_data->p++;
        return true;
    }
    return false;
}

int parse_var(ParseData *parse_data) {
    skip_spaces(parse_data);
    if ('A' <= *parse_data->p && *parse_data->p <= 'Z') {
        char c = *parse_data->p;
        parse_data->p++;
        return c - 'A';
    }
    return -1;
}

void trim(char *s) {
    size_t n = strlen(s);
    while (n > 0 && (isspace((unsigned char)s[n - 1]))) n--;
    s[n] = '\0';

    size_t i = 0;
    while (s[i] && isspace((unsigned char)s[i])) i++;
    if (i > 0) {
        memmove(s, s + i, strlen(s + i) + 1);
    }
}

bool print_inited_vars(FILE *fout, Vars *vars) {
    bool parsed_var = false;
    for (int i = 0; i < VAR_NUM; i++) {
        if (vars->initialized[i]) {
            if (parsed_var) {
                fprintf(fout, ", ");
            }
            fprintf(fout, "%c=%d", i + 'A', vars->vars[i]);
            parsed_var = true;
        }
    }
    return parsed_var;
}

Status try_parse_print(Vars *vars, ParseData *parse_data) {
    parse_data->p = parse_data->buffer + strlen("print");

    if (!parse_char('(', parse_data)) return NO_OPEN_BRACKET_AFTER_PRINT;

    skip_spaces(parse_data);
    char *start_pointer = parse_data->p;
    int res = parse_expression(vars, parse_data);

    if (parse_data->has_error) return parse_data->status;
    
    char *end_pointer = parse_data->p;

    if (!parse_char(')', parse_data)) return NO_CLOSE_BRACKET_AFTER_VAR_IN_PRINT;
    if (!parse_char('\0', parse_data)) return UNEXPECTED_CHARACTERS_AFTER_PRINT;

    char expression_str[MAX_STR_LEN - 6];  // "-6" для учета "Print " и нулевого символа;
    strncpy(expression_str, start_pointer, end_pointer - start_pointer);
    expression_str[end_pointer - start_pointer] = '\0';
    sprintf(parse_data->operation_msg, "Print %s", expression_str);
    printf("%d\n", res);

    return OK;
}

Status try_parse_assignment(Vars *vars, ParseData *parse_data) {
    char var_name;
    if (is_valid_var(*parse_data->p)) {
        var_name = *parse_data->p;
        parse_data->p++;
    } else return EXPECTED_VAR_IN_ASSIGNMENT;

    if (!parse_char('=', parse_data)) return NO_EQUAL_SIGN_IN_ASSIGNMENT;

    int expression_res = parse_expression(vars, parse_data);
    if (parse_data->has_error) return parse_data->status;

    if (!parse_char('\0', parse_data)) return UNEXPECTED_CHARACTERS_AFTER_ASSIGNMENT;

    int var_idx = var_name - 'A';
    vars->initialized[var_idx] = true;
    vars->vars[var_idx] = expression_res;

    sprintf(parse_data->operation_msg, "%s", parse_data->op_used ? "Arithmetic operation" : "Assignment");
    return OK;
}

void upload_parsed_data(FILE *fout, ParseData *parse_data, Vars *vars) {
    fprintf(fout, "[%d] %s | ", parse_data->cur_line, parse_data->buffer);
    if (parse_data->has_error) {
        bool parsed_vars = print_inited_vars(fout, vars);
        if (parsed_vars)
            fprintf(fout, " | ");
        fprintf(fout, "ERROR: %s\n", parse_data->operation_msg);
        fprintf(stderr, "Error on line %d: %s\n", parse_data->cur_line, parse_data->operation_msg);
    } else {
        bool parsed_vars = print_inited_vars(fout, vars);
        if (parsed_vars)
            fprintf(fout, " | ");
        fprintf(fout, "%s\n", parse_data->operation_msg);
    }
}

void parse_file(FILE *fin, FILE *fout) {
    Vars vars = {0};
    char buffer[MAX_STR_LEN];
    int lines_len = 0;
    while (fgets(buffer, sizeof(buffer), fin)) {
        trim(buffer);
        if (buffer[0] == '\0') {
            continue;
        }
        lines_len++;
        ParseData parse_data = {buffer, buffer, lines_len, false, false, "", OK};
        if (strncmp(buffer, "print", strlen("print")) == 0) {
            Status status = try_parse_print(&vars, &parse_data);
            if (status != OK) {
                parse_data.has_error = true;
                sprintf(parse_data.operation_msg, "%s", getStatusMsg(status));
            }
        } else {
            Status status = try_parse_assignment(&vars, &parse_data);
            if (status != OK) {
                parse_data.has_error = true;
                sprintf(parse_data.operation_msg, "%s", getStatusMsg(status));
            }
        }
        upload_parsed_data(fout, &parse_data, &vars);
        if (parse_data.has_error) {
            break;
        }
    }
}