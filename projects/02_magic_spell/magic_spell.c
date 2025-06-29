// magic_spell.c
#include <stdio.h>
#include <stdlib.h>

int magic_spell(int input_number) {
    int result = input_number * 3; // Multiply by 3
    result = result + 7;          // Add 7
    result = result ^ 0xAA;       // XOR with 0xAA (170 in decimal)
    return result;
}

int main() {
    int user_input;
    printf("歡迎來到魔法咒語遊戲！\n");
    printf("請輸入一個數字：");
    scanf("%d", &user_input);

    int transformed_number = magic_spell(user_input);

    printf("您的數字經過魔法咒語後變成了：%d\n", transformed_number);

    return 0;
}

