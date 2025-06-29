# generate_license_key.py

def generate_license_key(username):
    key = 0
    for char_code in username.encode('utf-8'): # Get ASCII/UTF-8 value of each character
        key += char_code
    
    # Apply the obfuscation logic
    # Simulate 32-bit signed integer overflow for exact reproduction of C behavior
    # 0x1337 is 4919 in decimal
    # 0xDEADBEEF is 3735928559 in decimal (unsigned)
    # In 32-bit signed, 0xDEADBEEF is -560631989

    # Step 1: key * 0x1337, then simulate 32-bit overflow
    key = (key * 0x1337) & 0xFFFFFFFF # Truncate to 32 bits
    if key & 0x80000000: # Check if the 31st bit (sign bit) is set
        key = key - 0x100000000 # Convert to signed 32-bit

    # Step 2: XOR with 0xDEADBEEF, then simulate 32-bit overflow
    # 0xDEADBEEF as a signed 32-bit integer is -560631989
    xor_val = 0xDEADBEEF
    if xor_val & 0x80000000:
        xor_val = xor_val - 0x100000000

    key = key ^ xor_val

    return key

if __name__ == "__main__":
    print("歡迎來到授權金鑰生成器！")
    user_input_username = input("請輸入使用者名稱：")
    
    generated_key = generate_license_key(user_input_username);
    print(f"為使用者 '{user_input_username}' 生成的授權金鑰是：{generated_key}")

    # You can now use this generated_key in the original C program.