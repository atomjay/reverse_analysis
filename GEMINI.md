# 逆向工程學習專案總覽

這份文件是您逆向工程學習之旅的總目錄，它將引導您完成一系列循序漸進的專案，並提供相關的學習指南。

---

## 指導原則 (Guiding Principles)

*   **雙層解釋 (Dual-Layer Explanation):**
    *   **【FANNG 級別專業解釋】:** 提供技術細節、業界術語和原理，確保知識的深度和廣度。
*   **安全與可控 (Safety & Control):**
    *   所有工具的安裝與移除都應透過套件管理器 (Homebrew) 或專用環境 (如 `uv` 虛擬環境) 進行，確保系統的整潔與穩定。
    *   任何可能修改系統或檔案的操作，都將提前解釋其目的和潛在影響。
*   **環境隔離 (Environment Isolation):**
    *   優先使用 `uv` 建立和管理虛擬環境，確保逆向工程工具與系統 Python 環境完全隔離，避免衝突。
    *   所有 Python 相關的執行和套件安裝，都應在已啟用的虛擬環境中進行。
*   **效率與現代化 (Efficiency & Modernization):**
    *   優先採用最新、最有效率的工具和方法 (例如 `uv` 而非 `pip`)。
    *   持續優化工作流程，確保每一步操作都高效且符合業界最佳實踐。
*   **清晰溝通與記錄 (Clear Communication & Documentation):**
    *   每一步操作、每個指令都將提供詳細的解釋。
    *   專案進度、重要發現和學習筆記將被記錄在專門的 Markdown 文件中，便於追蹤和回顧。
    *   文件管理將採用主題/階段拆分和統一命名規則，確保可讀性和可可維護性。
*   **互動式命令處理 (Interactive Command Handling):**
    *   對於會建立持續會話或需要使用者互動的命令 (例如 `frida -p <PID>`、偵錯器等)，將明確指示使用者在自己的終端機 (TTY) 中手動執行，而非透過 `run_shell_command` 工具。`run_shell_command` 僅適用於執行後會自動終止的命令。
    *   **重要提示：** 當您處於某個工具的互動式會話中（例如看到 `[Local::PID::XXXXX ]->` 這樣的提示符號），您必須先輸入該工具的退出命令（通常是 `exit` 或 `quit`）來結束當前會話，才能在同一個終端機視窗中執行其他命令，或在新的終端機視窗中啟動新的工具。

---

## 學習指南

*   [macOS 逆向工程學習指南總目錄](guides/re_guide/README.md)
*   [macOS 逆向工程工具指南](guides/re_guide/RE_TOOLS_GUIDE.md)

---

## 專案進度 (Project Progress)

### **專案一：破解「神秘密碼」**

*   [專案概述](projects/01_secret_password/01_project_overview.md)

**專案狀態:** **已完成** 🎉

---

### **專案二：破解「魔法咒語」**

*   [專案概述](projects/02_magic_spell/README.md) (待創建)

**專案狀態:** **已完成** 🎉

---

### **專案三：破解「隱藏的訊息」**

*   [專案概述](projects/03_hidden_message/README.md) (待創建)

**專案狀態:** **已完成** 🎉

---

### **專案四：破解「授權金鑰」**

*   [專案概述](projects/04_license_key/README.md) (待創建)

**專案狀態:** **已完成** 🎉

---

### **專案五：破解「混淆的演算法」**

**目標:** 逆向分析一個程式，該程式包含一個經過混淆處理的演算法，揭示其真實邏輯。

**學習路徑:**
1.  **建立混淆程式:** 創建一個包含簡單但經過混淆處理的演算法的程式。
2.  **靜態分析混淆程式:** 使用 `strings` 和 `r2` 分析混淆後的程式，識別潛在的數據和函式。
3.  **動態偵錯混淆演算法:** 使用 LLDB 在程式運行時逐步執行混淆後的演算法，觀察變數和暫存器的變化，從而推斷其真實邏輯。
4.  **理解混淆技術:** 了解一些基本的混淆技術，以及如何透過逆向工程來繞過它們。
5.  **重現演算法:** 根據逆向分析的結果，編寫一個能夠精確重現原始演算法的程式。

### **專案狀態:** **已完成** 🎉

#### **解決方案:**

*   **靜態分析 (Radare2):** 透過 Radare2 分析 `obfuscated_algo` 的組譯碼，識別出混淆演算法的邏輯。
*   **動態驗證:** 執行原始程式並輸入測試數據，確認其輸出與 Python 重現的演算法一致。
*   **Python 重現:** 根據分析結果，使用 Python 成功重現了混淆演算法，並驗證了其正確性。

---

### **專案六：破解「打包與解包」**

**目標:** 深入理解常見的二進位保護技術（加殼、混淆、加密打包、符號移除），並學習如何針對這些保護進行逆向工程。

**學習路徑:**
1.  **UPX 加殼與解殼:** **已完成** 🎉 ([分析報告](projects/06_packing_unpacking/01_upx_packing/01_upx_packing_analysis.md))
2.  **程式碼混淆與去混淆:** **已完成** 🎉 ([分析報告](projects/06_packing_unpacking/02_code_obfuscation/02_code_obfuscation_analysis.md))
3.  **自定義加密打包與解密:** **已完成** 🎉 ([分析報告](projects/06_packing_unpacking/03_custom_encryption_packing/03_custom_encryption_packing_analysis.md))
4.  **符號移除與逆向:** **已完成** 🎉 ([分析報告](projects/06_packing_unpacking/04_symbol_stripping/04_symbol_stripping_analysis.md))
5.  **綜合保護與逆向:** **已完成** 🎉 ([分析報告](projects/06_packing_unpacking/05_combined_protections/05_combined_protections_analysis.md))

### **專案狀態:** **已完成** 🎉
