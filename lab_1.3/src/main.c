#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../include/findRoot.h"
#include "../include/triangleSidesCheck.h"
#include "../include/unqCheck.h"

int main(int argc, char *argv[])
{
    if (argc <= 2) {
        printf("Usage: %s <flag> <...>\n", argv[0]);
        return -1;
    }

    char *flag = argv[1];

    if(flag[0] == '-' || flag[0] == '/'){
        switch (flag[1]){
        
        case 'q':
            if(argc == 6){
                char *endptr_eps, *endptr1, *endptr2, *endptr3;
                double epsilon = strtod(argv[2], &endptr_eps);
                double a = strtod(argv[3], &endptr1);
                double b = strtod(argv[4], &endptr2);
                double c = strtod(argv[5], &endptr3);

                if ((argv[2] == endptr_eps) || (argv[3] == endptr1) || (argv[4] == endptr2) || (argv[5] == endptr3)) {
                    printf("Invalid input data\n");
                } else {
                    double * roots;

                    double combination[6][3] = {{a, b, c}, {a, c, b}, {b, a, c}, {b, c, a}, {c, b, a}, {c, a, b}};

                    for(int i = 0; i < 6; i++){
                        bool flag = true;

                        for(int j = i - 1; j >= 0; j--){
                            if(!unqCheck(combination[i], 3, combination[j], 3, epsilon)){
                                flag = false;
                                break;
                            }
                        }
                        if (flag)
                        {
                            roots = findRoot(combination[i][0], combination[i][1], combination[i][2], epsilon);
                            printf("Roots with a = %.5f, b = %.5f, c = %.5f : ", combination[i][0], combination[i][1], combination[i][2]);
                            if(roots != NULL){
                                if (fabs(roots[1]) < epsilon) roots[1] = 0.0;
                                if (roots[0] == 2 && fabs(roots[2]) < epsilon) roots[2] = 0.0;
                                for (int k = 0; k < roots[0]; k++){
                                    printf("%.5f  ", roots[k + 1]);
                                }
                                free(roots);
                            } 
                            else {
                                printf("No roots");
                            }
                            printf("\n");
                        }
                        
                    }

                }

            }else{
                printf("Invalid input data\n");
            }
            break;
        

        case 'm':
            if(argc == 4 && atoi(argv[2]) && atoi(argv[3])){
                int a = atoi(argv[2]);
                int b = atoi(argv[3]);

                if(a % b == 0){
                    printf("Divisible");
                }else{
                    printf("Not divisible");
                }
            }else{
                printf("When using flag '-m', you need to enter 2 non-zero integers");
            }
            break;


        case 't':
            if(argc == 6 && atof(argv[3]) && atof(argv[4]) && atof(argv[5])){
                double epsilon = atof(argv[2]);
                double a = atof(argv[3]);
                double b = atof(argv[4]);
                double c = atof(argv[5]);

                int isSides = triangleSidesCheck(a, b, c, epsilon);
                
                if(isSides == -1){
                    printf("All numbers must be positive");
                } else if (isSides == 0){
                    printf("Cannot be sides of a right triangle");
                } else{
                    printf("Can be sides of a right triangle");
                }

            }else{
                printf("When using flag '-t', you need to enter 4 real more than zero numbers");
            }
            break;
        
        
        default:
            printf("Unknown flag");
            break;
        }
    }else{
        printf("Invalid flag format\n");
        return 1;
    }

    return 0;
}