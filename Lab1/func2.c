#include <stdio.h>

int factorial(unsigned int a) {
    if(a > 0) {
        return a * factorial(a - 1);
    } else return 1;
}
