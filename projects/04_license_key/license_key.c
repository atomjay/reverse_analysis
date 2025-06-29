// license_key.c
#include <stdio.h>
#include <string.h>
#include <stdlib.h> // For atoi

// Function to calculate the expected license key
// This is the function we will try to reverse engineer
int calculate_license_key(const char* username) {
    int key = 0;
    for (int i = 0; i < strlen(username); i++) {
        key += username[i];
    }
    key = (key * 0x1337) ^ 0xDEADBEEF; // Simple obfuscation
    return key;
}

int main() {
    char username[256];
    char license_key_str[256];
    int expected_key;
    int entered_key;

    printf("歡迎來到授權金鑰驗證程式！\n");

    printf("請輸入使用者名稱：");
    scanf("%s", username);

    printf("請輸入授權金鑰：");
    scanf("%s", license_key_str);

    // Convert entered license key string to integer
    entered_key = atoi(license_key_str);

    // Calculate the expected key based on the username
    expected_key = calculate_license_key(username);

    if (entered_key == expected_key) {
        printf("授權有效！歡迎使用！\n");
    } else {
        printf("授權無效！請檢查使用者名稱或金鑰。\n");
    }

    return 0;
}