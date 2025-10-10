#pragma once

#include <stdbool.h>

typedef enum {
    SUCCESS = 0,              
    ERR_INVALID_INPUT,        
    ERR_INVALID_NUMBER,       
    ERR_NUM_OVERFLOW,         
    ERR_MEMORY_ALLOCATION     
} StatusCode;

typedef struct {
    int *primes;        
    int count;          
    int capacity;       
    int sieve_limit;   
} PrimeCache;