#include <stdio.h>

int calculate_sum(int a, int b) {
    int sum = a + b;
    return sum;
}

int main() {
    int x = 10;
    int y = 20;
    int result = calculate_sum(x, y);
    printf("The sum is: %d\n", result);
    return 0;
}

