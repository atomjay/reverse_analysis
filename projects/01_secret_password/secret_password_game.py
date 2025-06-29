# secret_password_game.py

def check_password(password):
    # 這是我們藏起來的秘密密碼
    secret = "偵探小幫手"
    if password == secret:
        print("恭喜！密碼正確！你真是個厲害的偵探！")
        return True
    else:
        print("密碼錯誤！請再試一次。")
        return False

if __name__ == "__main__":
    print("歡迎來到「神秘密碼」遊戲！")
    while True:
        user_input = input("請輸入密碼：")
        if check_password(user_input):
            break
        else:
            print("提示：密碼是關於「偵探」的。")
