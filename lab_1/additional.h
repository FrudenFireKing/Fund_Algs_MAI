#pragma once

typedef enum {
    PRIME_STATUS_ONE,        
    PRIME_STATUS_PRIME,      
    PRIME_STATUS_COMPOSITE,  
    PRIME_STATUS_INVALID,    
    PRIME_STATUS_ZERO 
} PrimeStatus;

typedef enum {
    RC_OK,                   
    RC_INVALID_ARGS,         
    RC_INVALID_FLAG,       
    RC_INVALID_NUMBER,       
    RC_OVERFLOW,            
    RC_ALLOCATION_ERROR      
} ReturnCode;

ReturnCode findMultiples(int number, int *result, int *count);
ReturnCode checkPrime(int number, PrimeStatus *status);
ReturnCode calculateSum(int number, int *result);
ReturnCode calculateFactorial(int number, long long *result);
ReturnCode buildPowerTable(int maxPower, int (*table)[11], int *count);
ReturnCode convertToHex(const char *decimalStr, char *hexResult);

void handleError(ReturnCode status);
ReturnCode parseInteger(char *str, int *number);
ReturnCode validateArguments(int argc, char *numberInput, char *flagInput, int *number, char *flag);
ReturnCode validateDigits(const char *str);