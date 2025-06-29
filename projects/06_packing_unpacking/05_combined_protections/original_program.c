#include <stdio.h>

int calculate_secret(int input) {
    int result = input * 7 + 13;
    return result;
}

int main() {
    int value = 5;
    int secret = calculate_secret(value);
    printf("The secret value is: %d\n", secret);
    return 0;
}
