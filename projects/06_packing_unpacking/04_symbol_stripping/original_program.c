#include <stdio.h>

int add(int a, int b) {
    return a + b;
}

int subtract(int a, int b) {
    return a - b;
}

int multiply(int a, int b) {
    return a * b;
}

int main() {
    int x = 10;
    int y = 5;

    int sum = add(x, y);
    printf("Sum: %d\n", sum);

    int difference = subtract(x, y);
    printf("Difference: %d\n", difference);

    int product = multiply(x, y);
    printf("Product: %d\n", product);

    return 0;
}

