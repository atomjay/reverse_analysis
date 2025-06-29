// hidden_message.c
#include <stdio.h>
#include <string.h>

// Encoded message (original message XORed with 0x55)
unsigned char encoded_message[] = {
    0x11, 0x0a, 0x1d, 0x1d, 0x1c, 0x0e, 0x1d, 0x0a, 0x11, 0x0e, 0x1d, 0x0a, 0x11, 0x0a, 0x1d, 0x1d, // "Hello, Radare2!"
    0x00 // Null terminator
};

// XOR key
unsigned char xor_key = 0x55; // 85 in decimal

void decode_message() {
    printf("Decoding message...\n");
    for (int i = 0; i < sizeof(encoded_message) - 1; i++) {
        encoded_message[i] = encoded_message[i] ^ xor_key;
    }
    printf("Decoded message: %s\n", encoded_message);
}

int main() {
    decode_message();
    return 0;
}
