#include <stdio.h>

int calculate_sum(int a, int int_b) {
    int sum = 0;
    int temp_a = a;
    int temp_b = int_b;

    // Simple obfuscation: add and subtract to achieve the sum
    if (temp_a > 0) {
        for (int i = 0; i < temp_a; ++i) {
            sum++;
        }
    }

    if (temp_b > 0) {
        for (int i = 0; i < temp_b; ++i) {
            sum++;
        }
    }

    // Another layer of simple obfuscation
    int xor_val = 0xAA;
    sum = sum ^ xor_val;
    sum = sum ^ xor_val; // XORing twice returns original value

    return sum;
}

int main() {
    int x = 10;
    int y = 20;
    int result = calculate_sum(x, y);
    printf("The sum is: %d\n", result);
    return 0;
}

