#include "../include/parser.h"
#include "../include/expression.h"
#include <ctype.h>
#include <stdbool.h>

int bin_pow(int base, int exp, ParseData *parse_data) {
    if (base == 0 && exp <= 0) {
        parse_data->status = UNDEFINED_POWER_OPERATION;
        parse_data->has_error = true;
        return 0;
    }
    if (exp < 0) {
        return bin_pow(1 / base, -exp, parse_data);
    }
    
    int res = 1;
    int a = base;
    int e = exp;
    while (e > 0) {
        if (e & 1) res *= a;
        a *= a;
        e >>= 1;
    }
    return res;
}

int parse_number(ParseData *parse_data) {
    if (!isdigit(*parse_data->p)) {
        parse_data->status = UNEXPECTED_CHARACTER_IN_EXPRESSION;
        parse_data->has_error = true;
    }
    int res = 0;
    while (isdigit(*parse_data->p)) {
        res = res * 10 + (*parse_data->p - '0');
        parse_data->p++;
    }

    return res;
}

int parse_factor(Vars *vars, ParseData *parse_data) {
    if (parse_char('(', parse_data)) {
        int res = parse_expression(vars, parse_data);
        if (!parse_char(')', parse_data)) {
            parse_data->status = NO_CLOSE_BRACKET_AFTER_EXPRESSION;
            parse_data->has_error = true;
            return 0;
        }
        return res;
    }

    if (is_valid_var(*parse_data->p)) {
        int idx = *parse_data->p - 'A';
        parse_data->p++;
        if (!vars->initialized[idx]) {
            parse_data->status = UNKNOWN_VAR_IN_EXPRESSION;
            parse_data->has_error = true;
            return 0;
        }
        return vars->vars[idx];
    }

    return parse_number(parse_data);
}

int parse_power(Vars *vars, ParseData *parse_data) {
    bool is_positive = true;
    if (!parse_char('+', parse_data)) {
        if (parse_char('-', parse_data)) {
            is_positive = false;
        }
    }
    int left = parse_factor(vars, parse_data);
    if (parse_data->has_error) return 0;
    skip_spaces(parse_data);
    if (*parse_data->p == '^') {
        parse_data->p++;
        parse_data->op_used = true;
        int right = parse_power(vars, parse_data);
        if (parse_data->has_error) return 0;
        left = bin_pow(left, right, parse_data);
    }
    return is_positive ? left : -left;
}

int parse_term(Vars *vars, ParseData *parse_data) {
    int res = parse_power(vars, parse_data);
    if (parse_data->has_error) return 0;
    while (true) {
        skip_spaces(parse_data);
        if (*parse_data->p == '*' || *parse_data->p == '/') {
            bool is_mul = (*parse_data->p == '*');
            parse_data->p++;
            parse_data->op_used = true;
            int cur = parse_power(vars, parse_data);
            if (parse_data->has_error) return 0;
            if (!is_mul) {
                if (cur == 0) {
                    parse_data->status = DIVISION_BY_ZERO;
                    parse_data->has_error = true;
                    return 0;
                }
                res /= cur;
            } else res *= cur;
        } else break;
    }

    return res;
}


int parse_expression(Vars *vars, ParseData *parse_data) {
    int res = parse_term(vars, parse_data);
    if (parse_data->has_error) return 0;
    while (true) {
        skip_spaces(parse_data);
        if (*parse_data->p == '+' || *parse_data->p == '-') {
            bool is_add = (*parse_data->p == '+');
            parse_data->p++;
            parse_data->op_used = true;
            int cur = parse_term(vars, parse_data);
            res += is_add ? cur : -cur;
            if (parse_data->has_error) return 0;
        } else break;
    }
    return res;
}