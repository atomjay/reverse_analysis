// obfuscated_algo.c
#include <stdio.h>
#include <stdlib.h>

// A simple but obfuscated algorithm
int obfuscated_algorithm(int input) {
    int result = 0;
    int temp = input;

    if (temp % 2 == 0) {
        result = temp / 2;
        result = result + 10;
    } else {
        result = temp * 3;
        result = result - 5;
    }

    // Further obfuscation with bitwise operations and conditional logic
    if ((result & 0xF) == 0) { // Check if last 4 bits are zero
        result = result ^ 0xAA;
    } else {
        result = result + (result >> 2);
    }

    return result;
}

int main() {
    int user_input;
    printf("歡迎來到混淆演算法遊戲！\n");
    printf("請輸入一個數字：");
    scanf("%d", &user_input);

    int output = obfuscated_algorithm(user_input);

    printf("您的數字經過混淆演算法後變成了：%d\n", output);

    return 0;
}

