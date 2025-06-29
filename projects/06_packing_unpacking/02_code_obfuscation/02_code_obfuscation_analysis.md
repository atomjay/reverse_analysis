# 02_code_obfuscation: 程式碼混淆與去混淆 - 分析報告

## 概述
本模組旨在探索程式碼混淆技術，並學習如何透過靜態分析工具（IDA Pro / Hopper）來理解混淆後的程式碼邏輯。我們透過手動模擬混淆，並觀察編譯器優化對混淆效果的影響，從而深入理解了去混淆的挑戰。

## 1. 初始設定與程式編譯

我們首先創建了一個簡單的 C 程式 `original_program.c`：

```c
#include <stdio.h>

int calculate_sum(int a, int b) {
    int sum = a + b;
    return sum;
}

int main() {
    int x = 10;
    int y = 20;
    int result = calculate_sum(x, y);
    printf("The sum is: %d\n", result);
    return 0;
}
```

並使用 `clang` 將其編譯為可執行文件 `original_program`。`original_program` 執行正常，輸出 "The sum is: 30"。

## 2. 手動模擬程式碼混淆

為了避免複雜的混淆工具設定，我們手動創建了 `obfuscated_program.c`，在 `calculate_sum` 函數中添加了簡單的混淆邏輯：

```c
#include <stdio.h>

int calculate_sum(int a, int int_b) {
    int sum = 0;
    int temp_a = a;
    int temp_b = int_b;

    // Simple obfuscation: add and subtract to achieve the sum
    if (temp_a > 0) {
        for (int i = 0; i < temp_a; ++i) {
            sum++;
        }
    }

    if (temp_b > 0) {
        for (int i = 0; i < temp_b; ++i) {
            sum++;
        }
    }

    // Another layer of simple obfuscation
    int xor_val = 0xAA;
    sum = sum ^ xor_val;
    sum = sum ^ xor_val; // XORing twice returns original value

    return sum;
}

int main() {
    int x = 10;
    int y = 20;
    int result = calculate_sum(x, y);
    printf("The sum is: %d\n", result);
    return 0;
}
```

## 3. 編譯器優化對混淆的影響

### 3.1 首次編譯 (預設優化)

我們首先使用 `clang` 預設優化編譯 `obfuscated_program.c`：

```bash
clang -o projects/06_packing_unpacking/02_code_obfuscation/obfuscated_program projects/06_packing_unpacking/02_code_obfuscation/obfuscated_program.c
```

在 IDA Pro/Hopper 中反編譯 `calculate_sum` 函數，結果如下：

```c
__int64 __fastcall calculate_sum(int a1, int a2)
{
  return (unsigned int)(a1 + a2);
}
```

這表明編譯器優化器非常強大，它將我們手動添加的循環和自抵消的 XOR 操作完全優化掉了，只保留了最原始的加法邏輯。這突顯了編譯器優化在逆向工程中帶來的挑戰。

### 3.2 重新編譯 (無優化 `-O0`)

為了觀察我們手動添加的混淆邏輯，我們使用 `-O0` 旗標重新編譯 `obfuscated_program.c`：

```bash
clang -O0 -o projects/06_packing_unpacking/02_code_obfuscation/obfuscated_program projects/06_packing_unpacking/02_code_obfuscation/obfuscated_program.c
```

重新編譯後，在 IDA Pro/Hopper 中反編譯 `calculate_sum` 函數，結果如下：

```c
__int64 __fastcall calculate_sum(int a1, int a2)
{
  int j; // [xsp+4h] [xbp-1Ch]
  int i; // [xsp+8h] [xbp-18h]
  unsigned int v5; // [xsp+14h] [xbp-Ch]

  v5 = 0;
  if ( a1 > 0 )
  {
    for ( i = 0; i < a1; ++i )
      ++v5;
  }
  if ( a2 > 0 )
  {
    for ( j = 0; j < a2; ++j )
      ++v5;
  }
  return v5;
}
```

這次，我們成功看到了手動添加的混淆邏輯：兩個 `for` 循環分別將 `a1` 和 `a2` 的值累加到 `v5` 中。這證明了 `-O0` 成功地阻止了編譯器優化，保留了原始程式碼的結構。

值得注意的是，即使在 `-O0` 的情況下，兩次 XOR 操作 (`sum = sum ^ xor_val; sum = sum ^ xor_val;`) 也沒有出現在反編譯結果中。這是因為 `XOR` 兩次相同的數會得到原始值 (`A ^ B ^ B = A`)，編譯器足夠聰明，能夠識別這種自抵消的模式並將其優化掉。

## 4. 去混淆分析

透過對反編譯結果的分析，我們成功理解了 `calculate_sum` 函數的真實邏輯：它透過兩個循環將兩個輸入參數 `a1` 和 `a2` 的值相加，最終返回它們的和。儘管程式碼看起來更複雜，但其核心功能並未改變。

## 5. 關鍵學習與結論

*   **編譯器優化的影響：** 編譯器優化是逆向工程中的一個重要因素。即使是簡單的程式碼，在優化後也可能變得難以理解。在分析時，需要考慮編譯器優化對程式碼結構的影響。
*   **去混淆的挑戰：** 程式碼混淆旨在增加逆向工程的難度。去混淆的過程需要仔細分析程式碼邏輯，識別混淆模式，並推斷原始功能。
*   **工具的重要性：** IDA Pro 或 Hopper 等專業反編譯器在理解混淆程式碼方面發揮著關鍵作用，它們能夠將組譯程式碼轉換為更易讀的偽代碼。

**模組狀態：已完成** 🎉

