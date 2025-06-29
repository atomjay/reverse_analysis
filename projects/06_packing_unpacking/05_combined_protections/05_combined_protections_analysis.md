# 05_combined_protections: 綜合保護與逆向 - 分析報告

## 概述
本模組是整個「破解打包與解包」專案的集大成者，旨在將之前學習的各種二進位保護技術（程式碼混淆、符號移除、自定義加密打包、UPX 加殼）應用到一個程式上，並學習如何制定一個綜合性的逆向策略來逐步解除多層保護，最終恢復原始程式的邏輯。

## 1. 初始設定與程式編譯

我們首先創建了一個簡單的 C 程式 `original_program.c`：

```c
#include <stdio.h>

int calculate_secret(int input) {
    int result = input * 7 + 13;
    return result;
}

int main() {
    int value = 5;
    int secret = calculate_secret(value);
    printf("The secret value is: %d\n", secret);
    return 0;
}
```

並使用 `clang` 將其編譯為可執行文件 `original_program`。`original_program` 執行正常，輸出 "The secret value is: 48"。

## 2. 應用綜合保護

我們按照以下順序，從內到外地應用了多層保護：

### 2.1 程式碼混淆

創建 `obfuscated_original_program.c`，手動模擬混淆 `calculate_secret` 函數的邏輯，使其透過循環實現乘法和加法。使用 `clang -O0` 編譯為 `obfuscated_program`，以保留混淆邏輯。

### 2.2 符號移除

對 `obfuscated_program` 應用 `strip` 命令，生成 `stripped_program`，移除了所有符號信息。

### 2.3 自定義加密打包

1.  使用 `openssl` 和密碼 `combinedsecretkey` 加密 `stripped_program`，生成 `encrypted_stripped_program.enc`。
2.  創建 `combined_unpack_stub.c`，該存根負責解密 `encrypted_stripped_program.enc`，將其寫入臨時文件，並執行。編譯為 `combined_packed_program`。

### 2.4 UPX 加殼

最後，使用 `upx --force-macos` 對 `combined_packed_program` 進行加殼，生成最終的 `combined_protected_program`。

執行 `combined_protected_program` 時，程式被 macOS 終止 (`zsh: killed`)，這表明最外層的 UPX 保護已生效。

## 3. 逆向分析綜合保護

我們採用了「層層剝離」的逆向策略，從最外層的保護開始，逐步深入。

### 3.1 剝離 UPX 加殼

1.  **識別 UPX：** 在 IDA Pro/Hopper 中打開 `combined_protected_program`，並在字串視圖中找到 UPX 簽名，確認其被 UPX 加殼。
2.  **找到入口點：** 使用 `r2 -q -c "ie"` 獲取程式入口點 `0x100000d08`。
3.  **分析解殼器：** 在 IDA Pro/Hopper 中分析 `start` 函數和其調用的 `loc_100000FD0`、`sub_100000EF4`。我們觀察到典型的 UPX 解殼器模式，包括 `mmap` (syscall 197) 和 `mprotect` (syscall 74) 系統呼叫，以及最終的 `BR X19` 跳轉。
4.  **確定 OEP：** 透過分析 `sub_100000EF4` 函數的邏輯，我們推斷 `BR X19` 將跳轉到我們的自定義加密解密存根的入口點。我們在 IDA Pro/Hopper 中打開未經 UPX 加殼的 `combined_packed_program`，確認其 `_main` 函數的入口點為 `0x1000004F8`。這就是 UPX 解殼器將控制權轉移到的 OEP。

### 3.2 剝離自定義加密打包

1.  **分析解密存根：** 在 IDA Pro/Hopper 中打開 `combined_packed_program` (即 UPX 解殼後的程式)。
2.  **提取解密密碼：** 在 `_main` 函數中，我們找到了 `snprintf_chk` 函數調用，其格式字串揭示了 `openssl` 命令的 `-k` 參數，從而提取到硬編碼的解密密碼：`combinedsecretkey`。
3.  **理解解密邏輯：** `_main` 函數的偽代碼清晰地展示了文件讀取、`openssl` 解密命令執行（透過 `popen`）、數據寫入臨時文件、設置執行權限和 `execl` 執行臨時文件的完整流程。
4.  **手動解密：** 使用提取到的密碼，我們手動執行 `openssl` 命令解密 `encrypted_stripped_program.enc`，得到 `decrypted_stripped_program`。為其添加執行權限後，成功運行並輸出了預期結果。

### 3.3 剝離符號移除與去混淆

1.  **分析 `decrypted_stripped_program`：** 在 IDA Pro/Hopper 中打開 `decrypted_stripped_program`。
2.  **識別 `main` 函數：** 由於符號被移除，我們透過字串 "The secret value is: %d\n" 的交叉引用，找到了程式的入口函數 `start` (即原始的 `main` 函數)。
3.  **識別混淆函數：** 在 `start` 函數中，我們觀察到對 `sub_100000460` 的調用，其參數和上下文表明它就是原始的 `calculate_secret` 函數。
4.  **去混淆邏輯：** 分析 `sub_100000460` 的反編譯偽代碼：
    ```c
    __int64 __fastcall sub_100000460(int a1)
    {
      int j; // [xsp+Ch] [xbp-14h]
      int i; // [xsp+10h] [xbp-10h]
      unsigned int v4; // [xsp+18h] [xbp-8h]

      v4 = 0;
      for ( i = 0; i < a1; ++i )
        v4 += 7;
      for ( j = 0; j < 13; ++j )
        ++v4;
      return v4;
    }
    ```
    這段程式碼清晰地揭示了原始邏輯：`return (a1 * 7) + 13;`，與 `original_program.c` 中的 `calculate_secret` 函數完全一致。

## 4. 關鍵學習與結論

*   **層次化逆向策略：** 成功應對多層保護的關鍵在於採用層次化的逆向策略，從最外層開始，逐步剝離保護。
*   **綜合工具應用：** 熟練運用 `r2` 獲取入口點，IDA Pro/Hopper 進行靜態分析和反編譯，以及 `openssl` 和 `chmod` 等命令行工具進行數據處理。
*   **理解保護機制：** 深入理解每種保護技術的原理和其對逆向工程的影響，是成功逆向的基礎。
*   **挑戰與解決方案：** 即使面對 macOS 的安全限制（如 `zsh: killed`），透過靜態分析和對保護原理的理解，仍然可以有效地進行逆向。
*   **耐心與細緻：** 逆向工程是一個需要耐心和細緻觀察的過程，尤其是在處理多層保護時。

**模組狀態：已完成** 🎉

**專案六：破解「打包與解包」狀態：已完成** 🎉

