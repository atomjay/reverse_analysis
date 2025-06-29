# 03_custom_encryption_packing: 自定義加密打包與解密

## 目標
學習如何手動加密一個可執行文件，並編寫一個簡單的解密存根來實現自定義打包，然後逆向這個自定義打包的程式。

## 原始程式碼
`original_program.c` 將是一個簡單的 C 程式碼。

## 加密打包步驟
1. 編譯 `original_program.c` 得到 `original_program`。
2. 使用 `openssl` 或其他工具加密 `original_program`，得到 `encrypted_program.enc`。
3. 編寫 `unpack_stub.c`，該程式碼將讀取 `encrypted_program.enc`，在記憶體中解密，然後執行解密後的程式碼。
4. 將 `unpack_stub.c` 編譯為 `packed_program`，並確保 `encrypted_program.enc` 作為資源或嵌入到 `packed_program` 中。

## 逆向策略
1. **靜態分析 (IDA Pro / Hopper):**
    *   載入 `packed_program` 到 IDA Pro 或 Hopper 中。
    *   分析 `unpack_stub` 的程式碼，識別其讀取、解密和執行原始程式的邏輯。
    *   尋找加密數據在 `packed_program` 中的位置。
2. **動態偵錯 (LLDB):**
    *   使用 `lldb` 載入 `packed_program`。
    *   在解密函數處設置斷點，觀察解密過程。
    *   在解密完成後，原始程式碼被載入記憶體時，轉儲記憶體區塊。
3. **提取與解密:** 根據分析結果，手動提取加密數據，並使用正確的金鑰和演算法進行解密。

## 工具
*   `clang` (編譯器)
*   `openssl` (加密工具)
*   `r2` (Radare2，靜態分析)
*   `lldb` (動態偵錯)
*   `IDA Pro` 或 `Hopper` (專業靜態分析工具)
