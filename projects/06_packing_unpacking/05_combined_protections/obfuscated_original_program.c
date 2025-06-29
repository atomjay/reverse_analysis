#include <stdio.h>

int calculate_secret(int input) {
    int result = 0;
    int temp_input = input;

    // Simple obfuscation: add input value multiple times
    for (int i = 0; i < temp_input; ++i) {
        result += 7;
    }

    // Another layer of simple obfuscation: add 13 using a loop
    for (int i = 0; i < 13; ++i) {
        result++;
    }

    // XORing twice to return original value (compiler might optimize this out)
    int xor_val = 0x55;
    result = result ^ xor_val;
    result = result ^ xor_val;

    return result;
}

int main() {
    int value = 5;
    int secret = calculate_secret(value);
    printf("The secret value is: %d\n", secret);
    return 0;
}

