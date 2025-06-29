# 01_upx_packing: UPX 加殼與解殼

## 目標
學習 UPX 的基本使用，理解其壓縮和自解壓原理，並掌握如何識別 UPX 加殼並進行解殼。

## 原始程式碼
`original_program.c` 將是一個簡單的 C 程式碼，用於演示加殼效果。

## 加殼步驟
1. 編譯 `original_program.c` 得到 `original_program`。
2. 使用 `upx original_program -o packed_program` 進行加殼。

## 逆向策略
1. **識別加殼:** 使用 `strings`、`file` 或 `Detect It Easy (DIE)` 等工具識別 `packed_program` 是否被 UPX 加殼。
2. **官方解殼:** 嘗試使用 `upx -d packed_program` 進行官方解殼。
3. **手動解殼 (偵錯器):**
    *   使用 `lldb` 載入 `packed_program`。
    *   在 UPX 解殼器完成工作後，尋找原始入口點 (OEP)。通常可以透過觀察記憶體權限變化 (例如 `mprotect` 系統呼叫) 或在 `popad` 指令後設置斷點來尋找。
    *   在 OEP 處轉儲記憶體，並嘗試修復導入表 (如果需要)。
4. **靜態分析 (IDA Pro / Hopper):** 載入 `packed_program` 到 IDA Pro 或 Hopper 中，觀察加殼後的程式結構，並嘗試識別解殼器邏輯。

## 工具
*   `clang` (編譯器)
*   `upx` (加殼工具)
*   `strings` (字串提取)
*   `r2` (Radare2，靜態分析)
*   `lldb` (動態偵錯)
*   `IDA Pro` 或 `Hopper` (專業靜態分析工具)
