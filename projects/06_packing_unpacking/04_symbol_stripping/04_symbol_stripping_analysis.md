# 04_symbol_stripping: 符號移除與逆向 - 分析報告

## 概述
本模組旨在探索符號移除技術對逆向工程的影響，並學習如何在缺乏符號信息的情況下，透過靜態分析工具（IDA Pro / Hopper）來識別函數功能。我們成功地透過字串交叉引用和程式碼邏輯分析，恢復了原始函數的語義。

## 1. 初始設定與程式編譯

我們首先創建了一個包含多個函數的 C 程式 `original_program.c`：

```c
#include <stdio.h>

int add(int a, int b) {
    return a + b;
}

int subtract(int a, int b) {
    return a - b;
}

int multiply(int a, int b) {
    return a * b;
}

int main() {
    int x = 10;
    int y = 5;

    int sum = add(x, y);
    printf("Sum: %d\n", sum);

    int difference = subtract(x, y);
    printf("Difference: %d\n", difference);

    int product = multiply(x, y);
    printf("Product: %d\n", product);

    return 0;
}
```

並使用 `clang` 將其編譯為可執行文件 `original_program`。`original_program` 執行正常，輸出了預期的計算結果。

## 2. 符號移除

我們使用 `strip` 命令對 `original_program` 進行符號移除，生成 `stripped_program`：

```bash
strip -o projects/06_packing_unpacking/04_symbol_stripping/stripped_program projects/06_packing_unpacking/04_symbol_stripping/original_program
```

`stripped_program` 執行正常，與原始程式輸出相同，這證實了符號移除不會影響程式的執行功能。

## 3. 逆向分析符號移除後的程式

我們使用 IDA Pro 或 Hopper 對 `stripped_program` 進行靜態分析。

### 3.1 觀察符號移除的效果

在 IDA Pro/Hopper 的函數列表中，我們發現原始的函數名稱（如 `main`, `add`, `subtract`, `multiply`）都被替換為通用名稱，例如 `sub_XXXX` 或 `loc_XXXX`。這使得程式碼的閱讀和理解變得更加困難。

### 3.2 識別 `main` 函數

由於 `main` 函數的符號也被移除，我們無法直接找到它。我們利用程式中仍然存在的字串作為線索。我們在字串視圖中找到了以下字串：

*   "Sum: %d\n"
*   "Difference: %d\n"
*   "Product: %d\n"

透過查看這些字串的交叉引用，我們發現它們都被 `start` 函數引用。這表明 `start` 函數就是我們原始程式中的 `main` 函數（或其等效的入口函數）。

### 3.3 識別 `add`, `subtract`, `multiply` 函數

分析 `start` 函數的反編譯偽代碼，我們觀察到它調用了三個沒有明確名稱的函數，並將它們的返回值與上述字串一起打印：

```c
__int64 start()
{
  int v1; // [xsp+18h] [xbp-18h]
  int v2; // [xsp+1Ch] [xbp-14h]
  int v3; // [xsp+20h] [xbp-10h]

  v3 = sub_100000460(10, 5);
  printf("Sum: %d\n", v3);
  v2 = sub_100000480(10, 5);
  printf("Difference: %d\n", v2);
  v1 = sub_1000004A0(10, 5);
  printf("Product: %d\n", v1);
  return 0;
}
```

根據調用上下文和打印的字串，我們成功推斷出這些函數的功能：

*   `sub_100000460` 執行加法操作，對應 `add` 函數。
*   `sub_100000480` 執行減法操作，對應 `subtract` 函數。
*   `sub_1000004A0` 執行乘法操作，對應 `multiply` 函數。

我們進一步驗證了 `add` 函數的反編譯偽代碼，確認其確實執行了加法：

```c
__int64 __fastcall add(int a1, int a2)
{
  return (unsigned int)(a1 + a2);
}
```

## 4. 關鍵學習與結論

*   **符號移除的影響：** 符號移除會顯著增加逆向工程的難度，因為它移除了函數和變數的語義信息。
*   **字串交叉引用的重要性：** 在缺乏符號信息的情況下，字串交叉引用是定位關鍵程式碼區域（如 `main` 函數）的有效方法。
*   **程式碼邏輯推斷：** 即使沒有符號，也可以透過分析函數的輸入、輸出和內部邏輯來推斷其功能。
*   **工具的應用：** IDA Pro 或 Hopper 等專業反編譯器在分析符號移除後的程式碼方面發揮著關鍵作用，它們的反編譯功能有助於理解底層邏輯。

**模組狀態：已完成** 🎉
