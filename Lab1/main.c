#include <stdio.h>
#include "func1.h"
#include "func2.h"

int main() {
    printf("a = %d, b = %d\ta^b = %d\na = %d\ta! = %d\n", 2, 5, to_power(2, 5), 6, factorial(6));
}