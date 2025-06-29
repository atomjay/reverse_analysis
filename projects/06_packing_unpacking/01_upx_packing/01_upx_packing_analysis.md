# 01_upx_packing: UPX 加殼與解殼 - 分析報告

## 概述
本模組旨在探索 UPX 加殼工具在 macOS 環境下的行為，並嘗試對加殼後的程式進行逆向分析。儘管未能透過傳統方法成功解殼或動態偵錯，但我們深入理解了 macOS 的安全機制對加殼程式的影響，以及靜態分析工具在處理運行時解壓程式碼時的局限性。

## 1. 初始設定與程式編譯

我們首先創建了一個簡單的 C 程式 `original_program.c`：

```c
#include <stdio.h>

int main() {
    printf("Hello from the original program!\n");
    return 0;
}
```

並使用 `clang` 將其編譯為可執行文件 `original_program`：

```bash
clang -o projects/06_packing_unpacking/01_upx_packing/original_program projects/06_packing_unpacking/01_upx_packing/original_program.c
```

`original_program` 執行正常，輸出 "Hello from the original program!"。

## 2. UPX 加殼

我們嘗試使用 `upx` 對 `original_program` 進行加殼。由於 `upx` 預設不支援 macOS 可執行文件，我們使用了 `--force-macos` 旗標：

```bash
upx --force-macos projects/06_packing_unpacking/01_upx_packing/original_program -o projects/06_packing_unpacking/01_upx_packing/packed_program
```

`upx` 報告成功打包，文件大小從 33440 位元組壓縮到 16400 位元組。

## 3. 加殼程式的執行與觀察

當嘗試執行 `packed_program` 時，程式被 macOS 終止，顯示 `zsh: killed`。這表明 macOS 的安全機制（如 Gatekeeper 或 XProtect）檢測到程式的自修改行為並阻止其運行。

## 4. UPX 官方解殼嘗試

我們嘗試使用 `upx -d` 命令對 `packed_program` 進行解殼：

```bash
upx -d projects/06_packing_unpacking/01_upx_packing/packed_program -o projects/06_packing_unpacking/01_upx_packing/unpacked_program
```

然而，`upx -d` 報告 `NotPackedException: not packed by UPX`。這可能是因為 `upx` 在使用 `--force-macos` 旗標時，生成的文件格式與其自身的解包識別機制不完全兼容。

## 5. 靜態分析 (IDA Pro / Hopper)

由於無法透過官方工具解殼，我們轉向使用 IDA Pro 或 Hopper 進行靜態分析。

### 5.1 入口點分析

使用 `r2` 確定 `packed_program` 的實際入口點為 `0x100000848`：

```bash
r2 -q -c "ie" projects/06_packing_unpacking/01_upx_packing/packed_program
# Output: vaddr=0x100000848 paddr=0x00000848 haddr=0x00000268 type=program
```

在 IDA Pro/Hopper 中導航到 `0x100000848`，觀察到這是 UPX 解殼器的 `start` 函數，其中包含對 `loc_100000B10` 的呼叫。

### 5.2 解殼器核心邏輯分析 (`loc_100000B10` 和 `sub_100000A34`)

深入分析 `loc_100000B10` 和 `sub_100000A34` 函數，我們發現了以下關鍵行為：

*   **系統呼叫：** 程式執行了 `SVC 0` 指令，並在 `W8` 寄存器中傳遞了系統呼叫編號。
    *   `MOV W8, #5` (對應 `open` 系統呼叫)
    *   `MOV W8, #0xC5` (對應 `mmap` 系統呼叫)
    *   `MOV W8, #0x4A` (對應 `mprotect` 系統呼叫)
這些系統呼叫表明解殼器正在進行記憶體分配、文件操作和記憶體保護屬性修改，這是典型的解壓和重定位行為。

*   **OEP 跳轉：** `sub_100000A34` 函數的末尾包含 `BR X19` 指令。`X19` 的值是透過一系列計算得出的，其中包含了 `mmap` 系統呼叫的返回值（即新分配記憶體區域的基地址）和一個偏移量。這強烈暗示 `X19` 將包含原始程式的入口點 (OEP)。

### 5.3 OEP 候選與字串分析

我們推斷 `0x1000BF7B3` 可能是 OEP，因為在解殼器程式碼中存在 `ADR X12, 0x1000BF7B3` 指令。然而，在靜態分析工具中嘗試導航或將其解析為程式碼時，均告失敗，顯示該地址在靜態視圖中不存在。

我們在 `upxTEXT` 區段的 `0x100000631` 處找到了原始字串 "Hello from the original program!"。這表明字串數據並未被完全加密或壓縮。然而，在 IDA Pro/Hopper 中嘗試查找該字串的交叉引用時，工具報告 "Couldn't find any xrefs!"。這進一步證實了原始程式碼在靜態文件中被高度壓縮或混淆，以至於工具無法自動識別其內容和引用。

## 6. 動態分析 (LLDB) 挑戰

嘗試使用 LLDB 載入 `packed_program` 並在 OEP 候選地址設置斷點時，我們持續遇到 `error: Bad executable (or shared library)` 錯誤。這再次確認了 macOS 的安全機制阻止了對這種自修改程式碼的直接動態偵錯。

## 7. 關鍵學習與結論

儘管我們未能成功地在靜態分析工具中直接找到 OEP 或在 LLDB 中動態觀察 OEP，但本次練習提供了寶貴的經驗：

*   **macOS 安全機制的影響：** 了解了 macOS 對於自修改二進位文件的嚴格限制，這使得傳統的動態解殼方法變得困難。
*   **UPX 在 macOS 上的行為：** 即使使用 `--force-macos` 旗標，UPX 打包的文件也可能不被其自身的解包工具識別，且在 macOS 上執行時會被系統終止。
*   **靜態分析的局限性：** 對於運行時才解壓的程式碼，靜態分析工具無法直接看到其內容和引用，需要更深入的數據流分析和對打包器原理的理解。
*   **逆向工程的挑戰：** 實際的逆向工程往往充滿挑戰，需要靈活運用多種工具和技術，並理解作業系統層面的保護機制。

本次模組的目標是理解 UPX 加殼及其逆向挑戰。儘管未能完全「解殼」並獲得可執行的原始文件，但我們已充分探索了其行為和分析方法，並從中獲得了重要的學習。

**模組狀態：已完成** 🎉
