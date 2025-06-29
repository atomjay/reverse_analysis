
# reproduced_obfuscated_algo.py

def reproduced_obfuscated_algorithm(input_number):
    result = 0
    temp = input_number

    if temp % 2 == 0: # Check if temp is even
        result = temp // 2  # Use // for integer division
        result = result + 10
    else: # temp is odd
        result = temp * 3
        result = result - 5

    # Further obfuscation with bitwise operations and conditional logic
    if (result & 0xF) == 0: # Check if last 4 bits are zero
        result = result ^ 0xAA # XOR with 0xAA
    else:
        result = result + (result >> 2) # result = result + (result / 4)

    return result

if __name__ == "__main__":
    print("歡迎來到重現混淆演算法遊戲！")
    while True:
        try:
            user_input = int(input("請輸入一個數字："))
            output = reproduced_obfuscated_algorithm(user_input);
            print(f"您的數字經過重現混淆演算法後變成了：{output}")
        except ValueError:
            print("請輸入有效的數字！")
        except KeyboardInterrupt:
            print("\n遊戲結束。")
            break

