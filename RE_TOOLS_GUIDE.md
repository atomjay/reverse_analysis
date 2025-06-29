# macOS 逆向工程工具指南 (macOS RE Tools Guide)

這份文件提供在 macOS 環境下，逆向工程相關工具的安裝、管理與基本使用方法。所有說明均以專業術語和精確描述呈現。

---

## 1. 套件管理器：Homebrew

Homebrew 是 macOS 的非官方套件管理器，用於簡化軟體包的安裝與管理。它將軟體安裝在獨立的目錄中，並透過符號連結使其在系統 PATH 中可用，確保系統環境的整潔。

### 1.1 安裝 Homebrew

**指令:**
```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

**指令解釋:**
*   `curl -fsSL ...`: `curl` 用於從 Homebrew 官方 GitHub 儲存庫安全地下載安裝腳本。
*   `/bin/bash -c "..."`: 執行下載的腳本，自動處理安裝過程。
*   **注意:** 安裝過程中可能需要輸入您的系統密碼，以獲取必要的檔案系統權限。

### 1.2 驗證 Homebrew 安裝

**指令:**
```bash
brew --version
```

**指令解釋:**
*   `brew --version`: 顯示 Homebrew 的當前版本號，確認安裝成功。

### 1.3 解除安裝 Homebrew

**指令:**
```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/uninstall.sh)"
```

**指令解釋:**
*   執行此腳本將從系統中完整移除 Homebrew 及其透過 Homebrew 安裝的所有軟體包。

---

## 2. Python 環境管理：uv

`uv` 是一個高效能的 Python 套件安裝器和虛擬環境管理器，旨在替代傳統的 `pip` 和 `venv`。它利用 Rust 的效能優勢，提供快速的套件解析與安裝。

### 2.1 安裝 uv

**指令 (檢查 uv 是否已安裝):**
```bash
uv --version
```

**指令解釋:**
*   如果顯示版本號，表示已安裝。如果顯示 `command not found`，則執行以下安裝指令。

**指令 (安裝 uv):**
```bash
brew install uv
```

**指令解釋:**
*   `brew install uv`: 透過 Homebrew 安裝 `uv`。

### 2.2 建立虛擬環境

**指令:**
```bash
uv venv
```

**指令解釋:**
*   `uv venv`: 在當前目錄下創建一個名為 `.venv` 的虛擬環境，包含獨立的 Python 解釋器和套件管理系統。

### 2.3 啟用虛擬環境

**指令:**
```bash
source .venv/bin/activate
```

**指令解釋:**
*   `source .venv/bin/activate`: 修改當前 shell 環境變數，使 `python` 和 `uv pip` 命令指向虛擬環境內部。

---

## 3. 動態插樁工具：Frida

Frida 是一個動態插樁框架，允許在運行時將 JavaScript 或 Python 腳本注入目標進程，以監控、攔截和修改其行為。它提供強大的 CLI 工具集。

### 3.1 安裝 Frida

**指令:**
```bash
uv pip install frida-tools
```

**指令解釋:**
*   `uv pip install frida-tools`: 在已啟用的虛擬環境中，使用 `uv` 的 `pip` 子命令安裝 `frida-tools` 及其依賴。

### 3.2 驗證 Frida 安裝

**指令:**
```bash
frida --version
```

**指令解釋:**
*   `frida --version`: 顯示 Frida 的版本號，確認安裝成功。

### 3.3 解除安裝 Frida

**指令:**
```bash
uv pip uninstall frida-tools
```

**指令解釋:**
*   `uv pip uninstall frida-tools`: 從虛擬環境中移除 Frida 工具集。

---

## 4. 靜態分析工具：Ghidra

Ghidra 是由 NSA 開發並開源的逆向工程框架，提供強大的反組譯與反編譯功能。它支援圖形介面和無頭模式，適合複雜的二進位分析。

### 4.1 安裝 Ghidra

**指令:**
```bash
brew install --cask ghidra
```

**指令解釋:**
*   `brew install --cask ghidra`: 透過 Homebrew Cask 安裝 Ghidra 應用程式。

### 4.2 安裝 Java Development Kit (JDK)

Ghidra 是一個基於 Java 的應用程式，需要 Java 17+ JDK 才能運行。

**指令:**
```bash
brew install --cask temurin
```

**指令解釋:**
*   `brew install --cask temurin`: 安裝 OpenJDK 的一個發行版，提供所需 Java 環境。

### 4.3 驗證 Java 版本

**指令:**
```bash
java -version
```

**指令解釋:**
*   `java -version`: 顯示當前系統使用的 Java 版本，確認符合 Ghidra 要求。

### 4.4 安裝 Rosetta 2 (僅限 Apple Silicon Mac)

如果您的 Mac 是 Apple Silicon (M1/M2/M3 晶片)，Ghidra 作為 Intel 編譯的應用程式需要 Rosetta 2 轉譯層。

**指令 (檢查晶片類型):**
```bash
sysctl -n machdep.cpu.brand_string
```

**指令解釋:**
*   `sysctl -n machdep.cpu.brand_string`: 顯示 Mac 的 CPU 品牌字串，判斷是否為 Apple Silicon。

**指令 (安裝 Rosetta 2):**
```bash
softwareupdate --install-rosetta --agree-to-license
```

**指令解釋:**
*   `softwareupdate --install-rosetta --agree-to-license`: 安裝 Rosetta 2 轉譯層。

### 4.5 啟動 Ghidra

**指令:**
```bash
ghidraRun
```

**指令解釋:**
*   `ghidraRun`: 啟動 Ghidra 應用程式。如果提示 JDK 路徑，請提供 `/Library/Java/JavaVirtualMachines/temurin-XX.jdk/Contents/Home` (其中 XX 為版本號)。

### 4.6 解除安裝 Ghidra

**指令:**
```bash
brew uninstall --cask ghidra
```

**指令解釋:**
*   `brew uninstall --cask ghidra`: 移除 Ghidra 應用程式。

### 4.7 解除安裝 Java (Temurin)

**指令:**
```bash
brew uninstall --cask temurin
```

**指令解釋:**
*   `brew uninstall --cask temurin`: 移除 Temurin JDK。

---

## 5. 命令列逆向工程框架：Radare2

Radare2 (r2) 是一個功能全面的命令列逆向工程框架，提供反組譯、分析、偵錯、十六進制編輯等功能。其所有操作均透過命令列完成。

### 5.1 安裝 Radare2

**指令:**
```bash
brew install radare2
```

**指令解釋:**
*   `brew install radare2`: 透過 Homebrew 安裝 Radare2。

### 5.2 載入程式

**指令:**
```bash
r2 <path_to_binary>
```

**指令解釋:**
*   `r2 <path_to_binary>`: 使用 Radare2 載入指定的二進位檔案。

### 5.3 分析程式

**指令:**
```bash
aaa
```

**指令解釋:**
*   `aaa`: 執行 Radare2 的自動分析任務，包括函式、數據、交叉引用等。

### 5.4 列出函式

**指令:**
```bash
afl
```

**指令解釋:**
*   `afl`: 列出所有已分析的函式。

### 5.5 查看函式內容

**指令:**
```bash
s <function_address>
pd @ <function_name>
```

**指令解釋:**
*   `s <function_address>`: 跳轉到指定的函式地址。
*   `pd @ <function_name>`: 反組譯指定的函式。

### 5.6 提取記憶體內容

**指令:**
```bash
px <size> @ <address>
```

**指令解釋:**
*   `px <size> @ <address>`: 以十六進制和 ASCII 形式打印指定地址和大小的記憶體內容。

### 5.7 解除安裝 Radare2

**指令:**
```bash
brew uninstall radare2
```

**指令解釋:**
*   `brew uninstall radare2`: 移除 Radare2。

---

## 6. 命令列偵錯器：LLDB

LLDB 是 macOS 內建的強大命令列偵錯器，用於在程式運行時觀察其內部狀態、逐步執行程式碼、檢查暫存器和記憶體。

### 6.1 載入程式

**指令:**
```bash
lldb <path_to_binary>
```

**指令解釋:**
*   `lldb <path_to_binary>`: 使用 LLDB 載入指定的二進位檔案。

### 6.2 設定斷點

**指令:**
```bash
b <function_name>
```

**指令解釋:**
*   `b <function_name>`: 在指定的函式處設定斷點。

### 6.3 運行程式

**指令:**
```bash
run
```

**指令解釋:**
*   `run`: 運行程式直到遇到斷點或程式結束。

### 6.4 逐步執行

**指令:**
```bash
n
```

**指令解釋:**
*   `n`: 執行下一行程式碼 (next)。

**指令:**
```bash
s
```

**指令解釋:**
*   `s`: 執行下一條組譯指令 (step)。

### 6.5 打印變數值

**指令:**
```bash
p <variable_name>
```

**指令解釋:**
*   `p <variable_name>`: 打印指定變數的當前值。

### 6.6 繼續執行

**指令:**
```bash
c
```

**指令解釋:**
*   `c`: 繼續執行程式直到下一個斷點或程式結束。

### 6.7 退出 LLDB

**指令:**
```bash
quit
```

**指令解釋:**
*   `quit`: 退出 LLDB 偵錯器。
