# 04_symbol_stripping: 符號移除與逆向

## 目標
理解符號表的作用以及 `strip` 命令如何移除符號，並學習在缺乏符號信息的情況下進行逆向分析的技巧。

## 原始程式碼
`original_program.c` 將是一個包含多個函數的 C 程式碼。

## 符號移除步驟
1. 編譯 `original_program.c` 得到 `original_program` (包含符號表)。
2. 使用 `strip original_program -o stripped_program` 移除符號。

## 逆向策略
1. **靜態分析 (IDA Pro / Hopper):**
    *   載入 `stripped_program` 到 IDA Pro 或 Hopper 中。
    *   觀察函數名稱變為 `sub_XXXX` 或 `loc_XXXX` 等通用名稱。
    *   利用交叉引用 (Xrefs)、函數簽名 (Function Signatures)、參數傳遞約定 (Calling Conventions) 和數據流分析來推斷函數的功能。
    *   手動重命名函數和變數以增加可讀性。
2. **動態偵錯 (LLDB):**
    *   使用 `lldb` 載入 `stripped_program`。
    *   透過觀察函數調用、參數和返回值來推斷函數功能。
    *   利用堆疊回溯來理解程式執行流程。
3. **字串分析:** 即使符號被移除，程式中的字串通常仍然存在，可以提供關於程式功能的線索。

## 工具
*   `clang` (編譯器)
*   `strip` (符號移除工具)
*   `r2` (Radare2，靜態分析)
*   `lldb` (動態偵錯)
*   `IDA Pro` 或 `Hopper` (專業靜態分析工具)
