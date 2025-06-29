

# reproduced_magic_spell.py

def reproduced_magic_spell(input_number):
    result = input_number * 3
    result = result + 7
    result = result ^ 0xAA  # 0xAA is 170 in decimal
    return result

if __name__ == "__main__":
    print("歡迎來到重現魔法咒語遊戲！")
    while True:
        try:
            user_input = int(input("請輸入一個數字："))
            transformed_number = reproduced_magic_spell(user_input);
            print(f"您的數字經過重現魔法咒語後變成了：{transformed_number}")
        except ValueError:
            print("請輸入有效的數字！")
        except KeyboardInterrupt:
            print("\n遊戲結束。")
            break

