#include "additional.h"
#include <stdio.h>

#define INITIAL_CAPACITY 64


int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Использование: %s <количество_запросов>\n", argv[0]);
        return ERR_INVALID_INPUT;
    }

    int query_count;
    StatusCode status = parse_integer(argv[1], &query_count);
    if (status != SUCCESS) {
        print_error(status);
        return status;
    }
    
    if (query_count <= 0) {
        fprintf(stderr, "Ошибка: Количество запросов должно быть положительным\n");
        return ERR_INVALID_NUMBER;
    }
    
    PrimeCache cache;
    status = prime_cache_init(INITIAL_CAPACITY, &cache);
    if (status != SUCCESS) {
        print_error(status);
        return status;
    }
    
    for (int i = 0; i < query_count; i++) {
        int n;
        
        status = read_integer(&n);
        if (status != SUCCESS) {
            print_error(status);
            prime_cache_free(&cache);
            return status;
        }
        
        if (n < 1) {
            fprintf(stderr, "Ошибка: Порядковый номер должен быть ? 1\n");
            prime_cache_free(&cache);
            return ERR_INVALID_NUMBER;
        }
        
        int prime;
        status = get_nth_prime(n, &prime, &cache);
        if (status != SUCCESS) {
            print_error(status);
            prime_cache_free(&cache);
            return status;
        }
        
        printf("%d\n", prime);
    }
    
    prime_cache_free(&cache);
    return SUCCESS;
}