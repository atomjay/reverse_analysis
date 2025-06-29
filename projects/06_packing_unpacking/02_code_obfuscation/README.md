# 02_code_obfuscation: 程式碼混淆與去混淆

## 目標
了解程式碼混淆的常見技術（如控制流平坦化、指令替換、垃圾程式碼），並學習如何透過靜態和動態分析來理解混淆後的邏輯。

## 原始程式碼
`original_program.c` 將是一個包含簡單邏輯的 C 程式碼。

## 混淆步驟
1. 編譯 `original_program.c` 得到 `original_program`。
2. 使用混淆編譯器 (例如 `ollvm`，如果設定複雜，將提供手動混淆的範例) 對 `original_program.c` 進行編譯，得到 `obfuscated_program`。

## 逆向策略
1. **靜態分析 (IDA Pro / Hopper):**
    *   載入 `obfuscated_program` 到 IDA Pro 或 Hopper 中。
    *   分析控制流圖 (CFG)，識別控制流平坦化、不透明謂詞等混淆技術。
    *   嘗試理解混淆後的程式碼邏輯，識別真實的程式碼塊和垃圾程式碼。
2. **動態偵錯 (LLDB):**
    *   使用 `lldb` 載入 `obfuscated_program`。
    *   逐步執行程式碼，觀察寄存器和記憶體的變化，追蹤數據流，以幫助理解混淆後的邏輯。
3. **模式識別:** 學習識別常見的混淆模式，並嘗試手動或使用腳本去混淆。

## 工具
*   `clang` (編譯器)
*   `ollvm` (或類似的混淆編譯器，如果無法設置，將提供手動混淆的範例)
*   `r2` (Radare2，靜態分析)
*   `lldb` (動態偵錯)
*   `IDA Pro` 或 `Hopper` (專業靜態分析工具)
