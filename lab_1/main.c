#include <stdio.h>
#include "additional.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        handleError(RC_INVALID_ARGS);
        return RC_INVALID_ARGS;
    }

    char *numberInput = argv[1];
    char *flagInput = argv[2];
    int number;
    char flag;

    ReturnCode status = validateArguments(argc, numberInput, flagInput, &number, &flag);
    if (status != RC_OK) {
        handleError(status);
        return status;
    }

    switch (flag) {
        case 'h': {
            int multiples[100];
            int count = 0;
            
            status = findMultiples(number, multiples, &count);
            if (status != RC_OK) {
                handleError(status);
                return status;
            }
            
            if (count == 0) {
                printf("No multiples found in range 1-100 for number %d\n", number);
            } else {
                printf("Multiples of %d in range 1-100:\n", number);
                for (int i = 0; i < count; i++) {
                    printf("%d ", multiples[i]);
                }
                printf("\n");
            }
            break;
        }
        
        case 'p': {
            PrimeStatus primeStatus;
            status = checkPrime(number, &primeStatus);
            if (status != RC_OK) {
                handleError(status);
                return status;
            }
            
            const char *statusText[] = {
                [PRIME_STATUS_ONE] = "neither prime nor composite",
                [PRIME_STATUS_PRIME] = "prime",
                [PRIME_STATUS_COMPOSITE] = "composite", 
                [PRIME_STATUS_INVALID] = "invalid (negative)",
                [PRIME_STATUS_ZERO] = "zero"
            };
            printf("Number %d is %s\n", number, statusText[primeStatus]);
            break;
        }
        
        case 'a': {
            int sum;
            status = calculateSum(number, &sum);
            if (status != RC_OK) {
                handleError(status);
                return status;
            }
            printf("Sum of numbers from 1 to %d = %d\n", number, sum);
            break;
        }
        
        case 'f': {
            long long factorial;
            status = calculateFactorial(number, &factorial);
            if (status != RC_OK) {
                handleError(status);
                return status;
            }
            printf("%d! = %lld\n", number, factorial);
            break;
        }
        
        case 'e': {
            if (number < 1 || number > 10) {
                handleError(RC_INVALID_NUMBER);
                return RC_INVALID_NUMBER;
            }
            
            long long powerTable[11][11];
            int count = 0;
            status = buildPowerTable(number, powerTable, &count);
            if (status != RC_OK) {
                handleError(status);
                return status;
            }
            
            printf("Power table (bases 1-10, powers 1-%d):\n", number);
            for (int base = 1; base <= 10; base++) {
                printf("Основание %d:\n", base);
                for (int power = 1; power <= number; power++) {
                    printf("  %d ^ %d = %lld\n", base, power, powerTable[base][power]);
                }
                printf("\n");
            }
            break;
        }
        
        case 's': {
            char hexResult[32];
            status = convertToHex(numberInput, hexResult);
            if (status != RC_OK) {
                handleError(status);
                return status;
            }
            
            printf("Decimal %s in hexadecimal: ", numberInput);
            for (int i = 0; hexResult[i] != '\0'; i++) {
                printf("%c ", hexResult[i]);
            }
            printf("\n");
            break;
        }
        
        default:
            handleError(RC_INVALID_FLAG);
            return RC_INVALID_FLAG;
    }

    return RC_OK;
}