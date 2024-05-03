#include <stdio.h>

int to_power(int a, int b) {
    int power = 1;
    for(int i = 1; i <= b; i++) {
        power *= a;
    }
    return power;
}
