# 03_custom_encryption_packing: 自定義加密打包與解密 - 分析報告

## 概述
本模組旨在探索自定義加密打包技術，並學習如何透過靜態分析工具（IDA Pro / Hopper）來逆向分析解密存根，從中提取解密密碼和解密邏輯，最終恢復原始程式。我們成功地實現了加密、打包、逆向分析和解密的全過程。

## 1. 初始設定與程式編譯

我們首先創建了一個簡單的 C 程式 `original_program.c`：

```c
#include <stdio.h>

int main() {
    printf("Hello from the custom encrypted program!\n");
    return 0;
}
```

並使用 `clang` 將其編譯為可執行文件 `original_program`。`original_program` 執行正常，輸出 "Hello from the custom encrypted program!"。

## 2. 自定義加密打包

### 2.1 加密原始程式

我們使用 `openssl` 的 `aes-256-cbc` 模式加密 `original_program`，並使用硬編碼密碼 `mysecretkey123`：

```bash
openssl enc -aes-256-cbc -salt -in projects/06_packing_unpacking/03_custom_encryption_packing/original_program -out projects/06_packing_unpacking/03_custom_encryption_packing/encrypted_program.enc -k 'mysecretkey123'
```

### 2.2 創建解密存根 (`unpack_stub.c`)

我們編寫了一個 C 程式 `unpack_stub.c`，它負責讀取 `encrypted_program.enc`，使用硬編碼的密碼透過 `openssl` 命令進行解密，將解密後的內容寫入臨時文件，並最終執行該臨時文件。為了簡化，我們使用了 `popen` 調用 `openssl` 命令，而非直接使用 OpenSSL 庫的 API。

### 2.3 編譯解密存根

將 `unpack_stub.c` 編譯為可執行文件 `packed_program`：

```bash
clang -o projects/06_packing_unpacking/03_custom_encryption_packing/packed_program projects/06_packing_unpacking/03_custom_encryption_packing/unpack_stub.c
```

`packed_program` 執行正常，成功解密並運行了原始程式，輸出了 "Hello from the custom encrypted program!"。

## 3. 逆向分析解密存根 (`packed_program`)

我們使用 IDA Pro 或 Hopper 對 `packed_program` 進行靜態分析。

### 3.1 尋找解密密碼

在 `main` 函數的反編譯偽代碼中，我們發現了 `snprintf_chk` 函數的調用，它用於構建 `openssl` 解密命令字串：

```c
__snprintf_chk(
  v18,
  0x200u,
  0,
  0x200u,
  "openssl enc -aes-256-cbc -d -salt -in projects/06_packing_unpacking/03_custom_encryption_packing/encrypted_progr"
  "am.enc -k \"%s\"",
  DECRYPTION_KEY);
```

從格式字串中，我們確認了 `openssl` 命令的參數，並且 `DECRYPTION_KEY` 被傳遞給 `-k` 參數。這直接揭示了硬編碼的解密密碼為 `mysecretkey123`。

### 3.2 分析解密邏輯

`main` 函數的偽代碼清晰地展示了解密流程：

1.  **讀取加密文件：** 使用 `fopen` 和 `fread` 讀取 `encrypted_program.enc` 的內容。
2.  **執行 `openssl` 解密命令：** 透過 `popen` 執行構建好的 `openssl` 命令，將加密數據傳遞給它，並從管道中讀取解密後的數據。
3.  **寫入臨時文件：** 使用 `mkstemp` 創建臨時文件，並使用 `fwrite` 將解密後的數據寫入。
4.  **設置執行權限：** 使用 `chmod` 為臨時文件添加執行權限。
5.  **執行原始程式：** 使用 `execl` 執行解密後的臨時文件。

## 4. 手動解密與驗證

根據逆向分析得到的解密密碼和方法，我們手動執行了 `openssl` 命令來解密 `encrypted_program.enc`：

```bash
openssl enc -aes-256-cbc -d -salt -in projects/06_packing_unpacking/03_custom_encryption_packing/encrypted_program.enc -out projects/06_packing_unpacking/03_custom_encryption_packing/decrypted_original_program -k 'mysecretkey123'
```

解密後，我們為 `decrypted_original_program` 添加了執行權限：

```bash
chmod +x projects/06_packing_unpacking/03_custom_encryption_packing/decrypted_original_program
```

最終，執行 `decrypted_original_program` 成功輸出了 "Hello from the custom encrypted program!"，證明我們成功恢復了原始程式。

## 5. 關鍵學習與結論

*   **自定義加密打包的原理：** 理解了透過加密原始程式並使用解密存根來實現打包的基本原理。
*   **硬編碼密碼的脆弱性：** 即使是簡單的靜態分析，也能輕易地從程式中提取硬編碼的密碼。
*   **逆向分析流程：** 掌握了從分析解密存根、提取關鍵信息到手動解密和驗證的完整逆向分析流程。
*   **工具的應用：** 熟練運用 `openssl` 進行加密/解密，以及 IDA Pro/Hopper 進行靜態分析。

**模組狀態：已完成** 🎉
