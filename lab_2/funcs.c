#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include "additional.h"

// –ешето Ёратосфена - находит все простые до limit
StatusCode sieve_of_eratosthenes(int limit, PrimeCache *cache) {
    if (limit < 2) {
        return SUCCESS;
    }
    
    bool *is_prime = malloc((limit + 1) * sizeof(bool));
    if (is_prime == NULL) {
        return ERR_MEMORY_ALLOCATION;
    }
    
    memset(is_prime, true, (limit + 1) * sizeof(bool));
    is_prime[0] = is_prime[1] = false;
    
    for (int p = 2; p * p <= limit; p++) {
        if (is_prime[p]) {
            for (int i = p * p; i <= limit; i += p) {
                is_prime[i] = false;
            }
        }
    }
    
    for (int i = 2; i <= limit; i++) {
        if (is_prime[i]) {
            if (cache->count >= cache->capacity) {
                StatusCode status = ensure_capacity(cache->count + 1, cache);
                if (status != SUCCESS) {
                    free(is_prime);
                    return status;
                }
            }
            cache->primes[cache->count++] = i;
        }
    }
    
    free(is_prime);
    cache->sieve_limit = limit;
    return SUCCESS;
}

StatusCode ensure_capacity(int required_capacity, PrimeCache *cache) {
    if (required_capacity <= cache->capacity) {
        return SUCCESS;
    }
    
    int new_capacity = cache->capacity;
    while (new_capacity < required_capacity) {
        if (new_capacity > INT_MAX / 2) {
            return ERR_MEMORY_ALLOCATION;
        }
        new_capacity = (new_capacity == 0) ? 64 : new_capacity * 2;
    }
    
    int *new_primes = realloc(cache->primes, sizeof(int) * new_capacity);
    if (new_primes == NULL) {
        return ERR_MEMORY_ALLOCATION;
    }
    
    cache->primes = new_primes;
    cache->capacity = new_capacity;
    return SUCCESS;
}

StatusCode get_nth_prime(int n, int *result, PrimeCache *cache) {
    if (n <= 0) {
        return ERR_INVALID_NUMBER;
    }
    
    if (n <= cache->count) {
        *result = cache->primes[n - 1];
        return SUCCESS;
    }
    

    int estimated_limit;
    if (n < 6) {
        estimated_limit = 20;  
    } else {
        estimated_limit = (int)(n * (log(n) + log(log(n))) * 1.2) + 100;
        if (estimated_limit < 0 || estimated_limit > INT_MAX - 1000) {
            return ERR_NUM_OVERFLOW;
        }
    }
    
    if (estimated_limit <= cache->sieve_limit) {
        estimated_limit = cache->sieve_limit * 2;
    }
    
    StatusCode status = sieve_of_eratosthenes(estimated_limit, cache);
    if (status != SUCCESS) {
        return status;
    }
    
    if (n > cache->count) {
        return get_nth_prime(n, result, cache);
    }
    
    *result = cache->primes[n - 1];
    return SUCCESS;
}

StatusCode prime_cache_init(int initial_capacity, PrimeCache *cache) {
    cache->primes = malloc(sizeof(int) * initial_capacity);
    if (cache->primes == NULL) {
        return ERR_MEMORY_ALLOCATION;
    }
    
    cache->capacity = initial_capacity;
    cache->count = 0;
    cache->sieve_limit = 1;
    
    return SUCCESS;
}

void prime_cache_free(PrimeCache *cache) {
    free(cache->primes);
    cache->primes = NULL;
    cache->count = 0;
    cache->capacity = 0;
    cache->sieve_limit = 0;
}