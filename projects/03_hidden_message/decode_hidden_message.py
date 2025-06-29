# decode_hidden_message.py

encoded_message_hex = "11 0a 1d 1d 1c 0e 1d 0a 11 0e 1d 0a 11 0a 1d 1d 00"
xor_key = 0x55

def hex_to_bytes(hex_str):
    bytes_list = []
    for byte_hex in hex_str.split():
        bytes_list.append(int(byte_hex, 16))
    return bytes(bytes_list)

encoded_bytes = hex_to_bytes(encoded_message_hex)

# Only decode the first 16 bytes, as per the C code's loop (sizeof(encoded_message) - 1)
# The last byte (0x00) is the null terminator and should not be XORed.
decoded_bytes_list = []
for i in range(len(encoded_bytes) - 1): # Loop up to the second to last byte
    decoded_bytes_list.append(encoded_bytes[i] ^ xor_key)

# Add the null terminator back without XORing it
decoded_bytes_list.append(encoded_bytes[len(encoded_bytes) - 1])

decoded_bytes = bytes(decoded_bytes_list)

try:
    decoded_message = decoded_bytes.decode('utf-8')
    print(f"Decoded message: {decoded_message}")
except UnicodeDecodeError:
    print("Error decoding to UTF-8. Raw bytes:", decoded_bytes)